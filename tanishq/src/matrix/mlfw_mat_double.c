#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<stdlib.h>
#include<stdio.h>
typedef struct __mlfw_mat_double
{
    double **data;
    dimension_t rows;
    dimension_t columns;
}mlfw_mat_double;
mlfw_mat_double * mlfw_mat_double_create_new(dimension_t rows,dimension_t columns)
{
    mlfw_mat_double *matrix;
    index_t r,k;
    if(rows<0 || columns<0) return NULL;
    matrix=(mlfw_mat_double *)malloc(sizeof(mlfw_mat_double));
    if(matrix==NULL) return NULL;
    matrix->data=(double **)malloc(sizeof(double *)*rows);
    if(matrix->data==NULL)
    {
        free(matrix);
        return NULL;
    }
    for(r=0;r<rows;r++)
    {
        matrix->data[r]=(double *)malloc(sizeof(double)*columns);
        if(matrix->data[r]==NULL)
        {
            for(k=0;k<r;k++) free(matrix->data[k]);
            free(matrix->data);
            free(matrix);
        }
    }
    matrix->rows=rows;
    matrix->columns=columns;
    return matrix;
}
void mlfw_mat_double_destroy(mlfw_mat_double *matrix)
{
    if(!matrix) return;
    index_t r;
    for(r=0;r<matrix->rows;r++) free(matrix->data[r]);
    free(matrix->data);
    free(matrix);
}
mlfw_mat_double * mlfw_mat_double_from_csv(const char *csv_file_name)
{
    FILE *file; //to open file
    if(csv_file_name==NULL) return NULL;
    file=fopen(csv_file_name,"r");
    if(!file) return NULL;
    mlfw_mat_double *matrix; //to create new matrix
    char m; //to reach each character of file
    dimension_t rows=0,columns=0;
    while(1) //counting number of rows and columns
    {
        m=fgetc(file);
        if(feof(file)) break;
        if(rows==0 && m==',') columns++;
        if(m=='\n') rows++;
    }
    columns++; // a,b,c means 2 commas and 2+1 columns
    matrix=mlfw_mat_double_create_new(rows,columns);
    if(matrix==NULL)
    {
        fclose(file);
        return NULL;
    }
    rewind(file); //move the internal pointer to first byte of file
    index_t r=0,c=0;
    char double_string[1025];
    int index=0;
    double value;
    while(1) //populating matrix
    {
        m=fgetc(file);
        if(feof(file)) break;
        if(m==',' || m=='\n')
        {
            double_string[index]='\0';
            value=strtod(double_string,NULL);
            index=0;
            matrix->data[r][c]=value;
            c++;
            if(c==matrix->columns)
            {
                r++;
                c=0;
            }
        }
        else
        {
            double_string[index]=m;
            index++;
        }
    }
    fclose(file);
    return matrix;
}

double mlfw_mat_double_get(mlfw_mat_double *matrix,index_t row,index_t column)
{
    double value=0.0;
    if(matrix==NULL) return value;
    if(row<0 && row>=matrix->rows) return value;
    if(column<0 && row>=matrix->columns) return value;
    return matrix->data[row][column];
}
void mlfw_mat_double_set(mlfw_mat_double *matrix,index_t row,index_t column,double value)
{
    if(matrix==NULL) return;
    if(row<0 && row>=matrix->rows) return;
    if(column<0 && row>=matrix->columns) return;
    matrix->data[row][column]=value;
}

void mlfw_mat_double_get_dimension(mlfw_mat_double *matrix,dimension_t *rows,dimension_t *columns)
{
    if(matrix==NULL) return;
    if(rows) *rows=matrix->rows;
    if(columns) *columns=matrix->columns;
}
void mlfw_mat_double_copy(mlfw_mat_double *target,mlfw_mat_double *source,index_t target_row_index,index_t target_column_index,index_t source_from_row_index,index_t source_from_column_index,index_t source_to_row_index,index_t source_to_column_index)
{
    if(target==NULL || source==NULL) return;
    if(target_row_index<0 || target_row_index>=target->rows) return;
    if(target_column_index<0 || target_column_index>=target->columns) return;
    if(source_from_row_index<0 || source_from_row_index>=source->rows) return;
    if(source_from_column_index<0 || source_from_column_index>=source->columns) return;
    if(source_from_row_index>source_to_row_index) return;
    if(source_from_column_index>source_to_column_index) return;
    index_t target_r,target_c;
    index_t source_r,source_c;
    target_r=target_row_index;
    source_r=source_from_row_index;
    while (source_r<=source_from_row_index)
    {
        target_c=target_column_index;
        source_c=source_from_column_index;
        while (source_c<=source_to_column_index)
        {
            if(target_r>=0 && target_r<target->rows && target_c>=0 && target_c<target->columns)
            {
                target->data[target_r][target_c]=source->data[source_r][source_c];
                target_r++;
                target_c++;
            }
            source_c++;
            target_c++;
        }
        
        source_r++;
        target_r++;
    }
    
}
void mlfw_mat_double_fill(mlfw_mat_double *matrix,index_t from_row_index,index_t from_column_index,index_t upto_row_index,index_t upto_column_index,index_t what_to_fill)
{
    if(matrix==NULL) return;
    if(from_row_index>=matrix->rows) return;
    if(from_column_index>=matrix->columns) return;
    if(from_row_index<0) from_row_index=0;
    if(from_column_index<0) from_column_index=0;
    if(upto_row_index>=matrix->rows) upto_row_index=matrix->rows-1;
    if(upto_column_index>=matrix->columns) upto_column_index=matrix->columns-1;
    if(from_row_index>upto_row_index) return;
    if(from_column_index>upto_column_index) return;
    for(index_t r=from_row_index;r<=upto_row_index;r++)
    {
        for(index_t c=from_column_index;c<=upto_column_index;c++)
        {
            matrix->data[r][c]=what_to_fill;
        }
    }
}

mlfw_column_vector_double * mlfw_mat_double_create_column_vec(mlfw_mat_double * matrix,index_t index)
{
    if(matrix==NULL) return NULL;
    if(index<0 || index>=matrix->columns) return NULL;
    mlfw_column_vector_double *vector;
    vector=mlfw_column_vector_double_create_new(matrix->rows);
    if(vector==NULL) return NULL;
    for(index_t r=0;r<matrix->rows;r++)
    {
        mlfw_column_vector_double_set(vector,r,matrix->data[r][index]);
    }
    return vector;
}
