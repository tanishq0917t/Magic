#ifndef __MLFW__VECTOR__DOUBLE__
#define __MLFW__VECTOR__DOUBLE__
struct __mlfw_row_vector_double;
struct __mlfw_column_vector_double;

typedef struct __mlfw_row_vector_double mlfw_row_vector_double;
typedef struct __mlfw_column_vector_double mlfw_column_vector_double;

//Column vector functions
mlfw_column_vector_double * mlfw_column_vector_double_create_new(dimension_t size);
void mlfw_column_vector_double_destroy(mlfw_column_vector_double *vector);
double mlfw_column_vector_double_get(mlfw_column_vector_double *vector,index_t index);
void mlfw_column_vector_double_set(mlfw_column_vector_double *vector,index_t index,double value);
mlfw_column_vector_double * mlfw_column_vector_create_new_filled(dimension_t size,double value);
mlfw_row_vector_double * mlfw_column_vector_double_transpose(mlfw_column_vector_double *vector);
dimension_t mlfw_column_vector_double_get_size(mlfw_column_vector_double *vector);
double mlfw_column_vector_double_mean(mlfw_column_vector_double *vector);
void mlfw_column_vector_double_to_csv(mlfw_column_vector_double *vector,char *CSV_FILE_NAME);
mlfw_column_vector_double * mlfw_column_vector_double_from_csv(char *CSV_FILE_NAME);

//Row vector functions
mlfw_row_vector_double * mlfw_row_vector_double_create_new(dimension_t size);
void mlfw_row_vector_double_destroy(mlfw_row_vector_double *vector);
double mlfw_row_vector_double_get(mlfw_row_vector_double *vector,index_t index);
void mlfw_row_vector_double_set(mlfw_row_vector_double *vector,index_t index,double value);
mlfw_row_vector_double * mlfw_row_vector_create_new_filled(dimension_t size,double value);
mlfw_column_vector_double * mlfw_row_vector_double_transpose(mlfw_row_vector_double *vector);
dimension_t mlfw_row_vector_double_get_size(mlfw_row_vector_double *vector);
double mlfw_row_vector_double_mean(mlfw_row_vector_double *vector);
void mlfw_row_vector_double_to_csv(mlfw_row_vector_double *vector,char *CSV_FILE_NAME);
mlfw_row_vector_double * mlfw_row_vector_double_from_csv(char *CSV_FILE_NAME);
#endif
