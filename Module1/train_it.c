#include <mlfw_matrix.h>
#include <mlfw_vector.h>
#include <mlfw_operations.h>
#include <stdlib.h>
#include <stdio.h>
#include<pthread.h>


uint64_t NO_OF_ITERATIONS = 5000000;
char * DATASET_FILE_NAME=NULL;
double LEARNING_RATE=1.0;
dimension_t HISTORY_SIZE=0;
char *HISTORY_FILE_NAME=NULL;
uint8_t STOP_FLAG=0;

void train_it()
{
    FILE *graph_file;
    double iteration_no;
    mlfw_mat_double *history;
    dimension_t history_rows,history_columns;
    index_t history_index=0;

    uint64_t k = 0;
    mlfw_mat_double *dataset;
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

    dataset = mlfw_mat_double_from_csv(DATASET_FILE_NAME);
    if (dataset == NULL)
    {
        printf("Unable to load data from %s\n",DATASET_FILE_NAME);
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
    mlfw_mat_double_copy(I, dataset, 0, 1, 0, 0, dataset_rows - 1, 0);

    /*
        1st Arg : matrix to fill
        2nd Arg : from row index
        3rd Arg : from column index
        4th Arg : upto row index
        5th Arg : upto column index
        6th Arg : what to fill
    */
    mlfw_mat_double_fill(I, 0, 0, dataset_rows - 1, 0, 1.0);

    IT = mlfw_mat_double_transpose(I);
    if (IT == NULL)
    {
        mlfw_mat_double_destroy(I);
        mlfw_mat_double_destroy(dataset);
        return;
    }
    /*
        1st Arg: source matrix
        2nd Arg: which column to use to create column vector
    */
    A = mlfw_mat_double_create_column_vec(dataset, 1);
    if (A == NULL)
    {
        printf("Unable to create column vector");
        mlfw_mat_double_destroy(dataset);
        mlfw_mat_double_destroy(I);
        mlfw_mat_double_destroy(IT);
        return;
    }
    m = mlfw_column_vector_create_new_filled(I_columns, 0.0);
    if (m == NULL)
    {
        printf("Low Memory");
        mlfw_mat_double_destroy(dataset);
        mlfw_mat_double_destroy(I);
        mlfw_mat_double_destroy(IT);
        mlfw_column_vector_double_destroy(A);
        return;
    }
    // Operations starts from here
    history_rows=HISTORY_SIZE;
    history_columns=I_columns+2;
    history=mlfw_mat_double_create_new(history_rows,history_columns);
    if(history==NULL)
    {
        printf("Low Memory\n");
        mlfw_mat_double_destroy(dataset);
        mlfw_mat_double_destroy(I);
        mlfw_mat_double_destroy(IT);
        mlfw_column_vector_double_destroy(A);
        mlfw_column_vector_double_destroy(m);
        return;
    }
    mlfw_mat_double_fill(history,0,0,history_rows-1,history_columns-1,0.0);
    graph_file=fopen("graph.csv","w");
    while (STOP_FLAG==0)
    {
        if(k==NO_OF_ITERATIONS) break;
        P = mlfw_multiply_double_matrix_with_column_vector(I, m);
        if (P == NULL)
        {
            printf("Low Memory");
            mlfw_mat_double_destroy(dataset);
            mlfw_mat_double_destroy(I);
            mlfw_mat_double_destroy(IT);
            mlfw_column_vector_double_destroy(A);
            mlfw_column_vector_double_destroy(m);
            mlfw_mat_double_destroy(history);
            return;
        }
        /*
            x=y-z
            1st Arg : left operand
            2nd Arg : right operand
        */
        E = mlfw_subtract_double_column_vector(P, A);
        if (E == NULL)
        {
            printf("Low Memory");
            mlfw_mat_double_destroy(dataset);
            mlfw_mat_double_destroy(I);
            mlfw_mat_double_destroy(IT);
            mlfw_column_vector_double_destroy(A);
            mlfw_column_vector_double_destroy(m);
            mlfw_column_vector_double_destroy(P);
            mlfw_mat_double_destroy(history);
            return;
        }
        ET = mlfw_column_vector_double_transpose(E);
        if (ET == NULL)
        {
            printf("Low Memory");
            mlfw_mat_double_destroy(dataset);
            mlfw_mat_double_destroy(I);
            mlfw_mat_double_destroy(IT);
            mlfw_column_vector_double_destroy(A);
            mlfw_column_vector_double_destroy(m);
            mlfw_column_vector_double_destroy(P);
            mlfw_column_vector_double_destroy(E);
            mlfw_mat_double_destroy(history);
            return;
        }
        ETE = mlfw_multiply_double_row_vector_with_column_vector(ET, E);
        if (ETE == NULL)
        {
            printf("Low Memory");
            mlfw_mat_double_destroy(dataset);
            mlfw_mat_double_destroy(I);
            mlfw_column_vector_double_destroy(A);
            mlfw_mat_double_destroy(IT);
            mlfw_column_vector_double_destroy(m);
            mlfw_column_vector_double_destroy(P);
            mlfw_column_vector_double_destroy(E);
            mlfw_row_vector_double_destroy(ET);
            mlfw_mat_double_destroy(history);
            return;
        }
        sum_of_squared_error_values = mlfw_column_vector_double_get(ETE, 0);
        final_error_value = sum_of_squared_error_values / (2 * I_rows);

        //printf("Sum of square error values: %lf\n", sum_of_squared_error_values);
        //printf("Final_error_value: %lf\n", final_error_value);
        printf("Iteration Number: %" PRIu64",Error: %41.15lf\n",k,final_error_value);

        fprintf(graph_file,"%" PRIu64 ",%lf\n",k,final_error_value);

        iteration_no=(double)k;
        //logic to add history starts here
        if(history_index==HISTORY_SIZE)
        {
            for(index_t i=1;i<HISTORY_SIZE;i++)
            {
                for(index_t j=0;j<history_columns;j++)
                {
                    mlfw_mat_double_set(history,i-1,j,mlfw_mat_double_get(history,i,j));
                }
            }
            history_index--;
        }
        mlfw_mat_double_set(history,history_index,0,iteration_no);
        mlfw_mat_double_set(history,history_index,1,final_error_value);
        index_t j=mlfw_column_vector_double_get_size(m);
        for(index_t i=0;i<j;i++)
        {
            mlfw_mat_double_set(history,history_index,i+2,mlfw_column_vector_double_get(m,i));
        }
        history_index++;

        
        ITE = mlfw_multiply_double_matrix_with_column_vector(IT, E);
        if (ITE == NULL)
        {
            printf("Low memory\n");
            mlfw_mat_double_destroy(dataset);
            mlfw_mat_double_destroy(I);
            mlfw_mat_double_destroy(IT);
            mlfw_column_vector_double_destroy(A);
            mlfw_column_vector_double_destroy(m);
            mlfw_column_vector_double_destroy(P);
            mlfw_column_vector_double_destroy(E);
            mlfw_row_vector_double_destroy(ET);
            mlfw_column_vector_double_destroy(ETE);
            mlfw_mat_double_destroy(history);
            return;
        }

        TMP = mlfw_multiply_column_vector_double_with_scaler(LEARNING_RATE*(1.0 / I_rows), ITE);
        if (TMP == NULL)
        {
            printf("Low memory\n");
            mlfw_mat_double_destroy(dataset);
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
            return;
        }

        UM = mlfw_subtract_double_column_vector(m, TMP);
        if (UM == NULL)
        {
            printf("Low memory\n");
            mlfw_mat_double_destroy(dataset);
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
            return;
        }
        mlfw_column_vector_double_destroy(m);
        m=UM; 
        k++;
        mlfw_column_vector_double_destroy(P);
        mlfw_column_vector_double_destroy(E);
        mlfw_row_vector_double_destroy(ET);
        mlfw_column_vector_double_destroy(ITE);
        mlfw_column_vector_double_destroy(ETE);
        mlfw_column_vector_double_destroy(TMP);
    }
    fclose(graph_file);
    mlfw_mat_double_to_csv(history,HISTORY_FILE_NAME);
    mlfw_column_vector_double_destroy(A);
    mlfw_column_vector_double_destroy(m);
    // mlfw_column_vector_double_destroy(P);
    // mlfw_column_vector_double_destroy(E);
    // mlfw_row_vector_double_destroy(ET);
    // mlfw_column_vector_double_destroy(ETE);
    // mlfw_mat_double_destroy(I);
    mlfw_mat_double_destroy(dataset);
}

void * thread_function(void *p)
{
    train_it(); //train_it will be executed on thread on which thread_function is being executed
    return NULL;
}

int main(int argc,char *argv[])
{
    // test_new_and_create();
    pthread_t thread_id;
    char m;
    int result;

    if(argc!=5 && argc!=6)
    {
        printf("Usage: train.out dataset_name learning_rate history_size history_file_name no_of_iterations(optional)\n");
        return 0;
    }
    char *ptr=NULL;
    DATASET_FILE_NAME=argv[1];
    LEARNING_RATE=strtod(argv[2],&ptr);
    ptr=NULL;
    HISTORY_SIZE=(dimension_t)strtoull(argv[3],&ptr,10);
    if(HISTORY_SIZE<=0)
    {
        printf("History Size be greater than 10\n");
        return 0;
    }
    HISTORY_FILE_NAME=argv[4];
    if(argc==6)
    {
        ptr=NULL;
        NO_OF_ITERATIONS=strtoull(argv[5],&ptr,10);
    }
    result=pthread_create(&thread_id,NULL,thread_function,NULL);
    if(result!=0)
    {
        printf("Unable to create Thread\n");
        return 0;
    }
    while(1)
    {
        m=getchar();
        if(m=='\n')
        {
            STOP_FLAG=1;
            break;
        }
    }
    pthread_join(thread_id,NULL);
    return 0;
}
