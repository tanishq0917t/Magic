#ifndef __MLFW__SCALE__DOUBLE__
#define __MLFW__SCALE__DOUBLE__
#include<mlfw_matrix.h>
mlfw_mat_double *mlfw_scale_double_min_max(mlfw_mat_double *matrix,index_t start_r,index_t start_c,index_t end_r,index_t end_c,char *min_max_file,mlfw_mat_double *scaled_matrix);

mlfw_mat_double *mlfw_scale_double_min_max_with_given_file(mlfw_mat_double *matrix,index_t start_r,index_t start_c,index_t end_r,index_t end_c,mlfw_mat_double *min_max_matrix,mlfw_mat_double *scaled_matrix);
#endif