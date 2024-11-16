#include <mlfw_matrix.h>
#include <mlfw_vector.h>
#include <mlfw_operations.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

uint64_t NO_OF_ITERATIONS = 5000000;
char *DATASET_FILE_NAME = NULL;
double LEARNING_RATE = 1.0;
dimension_t HISTORY_SIZE = 0;
char *HISTORY_FILE_NAME = NULL;
char *PARAMETERS_FILE_NAME = NULL;
char *GRAPH_FILE_NAME = NULL;
uint8_t STOP_FLAG = 0;

void train_it()
{
    int error_flag = 0;
    FILE *graph_file;
    double iteration_no;
    mlfw_mat_double *history;
    mlfw_row_vec_string *history_header, *param_header;
    int x;
    char header_title[16];
    dimension_t history_rows, history_columns;
    index_t history_index = 0;

    uint64_t k = 0;
    mlfw_mat_double *dataset;
    mlfw_row_vec_string *dataset_header;
    dimension_t dataset_rows, dataset_columns;
    mlfw_mat_double *I;
    mlfw_mat_double *IT;
    dimension_t I_rows, I_columns;
    mlfw_column_vector_double *A;
    mlfw_column_vector_double *m;
    mlfw_column_vector_double *P;
    mlfw_column_vector_double *E;
    mlfw_row_vector_double *ET;
    mlfw_column_vector_double *ETE;
    mlfw_column_vector_double *ITE;
    mlfw_column_vector_double *TMP;
    mlfw_column_vector_double *UM;

    double sum_of_squared_error_values;
    double final_error_value;

    dataset = mlfw_mat_double_from_csv(DATASET_FILE_NAME, NULL, &dataset_header);
    if (dataset == NULL)
    {
        printf("Unable to load data from %s\n", DATASET_FILE_NAME);
        return;
    }
    mlfw_mat_double_get_dimension(dataset, &dataset_rows, &dataset_columns);
    I_rows = dataset_rows;
    I_columns = dataset_columns;
    I = mlfw_mat_double_create_new(I_rows, I_columns);
    if (I == NULL)
    {
        printf("Low Memory");
        mlfw_mat_double_destroy(dataset);
        mlfw_row_vec_string_destroy(dataset_header);
        return;
    }
    /*
        1st Arg : Target Matrix
        2nd Arg : Source Matrix
        3rd Arg : target_row_index
        4th Arg : target_column_index
        5th Arg : source_from_row_index
        6th Arg : source_from_column_index
        7th Arg : source_to_row_index
        8th Arg : source_to_column_index
    */
    mlfw_mat_double_copy(I, dataset, 0, 1, 0, 0, dataset_rows - 1, dataset_columns - 2);

    /*
        1st Arg : matrix to fill
        2nd Arg : from row index
        3rd Arg : from column index
        4th Arg : upto row index
        5th Arg : upto column index
        6th Arg : what to fill
    */
    mlfw_mat_double_fill(I, 0, 0, dataset_rows - 1, 0, 1.0);

    IT = mlfw_mat_double_transpose(I, NULL);
    if (IT == NULL)
    {
        mlfw_mat_double_destroy(I);
        mlfw_mat_double_destroy(dataset);
        mlfw_row_vec_string_destroy(dataset_header);
        return;
    }
    /*
        1st Arg: source matrix
        2nd Arg: which column to use to create column vector
    */
    A = mlfw_mat_double_create_column_vec(dataset, dataset_columns - 1, NULL);
    if (A == NULL)
    {
        printf("Unable to create column vector");
        mlfw_mat_double_destroy(dataset);
        mlfw_mat_double_destroy(I);
        mlfw_mat_double_destroy(IT);
        mlfw_row_vec_string_destroy(dataset_header);
        return;
    }
    m = mlfw_column_vector_create_new_filled(I_columns, 0.0, NULL);
    if (m == NULL)
    {
        printf("Low Memory");
        mlfw_mat_double_destroy(dataset);
        mlfw_mat_double_destroy(I);
        mlfw_mat_double_destroy(IT);
        mlfw_column_vector_double_destroy(A);
        mlfw_row_vec_string_destroy(dataset_header);
        return;
    }
    // Operations starts from here
    history_rows = HISTORY_SIZE;
    history_columns = I_columns + 2;
    history = mlfw_mat_double_create_new(history_rows, history_columns);
    if (history == NULL)
    {
        printf("Low Memory\n");
        mlfw_mat_double_destroy(dataset);
        mlfw_mat_double_destroy(I);
        mlfw_mat_double_destroy(IT);
        mlfw_column_vector_double_destroy(A);
        mlfw_column_vector_double_destroy(m);
        mlfw_row_vec_string_destroy(dataset_header);
        return;
    }
    history_header = mlfw_row_vec_string_create_new(history_columns);
    if (history_header == NULL)
    {
        printf("Low Memory\n");
        mlfw_mat_double_destroy(dataset);
        mlfw_mat_double_destroy(I);
        mlfw_mat_double_destroy(IT);
        mlfw_column_vector_double_destroy(A);
        mlfw_column_vector_double_destroy(m);
        mlfw_row_vec_string_destroy(dataset_header);
        mlfw_mat_double_destroy(history);
        return;
    }
    param_header = mlfw_row_vec_string_create_new(I_columns);
    if (param_header == NULL)
    {
        printf("Low Memory\n");
        mlfw_mat_double_destroy(dataset);
        mlfw_mat_double_destroy(I);
        mlfw_mat_double_destroy(IT);
        mlfw_column_vector_double_destroy(A);
        mlfw_column_vector_double_destroy(m);
        mlfw_row_vec_string_destroy(dataset_header);
        mlfw_row_vec_string_destroy(history_header);
        mlfw_mat_double_destroy(history);
        return;
    }

    mlfw_row_vec_string_set(history_header, 0, "iteration");
    mlfw_row_vec_string_set(history_header, 1, "error");
    x = 2;
    while (x < history_columns)
    {
        sprintf(header_title, "theta(%d)", x - 2);
        mlfw_row_vec_string_set(history_header, x, header_title);
        mlfw_row_vec_string_set(param_header, x - 2, header_title);
        x++;
    }

    mlfw_mat_double_fill(history, 0, 0, history_rows - 1, history_columns - 1, 0.0);
    graph_file = fopen(GRAPH_FILE_NAME, "w");

    P = mlfw_column_vector_double_create_new(I_rows);
    if (P == NULL)
    {
        printf("Low Memory");
        mlfw_mat_double_destroy(dataset);
        mlfw_mat_double_destroy(I);
        mlfw_row_vec_string_destroy(dataset_header);
        mlfw_mat_double_destroy(IT);
        mlfw_column_vector_double_destroy(A);
        mlfw_column_vector_double_destroy(m);
        mlfw_mat_double_destroy(history);
        mlfw_row_vec_string_destroy(history_header);
        mlfw_row_vec_string_destroy(param_header);
        return;
    }
    E = mlfw_column_vector_double_create_new(I_rows);
    if (E == NULL)
    {
        printf("Low Memory");
        mlfw_row_vec_string_destroy(dataset_header);
        mlfw_mat_double_destroy(dataset);
        mlfw_mat_double_destroy(I);
        mlfw_mat_double_destroy(IT);
        mlfw_column_vector_double_destroy(A);
        mlfw_column_vector_double_destroy(m);
        mlfw_column_vector_double_destroy(P);
        mlfw_mat_double_destroy(history);
        mlfw_row_vec_string_destroy(history_header);
        mlfw_row_vec_string_destroy(param_header);
        return;
    }
    ET = mlfw_row_vector_double_create_new(I_rows);
    if (ET == NULL)
    {
        printf("Low Memory");
        mlfw_row_vec_string_destroy(dataset_header);
        mlfw_mat_double_destroy(dataset);
        mlfw_mat_double_destroy(I);
        mlfw_mat_double_destroy(IT);
        mlfw_column_vector_double_destroy(A);
        mlfw_column_vector_double_destroy(m);
        mlfw_column_vector_double_destroy(P);
        mlfw_column_vector_double_destroy(E);
        mlfw_mat_double_destroy(history);
        mlfw_row_vec_string_destroy(history_header);
        mlfw_row_vec_string_destroy(param_header);
        return;
    }
    ETE = mlfw_column_vector_double_create_new(1);
    if (ETE == NULL)
    {
        printf("Low Memory");
        mlfw_row_vec_string_destroy(dataset_header);
        mlfw_mat_double_destroy(dataset);
        mlfw_mat_double_destroy(I);
        mlfw_column_vector_double_destroy(A);
        mlfw_mat_double_destroy(IT);
        mlfw_column_vector_double_destroy(m);
        mlfw_column_vector_double_destroy(P);
        mlfw_column_vector_double_destroy(E);
        mlfw_row_vector_double_destroy(ET);
        mlfw_mat_double_destroy(history);
        mlfw_row_vec_string_destroy(history_header);
        mlfw_row_vec_string_destroy(param_header);
        return;
    }
    ITE = mlfw_column_vector_double_create_new(I_columns);
    if (ITE == NULL)
    {
        printf("Low memory\n");
        mlfw_mat_double_destroy(dataset);
        mlfw_row_vec_string_destroy(dataset_header);
        mlfw_mat_double_destroy(I);
        mlfw_mat_double_destroy(IT);
        mlfw_column_vector_double_destroy(A);
        mlfw_column_vector_double_destroy(m);
        mlfw_column_vector_double_destroy(P);
        mlfw_column_vector_double_destroy(E);
        mlfw_row_vector_double_destroy(ET);
        mlfw_column_vector_double_destroy(ETE);
        mlfw_mat_double_destroy(history);
        mlfw_row_vec_string_destroy(history_header);
        mlfw_row_vec_string_destroy(param_header);
        return;
    }
    TMP = mlfw_column_vector_double_create_new(I_columns);
    if (TMP == NULL)
    {
        printf("Low memory\n");
        mlfw_mat_double_destroy(dataset);
        mlfw_row_vec_string_destroy(dataset_header);
        mlfw_mat_double_destroy(I);
        mlfw_mat_double_destroy(IT);
        mlfw_column_vector_double_destroy(A);
        mlfw_column_vector_double_destroy(m);
        mlfw_column_vector_double_destroy(P);
        mlfw_column_vector_double_destroy(E);
        mlfw_row_vector_double_destroy(ET);
        mlfw_column_vector_double_destroy(ITE);
        mlfw_column_vector_double_destroy(ETE);
        mlfw_mat_double_destroy(history);
        mlfw_row_vec_string_destroy(history_header);
        mlfw_row_vec_string_destroy(param_header);
        return;
    }
    UM = mlfw_column_vector_double_create_new(I_columns);
    if (UM == NULL)
    {
        printf("Low memory\n");
        mlfw_mat_double_destroy(dataset);
        mlfw_row_vec_string_destroy(dataset_header);
        mlfw_mat_double_destroy(I);
        mlfw_mat_double_destroy(IT);
        mlfw_column_vector_double_destroy(A);
        mlfw_column_vector_double_destroy(m);
        mlfw_column_vector_double_destroy(P);
        mlfw_column_vector_double_destroy(E);
        mlfw_row_vector_double_destroy(ET);
        mlfw_column_vector_double_destroy(ITE);
        mlfw_column_vector_double_destroy(ETE);
        mlfw_column_vector_double_destroy(TMP);
        mlfw_mat_double_destroy(history);
        mlfw_row_vec_string_destroy(history_header);
        mlfw_row_vec_string_destroy(param_header);
        return;
    }

    while (STOP_FLAG == 0)
    {
        if (k == NO_OF_ITERATIONS)
            break;
        P = mlfw_multiply_double_matrix_with_column_vector(I, m, P);
        if (P == NULL)
        {
            error_flag = 1;
            break;
        }
        /*
            x=y-z
            1st Arg : left operand
            2nd Arg : right operand
        */
        E = mlfw_subtract_double_column_vector(P, A, E);
        if (E == NULL)
        {
            error_flag = 1;
            break;
        }
        ET = mlfw_column_vector_double_transpose(E, ET);
        if (ET == NULL)
        {
            error_flag = 1;
            break;
        }
        ETE = mlfw_multiply_double_row_vector_with_column_vector(ET, E, ETE);
        if (ETE == NULL)
        {
            error_flag = 1;
            break;
        }
        sum_of_squared_error_values = mlfw_column_vector_double_get(ETE, 0);
        final_error_value = sum_of_squared_error_values / (2 * I_rows);

        // printf("Sum of square error values: %lf\n", sum_of_squared_error_values);
        // printf("Final_error_value: %lf\n", final_error_value);
        printf("Iteration Number: %" PRIu64 ",Error: %41.15lf\n", k, final_error_value);

        fprintf(graph_file, "%" PRIu64 ",%lf\n", k, final_error_value);

        iteration_no = (double)k;
        // logic to add history starts here
        if (history_index == HISTORY_SIZE)
        {
            for (index_t i = 1; i < HISTORY_SIZE; i++)
            {
                for (index_t j = 0; j < history_columns; j++)
                {
                    mlfw_mat_double_set(history, i - 1, j, mlfw_mat_double_get(history, i, j));
                }
            }
            history_index--;
        }
        mlfw_mat_double_set(history, history_index, 0, iteration_no);
        mlfw_mat_double_set(history, history_index, 1, final_error_value);
        index_t j = mlfw_column_vector_double_get_size(m);
        for (index_t i = 0; i < j; i++)
        {
            mlfw_mat_double_set(history, history_index, i + 2, mlfw_column_vector_double_get(m, i));
        }
        history_index++;

        ITE = mlfw_multiply_double_matrix_with_column_vector(IT, E, ITE);
        if (ITE == NULL)
        {
            error_flag = 1;
            break;
        }

        TMP = mlfw_multiply_column_vector_double_with_scaler(LEARNING_RATE * (1.0 / I_rows), ITE, TMP);
        if (TMP == NULL)
        {
            error_flag = 1;
            break;
        }

        UM = mlfw_subtract_double_column_vector(m, TMP, UM);
        if (UM == NULL)
        {
            error_flag = 1;
            break;
        }
        // Setting data from UM to m
        for (index_t i = 0; i < I_columns; i++)
        {
            mlfw_column_vector_double_set(m, i, mlfw_column_vector_double_get(UM, i));
        }

        k++;
    }
    fclose(graph_file);
    if (error_flag == 1)
    {
        mlfw_column_vector_double_destroy(P);
        mlfw_column_vector_double_destroy(E);
        mlfw_row_vector_double_destroy(ET);
        mlfw_column_vector_double_destroy(ETE);
        mlfw_column_vector_double_destroy(ITE);
        mlfw_column_vector_double_destroy(TMP);
        mlfw_column_vector_double_destroy(UM);
        mlfw_column_vector_double_destroy(m);
        mlfw_mat_double_destroy(I);
        mlfw_mat_double_destroy(IT);
        mlfw_row_vec_string_destroy(dataset_header);
        mlfw_column_vector_double_destroy(A);
        mlfw_column_vector_double_destroy(m);
        mlfw_mat_double_destroy(history);
        mlfw_row_vec_string_destroy(history_header);
        mlfw_row_vec_string_destroy(param_header);
    }

    // Dumping the history and model values data to csv
    mlfw_mat_double_to_csv(history, HISTORY_FILE_NAME, history_header);
    mlfw_column_vector_double_to_csv(m, PARAMETERS_FILE_NAME);

    // Release resources
    mlfw_column_vector_double_destroy(P);
    mlfw_column_vector_double_destroy(E);
    mlfw_row_vector_double_destroy(ET);
    mlfw_column_vector_double_destroy(ETE);
    mlfw_column_vector_double_destroy(ITE);
    mlfw_column_vector_double_destroy(TMP);
    mlfw_column_vector_double_destroy(UM);
    mlfw_column_vector_double_destroy(m);
    mlfw_mat_double_destroy(I);
    mlfw_mat_double_destroy(IT);
    mlfw_row_vec_string_destroy(dataset_header);
    mlfw_column_vector_double_destroy(A);
    mlfw_mat_double_destroy(history);
    mlfw_row_vec_string_destroy(history_header);
    mlfw_row_vec_string_destroy(param_header);
}

