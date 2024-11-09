#include<mlfw_matrix.h>
#include<stdio.h>
#include<stdlib.h>
#include<mlfw_vector.h>
int main(int argc,char *argv[])
{
    char *input_file,*output_file,*min_max_file;
    double min,max,scaled_value,value;
    mlfw_mat_double *matrix,*min_max_matrix;
    mlfw_row_vec_string *mat_header,*min_max_header;
    dimension_t matrix_rows,matrix_columns,min_max_rows,min_max_columns;
    if(argc!=4)
    {
        printf("Usage: [reverse_scale_output.out  input_file  output_file  min_max_file]\n");
        return 0;
    }
    input_file=argv[1];
    output_file=argv[2];
    min_max_file=argv[3];
    matrix=mlfw_mat_double_from_csv(input_file,NULL,&mat_header);
    if(matrix==NULL)
    {
        printf("Low memory\n");
        return 0;
    }
    mlfw_mat_double_get_dimension(matrix,&matrix_rows,&matrix_columns);
    min_max_matrix=mlfw_mat_double_from_csv(min_max_file,NULL,&min_max_header);
    if(min_max_matrix==NULL)
    {
        mlfw_mat_double_destroy(matrix);
        mlfw_row_vec_string_destroy(mat_header);
        return 0;
    }
    mlfw_mat_double_get_dimension(min_max_matrix,&min_max_rows,&min_max_columns);
    min=mlfw_mat_double_get(min_max_matrix,0,min_max_columns-1);
    max=mlfw_mat_double_get(min_max_matrix,1,min_max_columns-1);
    for(index_t r=0;r<matrix_rows;r++)
    {
        for(index_t c=matrix_columns-2;c<matrix_columns;c++)
        {
            scaled_value=mlfw_mat_double_get(matrix,r,c);
            value=(scaled_value*(max-min))+min;
            mlfw_mat_double_set(matrix,r,c,value);
        }
    }
    mlfw_mat_double_to_csv(matrix,output_file,mat_header);
    mlfw_mat_double_destroy(matrix);
    mlfw_mat_double_destroy(min_max_matrix);
    mlfw_row_vec_string_destroy(mat_header);
    mlfw_row_vec_string_destroy(min_max_header);
    return 0;
}