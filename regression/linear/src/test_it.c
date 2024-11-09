#include <mlfw_matrix.h>
#include <mlfw_vector.h>
#include <mlfw_operations.h>
#include <stdlib.h>
#include <stdio.h>

char *DATASET_FILE_NAME = NULL;
char *RESULT_FILE_NAME = NULL;
char *PARAMETERS_FILE_NAME = NULL;
void test_it()
{
    FILE *results_file;
    mlfw_mat_double *dataset;
    dimension_t dataset_rows, dataset_columns;
    mlfw_mat_double *I;
    dimension_t I_rows, I_columns;
    mlfw_column_vector_double *m=NULL;
    mlfw_column_vector_double *P;
    mlfw_row_vec_string *header,*param_header;

    dataset = mlfw_mat_double_from_csv(DATASET_FILE_NAME,NULL,&header);
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
        printf("Low Memory\n");
        mlfw_mat_double_destroy(dataset);
        mlfw_row_vec_string_destroy(header);
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
    mlfw_mat_double_copy(I, dataset, 0, 1, 0, 0, dataset_rows - 1, dataset_columns-1);

    /*
        1st Arg : matrix to fill
        2nd Arg : from row index
        3rd Arg : from column index
        4th Arg : upto row index
        5th Arg : upto column index
        6th Arg : what to fill
    */
    mlfw_mat_double_fill(I, 0, 0, dataset_rows - 1, 0, 1.0);

    //m = mlfw_column_vector_double_create_new(I_columns);
    m=mlfw_column_vector_double_from_csv(PARAMETERS_FILE_NAME,NULL,&param_header);
    if (m == NULL)
    {

        printf("M=NULL Low Memory\n");
        mlfw_mat_double_destroy(dataset);
        mlfw_mat_double_destroy(I);
        mlfw_row_vec_string_destroy(header);
        return;
    }

    P = mlfw_multiply_double_matrix_with_column_vector(I, m,NULL);
    if (P == NULL)
    {
        printf("Low Memory\n");
        mlfw_mat_double_destroy(dataset);
        mlfw_mat_double_destroy(I);
        mlfw_row_vec_string_destroy(header);
        mlfw_row_vec_string_destroy(param_header);
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
    mlfw_row_vec_string_destroy(header);
    mlfw_row_vec_string_destroy(param_header);
}

int main(int argc, char *argv[])
{

    if (argc != 4)
    {
        printf("Usage: test_it.out dataset_name parameter_file_name results_file_name\n");
        return 0;
    }
    DATASET_FILE_NAME = argv[1];
    PARAMETERS_FILE_NAME = argv[2];
    RESULT_FILE_NAME = argv[3];
    test_it();
    printf("Results are stored in %s\n", RESULT_FILE_NAME);
    return 0;
}