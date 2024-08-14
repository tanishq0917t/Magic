#include<mlfw_matrix.h>
#include<stdio.h>
#include<stdlib.h>
int main(int argc,char *argv[])
{
    mlfw_mat_double *shuffled_matrix;
    mlfw_mat_double *matrix;
    mlfw_mat_double *minor_matrix;
    mlfw_mat_double *major_matrix;
    dimension_t minor_rows;
    dimension_t major_rows;
    dimension_t shuffled_matrix_rows,shuffled_matrix_columns;
    char *dataset_file;
    char *test_file;
    char *train_file;
    int minor_percentage;
    if(argc!=5)
    {
        printf("Usage: [create_test_train_dataset dataset test_file train_file minor_percentage]\n");
        return 0;
    }    
    dataset_file=argv[1];
    test_file=argv[2];
    train_file=argv[3];
    minor_percentage=atoi(argv[4]);

    matrix=mlfw_mat_double_from_csv(dataset_file);
    mlfw_mat_double_get_dimension(matrix,&shuffled_matrix_rows,&shuffled_matrix_columns);

    if(matrix==NULL)
    {
        printf("Unable to load %s\n",dataset_file);
        return 0;
    }

    if(minor_percentage<=0 || minor_percentage>50)
    {
        printf("Invalid minor percentage\n");
        return 0;
    }
    
    shuffled_matrix=mlfw_mat_double_shuffle(matrix,3);
    if(shuffled_matrix==NULL)
    {
        printf("Unable to create test/train data files\n");
        mlfw_mat_double_destroy(matrix);
        return 0;
    }
    mlfw_mat_double_get_dimension(shuffled_matrix,&shuffled_matrix_rows,&shuffled_matrix_columns);
    minor_rows=(dimension_t)(minor_percentage*shuffled_matrix_rows)/100;
    major_rows=shuffled_matrix_rows-minor_rows;

    minor_matrix=mlfw_mat_double_create_new(minor_rows,shuffled_matrix_columns);
    if(minor_matrix==NULL)
    {
        mlfw_mat_double_destroy(matrix);
        mlfw_mat_double_destroy(shuffled_matrix);
        printf("Unable to create test/train data files\n");
        return 0;
    }
    major_matrix=mlfw_mat_double_create_new(major_rows,shuffled_matrix_columns);
    if(major_matrix==NULL)
    {
        mlfw_mat_double_destroy(matrix);
        mlfw_mat_double_destroy(shuffled_matrix);
        mlfw_mat_double_destroy(minor_matrix);
        printf("Unable to create test/train data files\n");
        return 0;
    }
    mlfw_mat_double_copy(minor_matrix,shuffled_matrix,0,0,0,0,minor_rows-1,shuffled_matrix_columns-1);
    mlfw_mat_double_copy(major_matrix,shuffled_matrix,0,0,minor_rows,0,shuffled_matrix_rows-1,shuffled_matrix_columns-1);
    mlfw_mat_double_to_csv(minor_matrix,test_file);
    mlfw_mat_double_to_csv(major_matrix,train_file);

    mlfw_mat_double_destroy(matrix);
    mlfw_mat_double_destroy(shuffled_matrix);
    mlfw_mat_double_destroy(minor_matrix);
    mlfw_mat_double_destroy(major_matrix);
    return 0;
}