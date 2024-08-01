#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<mlfw_operations.h>
#include<stdio.h>
#include<stdlib.h>

mlfw_column_vector_double * mlfw_multiply_double_row_vector_with_column_vector(mlfw_row_vector_double *left_vector,mlfw_column_vector_double *right_vector)
{
    if(left_vector==NULL || right_vector==NULL) return NULL;
    mlfw_column_vector_double *product_vector;
    dimension_t left_vector_size;
    dimension_t right_vector_size;
    double left_vector_value,right_vector_value;
    left_vector_size=mlfw_row_vector_double_get_size(left_vector);
    right_vector_size=mlfw_column_vector_double_get_size(right_vector);
    if(left_vector_size!=right_vector_size) return NULL;
    product_vector=mlfw_column_vector_double_create_new(1);
    if(product_vector==NULL) return NULL;
    double product=0.0;
    for(index_t i=0;i<left_vector_size;i++)
    {
        left_vector_value=mlfw_row_vector_double_get(left_vector,i);
        right_vector_value=mlfw_column_vector_double_get(right_vector,i);
        product+=(left_vector_value*right_vector_value);
    }
    mlfw_column_vector_double_set(product_vector,0,product);
    return product_vector;
}
mlfw_column_vector_double * mlfw_multiply_double_matrix_with_column_vector(mlfw_mat_double *left_matrix,mlfw_column_vector_double *right_vector)
{
    if(left_matrix==NULL || right_vector==NULL) return NULL;
    dimension_t left_matrix_rows,left_matrix_columns;
    dimension_t right_vector_size;
    double left_matrix_value,right_vector_value;
    double product;
    mlfw_column_vector_double *product_vector;
    mlfw_mat_double_get_dimension(left_matrix,&left_matrix_rows,&left_matrix_columns);
    right_vector_size=mlfw_column_vector_double_get_size(right_vector);
    product_vector=mlfw_column_vector_double_create_new(left_matrix_rows);
    if(left_matrix_columns!=right_vector_size) return NULL;
    if(product_vector==NULL) return NULL;
    for(index_t r=0;r<left_matrix_rows;r++)
    {
        product=0.0;
        for(index_t c=0;c<left_matrix_columns;c++)
        {
            left_matrix_value=mlfw_mat_double_get(left_matrix,r,c);
            right_vector_value=mlfw_column_vector_double_get(right_vector,c);
            product+=left_matrix_value*right_vector_value;
        }
        mlfw_column_vector_double_set(product_vector,r,product);
    }
    return product_vector;
}
mlfw_column_vector_double * mlfw_subtract_double_column_vector(mlfw_column_vector_double *left_vector,mlfw_column_vector_double *right_vector)
{
    if(left_vector==NULL || right_vector==NULL) return NULL;
    mlfw_column_vector_double *difference_vector;
    dimension_t left_vector_size;
    dimension_t right_vector_size;
    double left_vector_value,right_vector_value;
    left_vector_size=mlfw_column_vector_double_get_size(left_vector);
    right_vector_size=mlfw_column_vector_double_get_size(right_vector);
    if(left_vector_size!=right_vector_size) return NULL;
    difference_vector=mlfw_column_vector_double_create_new(left_vector_size);
    if(difference_vector==NULL) return NULL;
    double difference=0.0;
    for(index_t i=0;i<left_vector_size;i++)
    {
        left_vector_value=mlfw_column_vector_double_get(left_vector,i);
        right_vector_value=mlfw_column_vector_double_get(right_vector,i);
        difference=(left_vector_value-right_vector_value);
        mlfw_column_vector_double_set(difference_vector,i,difference);
    }
    return difference_vector;   
}
mlfw_column_vector_double * mlfw_multiply_column_vector_double_with_scaler(double scaler_value,mlfw_column_vector_double *vector)
{
    if(vector==NULL) return NULL;
    mlfw_column_vector_double *multiplied_vector;
    dimension_t size=mlfw_column_vector_double_get_size(vector);
    multiplied_vector=mlfw_column_vector_double_create_new(size);
    double curr_val,multiplied_val;
    for(index_t i=0;i<size;i++)
    {
        curr_val=mlfw_column_vector_double_get(vector,i);
        multiplied_val=curr_val*scaler_value;
        mlfw_column_vector_double_set(multiplied_vector,i,multiplied_val);
    }
    return multiplied_vector;
}