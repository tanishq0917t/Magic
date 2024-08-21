#include<stdio.h>
#include<stdlib.h>
#include<mlfw_vector.h>
typedef struct __mlfw_row_vec_string 
{

}mlfw_row_vec_string;

typedef struct __mlfw_column_vec_string
{

}mlfw_column_vec_string;

//row vector functions
mlfw_row_vec_string *mlfw_row_vec_string_create_new(dimension_t columns)
{
    return NULL;
}
void mlfw_row_vec_string_destroy(mlfw_row_vec_string *vector)
{

}
mlfw_row_vec_string *mlfw_row_vec_string_from_csv(const char *csv_file_name)
{
    return NULL;
}
void mlfw_row_vec_string_to_csv(mlfw_row_vec_string *vector,const char *csv_file_name)
{

}
void mlfw_row_vec_string_get(mlfw_row_vec_string *vector,index_t index,char **string)
{

}
void mlfw_row_vec_string_set(mlfw_row_vec_string *vector,index_t index,char *string)
{

}
dimension_t mlfw_row_vec_string_get_size(mlfw_row_vec_string *vector)
{
    return 0;
}
mlfw_column_vec_string * mlfw_row_vec_string_transpose(mlfw_row_vec_string *vector)
{
    return NULL;
}

//column vector functions
mlfw_column_vec_string *mlfw_column_vec_string_create_new(dimension_t columns)
{
    return NULL;
}
void mlfw_column_vec_string_destroy(mlfw_column_vec_string *vector)
{

}
mlfw_column_vec_string *mlfw_column_vec_string_from_csv(const char *csv_file_name)
{
    return NULL;
}
void mlfw_column_vec_string_to_csv(mlfw_column_vec_string *vector,const char *csv_file_name)
{

}
void mlfw_column_vec_string_get(mlfw_column_vec_string *vector,index_t index,char **string)
{

}
void mlfw_column_vec_string_set(mlfw_column_vec_string *vector,index_t index,char *string)
{

}
dimension_t mlfw_column_vec_string_get_size(mlfw_column_vec_string *vector)
{
    return 0;
}
mlfw_row_vec_string * mlfw_column_vec_string_transpose(mlfw_column_vec_string *vector)
{
    return NULL;
}