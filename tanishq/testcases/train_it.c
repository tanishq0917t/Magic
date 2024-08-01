#include <mlfw_matrix.h>
#include <mlfw_vector.h>
#include <mlfw_operations.h>
#include <stdlib.h>
#include <stdio.h>

uint64_t NO_OF_ITERATIONS = 5000000;

void train_it()
{
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

    dataset = mlfw_mat_double_from_csv("IceCreamSales.csv");
    if (dataset == NULL)
    {
        printf("Unable to load data from IceCreamSales.csv");
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
    while (k < NO_OF_ITERATIONS)
    {
        P = mlfw_multiply_double_matrix_with_column_vector(I, m);
        if (P == NULL)
        {
            printf("Low Memory");
            mlfw_mat_double_destroy(dataset);
            mlfw_mat_double_destroy(I);
            mlfw_mat_double_destroy(IT);
            mlfw_column_vector_double_destroy(A);
            mlfw_column_vector_double_destroy(m);
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
            return;
        }
        sum_of_squared_error_values = mlfw_column_vector_double_get(ETE, 0);
        final_error_value = sum_of_squared_error_values / (2 * I_rows);

        printf("Sum of square error values: %lf\n", sum_of_squared_error_values);
        printf("Final_error_value: %lf\n", final_error_value);

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
            return;
        }

        TMP = mlfw_multiply_column_vector_double_with_scaler(0.0001*(1.0 / I_rows), ITE);
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
    mlfw_column_vector_double_destroy(A);
    mlfw_column_vector_double_destroy(m);
    // mlfw_column_vector_double_destroy(P);
    // mlfw_column_vector_double_destroy(E);
    // mlfw_row_vector_double_destroy(ET);
    // mlfw_column_vector_double_destroy(ETE);
    // mlfw_mat_double_destroy(I);
    mlfw_mat_double_destroy(dataset);
}
int main()
{
    // test_new_and_create();
    train_it();
    return 0;
}