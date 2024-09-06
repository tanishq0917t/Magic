#ifndef __MLFW__MAT__STRING__
#define __MLFW__MAT__STRING__

struct __mlfw_mat_string;
typedef struct __mlfw_mat_string mlfw_mat_string;
mlfw_mat_string * mlfw_mat_string_create_new(dimension_t rows,dimension_t columns);
void mlfw_mat_string_destroy(mlfw_mat_string *matrix);
mlfw_mat_string * mlfw_mat_string_from_csv(const char *csv_file_name, mlfw_mat_string *matrix);
void mlfw_mat_string_to_csv(mlfw_mat_string *matrix,const char *csv_file_name);
void mlfw_mat_string_get(mlfw_mat_string *matrix,index_t row,index_t column,char **string);
void mlfw_mat_string_set(mlfw_mat_string *matrix,index_t row,index_t column,char *string);
void mlfw_mat_string_get_dimension(mlfw_mat_string *matrix,dimension_t *rows,dimension_t *columns);
#endif