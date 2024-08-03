#include <mlfw_matrix.h>
#include <mlfw_vector.h>
#include <mlfw_operations.h>
#include <stdlib.h>
#include <stdio.h>

char *DATASET_FILE_NAME = NULL;
char *RESULT_FILE_NAME = NULL;

void test_it()
{
    FILE *results_file;
    mlfw_mat_double *dataset;
    dimension_t dataset_rows, dataset_columns;
    mlfw_mat_double *I;
    mlfw_mat_double *IT;
    dimension_t I_rows, I_columns;
    mlfw_column_vector_double *A;
    mlfw_column_vector_double *m;
    mlfw_column_vector_double *P;

    double sum_of_squared_error_values;
    double final_error_value;

    dataset = mlfw_mat_double_from_csv(DATASET_FILE_NAME);
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
        return;
    }
    
    mlfw_mat_double_copy(I, dataset, 0, 1, 0, 0, dataset_rows - 1, 0);
    mlfw_mat_double_fill(I, 0, 0, dataset_rows - 1, 0, 1.0);

    m = mlfw_column_vector_double_create_new(I_columns);
    if (m == NULL)
    {
        printf("Low Memory");
        mlfw_mat_double_destroy(dataset);
        mlfw_mat_double_destroy(I);
        return;
    }

    mlfw_column_vector_double_set(m,0,-33.790833);
    mlfw_column_vector_double_set(m,1,1.192568);

    P = mlfw_multiply_double_matrix_with_column_vector(I, m);
    if (P == NULL)
    {
        printf("Low Memory");
        mlfw_mat_double_destroy(dataset);
        mlfw_mat_double_destroy(I);

        mlfw_column_vector_double_destroy(m);
        return;
    }
    // logic to write results in results_file
    results_file=fopen(RESULT_FILE_NAME,"w");
    for(index_t r=0;r<dataset_rows;r++)
    {
        for(index_t c=0;c<dataset_columns;c++)
        {
            fprintf(results_file,"%lf,",mlfw_mat_double_get(dataset,r,c));
        }
        fprintf(results_file,"%lf\n",mlfw_column_vector_double_get(P,r));
    }
    fclose(results_file);
    mlfw_mat_double_destroy(dataset);
    mlfw_mat_double_destroy(I);
    mlfw_column_vector_double_destroy(P);
    mlfw_column_vector_double_destroy(m);
}

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        printf("Usage: test_it.out dataset_name results_file_name\n");
        return 0;
    }
    DATASET_FILE_NAME = argv[1];
    RESULT_FILE_NAME = argv[2];
    test_it();
    printf("Results are stored in %s\n", RESULT_FILE_NAME);
    return 0;
}
