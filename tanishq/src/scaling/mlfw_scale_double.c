#include<mlfw_scale.h>
#include<mlfw_matrix.h>
#include<stdio.h>
#include<stdlib.h>

mlfw_mat_double *mlfw_scale_double_min_max(mlfw_mat_double *matrix,index_t start_r,index_t start_c,index_t end_r,index_t end_c,char *min_max_file)
{
    if(matrix==NULL) return NULL;
    FILE *file;
    dimension_t matrix_rows,matrix_columns;
    dimension_t new_matrix_rows,new_matrix_columns;
    mlfw_mat_double *new_matrix;
    mlfw_mat_double_get_dimension(matrix,&matrix_rows,&matrix_columns);
    double value,scaled_value;

    if(start_r<0) return NULL;
    if(start_c<0) return NULL;
    if (end_r>=matrix_rows) return NULL;
    if (end_c>=matrix_columns) return NULL;
    if (start_r>end_r) return NULL;
    if (start_c>end_c) return NULL;
    new_matrix_columns=end_c-start_c+1;
    new_matrix_rows=end_r-start_r+1;
    new_matrix=mlfw_mat_double_create_new(new_matrix_rows,new_matrix_columns);
    if(new_matrix==NULL) return NULL;
    double *min,*max;
    max=(double *)malloc(sizeof(double)*new_matrix_columns);
    if(max==NULL) return NULL;
    min=(double *)malloc(sizeof(double)*new_matrix_columns);
    if(min==NULL)
    {
        free(max);
        return NULL;
    }
    for(index_t c=start_c,i=0;c<=end_c;c++,i++)
    {
        min[i]=mlfw_mat_double_get_min(matrix,start_r,c,end_r,c);
        max[i]=mlfw_mat_double_get_max(matrix,start_r,c,end_r,c);
    }
    for(index_t r=start_r,new_r=0;new_r<new_matrix_rows;new_r++,r++)
    {
        for(index_t c=start_c,new_c=0;new_c<new_matrix_columns;new_c++,c++)
        {
            value=mlfw_mat_double_get(matrix,r,c);
            scaled_value=(value-min[new_c])/(max[new_c]-min[new_c]);
            mlfw_mat_double_set(new_matrix,new_r,new_c,scaled_value);
        }
    }
    file=fopen(min_max_file,"w");
    if(file==NULL)
    {
        free(min);
        free(max);
        mlfw_mat_double_destroy(new_matrix);
        return NULL;
    }
    for(index_t i=0;i<new_matrix_columns;i++)
    {
        fprintf(file,"%lf",min[i]);
        if(i==new_matrix_columns-1) fputc('\n',file);
        else fputc(',',file);
    }
    for(index_t i=0;i<new_matrix_columns;i++)
    {
        fprintf(file,"%lf",max[i]);
        if(i==new_matrix_columns-1) fputc('\n',file);
        else fputc(',',file);
    }
    fclose(file);
    free(max);
    free(min);
    return new_matrix;
}



mlfw_mat_double *mlfw_scale_double_min_max_with_given_file(mlfw_mat_double *matrix,index_t start_r,index_t start_c,index_t end_r,index_t end_c,mlfw_mat_double *min_max_matrix)
{
    if(matrix==NULL || min_max_matrix==NULL) return NULL;
    dimension_t matrix_rows,matrix_columns;
    dimension_t new_matrix_rows,new_matrix_columns;
    dimension_t min_max_rows,min_max_columns;
    mlfw_mat_double *new_matrix;
    mlfw_mat_double_get_dimension(matrix,&matrix_rows,&matrix_columns);
    mlfw_mat_double_get_dimension(min_max_matrix,&min_max_rows,&min_max_columns);
    if(min_max_rows!=2) return NULL;
    double value,scaled_value;
    double min,max;

    if(start_r<0) return NULL;
    if(start_c<0) return NULL;
    if (end_r>=matrix_rows) return NULL;
    if (end_c>=matrix_columns) return NULL;
    if (start_r>end_r) return NULL;
    if (start_c>end_c) return NULL;
    new_matrix_columns=end_c-start_c+1;
    if(min_max_columns!=new_matrix_columns) return NULL;
    new_matrix_rows=end_r-start_r+1;
    new_matrix=mlfw_mat_double_create_new(new_matrix_rows,new_matrix_columns);
    if(new_matrix==NULL) return NULL;
    
    for(index_t r=start_r,new_r=0;new_r<new_matrix_rows;new_r++,r++)
    {
        for(index_t c=start_c,new_c=0;new_c<new_matrix_columns;new_c++,c++)
        {
            min=mlfw_mat_double_get(min_max_matrix,0,new_c);
            max=mlfw_mat_double_get(min_max_matrix,1,new_c);
            value=mlfw_mat_double_get(matrix,r,c);
            scaled_value=(value-min)/(max-min);
            mlfw_mat_double_set(new_matrix,new_r,new_c,scaled_value);
        }
    }
    return new_matrix;   
}