void *thread_function(void *p)
{
    train_it(); // train_it will be executed on thread on which thread_function is being executed
    return NULL;
}

int main(int argc, char *argv[])
{
    // test_new_and_create();
    pthread_t thread_id;
    char m;
    int result;

    if (argc != 7 && argc != 8)
    {
        printf("Usage: train.out dataset_name learning_rate history_size history_file_name PARAMETER_FILE GRAPH_FILE no_of_iterations(optional)\n");
        return 0;
    }
    char *ptr = NULL;
    DATASET_FILE_NAME = argv[1];
    LEARNING_RATE = strtod(argv[2], &ptr);
    ptr = NULL;
    HISTORY_SIZE = (dimension_t)strtoull(argv[3], &ptr, 10);
    if (HISTORY_SIZE < 10)
    {
        printf("History Size be greater than 10\n");
        return 0;
    }
    HISTORY_FILE_NAME = argv[4];
    PARAMETERS_FILE_NAME = argv[5];
    GRAPH_FILE_NAME = argv[6];
    if (argc == 8)
    {
        ptr = NULL;
        NO_OF_ITERATIONS = strtoull(argv[7], &ptr, 10);
    }
    result = pthread_create(&thread_id, NULL, thread_function, NULL);
    if (result != 0)
    {
        printf("Unable to create Thread\n");
        return 0;
    }
    while (1)
    {
        m = getchar();
        if (m == '\n')
        {
            STOP_FLAG = 1;
            break;
        }
    }
    pthread_join(thread_id, NULL);
    return 0;
}