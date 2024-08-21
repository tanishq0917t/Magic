#include<mlfw_matrix.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
typedef struct __mlfw_mat_string
{
    char ***data;
    dimension_t rows;
    dimension_t columns;
}mlfw_mat_string;

mlfw_mat_string * mlfw_mat_string_create_new(dimension_t rows,dimension_t columns)
{
    mlfw_mat_string *matrix;
    index_t r,k;
    if(rows<0 || columns<0) return NULL;
    matrix=(mlfw_mat_string *)malloc(sizeof(mlfw_mat_string));
    if(matrix==NULL) return NULL;
    matrix->data=(char ***)malloc(sizeof(char **)*rows);
    if(matrix->data==NULL)
    {
        free(matrix);
        return NULL;
    }
    for(r=0;r<rows;r++)
    {
        matrix->data[r]=(char **)malloc(sizeof(char *)*columns);
        if(matrix->data[r]==NULL)
        {
            for(k=0;k<r;k++) free(matrix->data[k]);
            free(matrix->data);
            free(matrix);
        }
        else //if pointers are allocated, so they should not have garbage values in them, so setting with NULL
        {
            for(index_t c=0;c<columns;c++) matrix->data[r][c]=NULL;
        }
    }
    matrix->rows=rows;
    matrix->columns=columns;
    return matrix;
}
void mlfw_mat_string_destroy(mlfw_mat_string *matrix)
{
    if(!matrix) return;
    index_t r,c;
    for(r=0;r<matrix->rows;r++)
    {
        for(c=0;c<matrix->columns;c++)
        {
            if(matrix->data[r][c]!=NULL) free(matrix->data[r][c]);
        }
        free(matrix->data[r]);
    }
    free(matrix->data);
    free(matrix);
}
mlfw_mat_string * mlfw_mat_string_from_csv(const char *csv_file_name)
{
    FILE *file; //to open file
    if(csv_file_name==NULL) return NULL;
    file=fopen(csv_file_name,"r");
    if(!file) return NULL;
    mlfw_mat_string *matrix; //to create new matrix
    char m; //to reach each character of file
    dimension_t rows=0,columns=0;
    index_t r=0,c=0;
    char string[5001];
    int index=0;
    while(1) //counting number of rows and columns
    {
        m=fgetc(file);
        if(feof(file)) break;
        if(rows==0 && m==',') columns++;
        if(m=='\n') rows++;
    }
    columns++; // a,b,c means 2 commas and 2+1 columns
    // printf("Rows: %" PRIu32 " Columns: %" PRIu32" \n",rows,columns);
    matrix=mlfw_mat_string_create_new(rows,columns);
    if(matrix==NULL)
    {
        fclose(file);
        return NULL;
    }
    rewind(file); //move the internal pointer to first byte of file
    
    while(1) //populating matrix
    {
        m=fgetc(file);
        if(feof(file)) break;
        if(m==',' || m=='\n')
        {
            string[index]='\0';
            index=0;
            matrix->data[r][c]=(char *)malloc(sizeof(char)*strlen(string)+1);
            if(matrix->data[r][c])
            {
                strcpy(matrix->data[r][c],string);
            }
            c++;
            if(c==matrix->columns)
            {
                r++;
                c=0;
            }
        }
        else
        {
            string[index]=m;
            index++;
        }
    }
    fclose(file);
    return matrix;
}

void mlfw_mat_string_get(mlfw_mat_string *matrix,index_t row,index_t column,char **string)
{
    if(string==NULL) return;
    if(matrix==NULL)
    {
        *string=NULL;
        return;
    }
    if(row<0 && row>=matrix->rows)
    if(matrix==NULL)
    {
        *string=NULL;
        return;
    }
    if(column<0 && row>=matrix->columns)
    if(matrix==NULL)
    {
        *string=NULL;
        return;
    }
    if(matrix->data[row][column]==NULL) 
    {
        *string=NULL;
        return;
    }
    *string=(char *)malloc(sizeof(char)*strlen(matrix->data[row][column])+1);
    if(*string==NULL) return;
    strcpy(*string,matrix->data[row][column]);
}
void mlfw_mat_string_set(mlfw_mat_string *matrix,index_t row,index_t column,char * string)
{
    if(matrix==NULL) return;
    if(row<0 && row>=matrix->rows) return;
    if(column<0 && row>=matrix->columns) return;
    //re-writing a cell
    if(matrix->data[row][column]!=NULL) free(matrix->data[row][column]);
    matrix->data[row][column]=(char *)malloc(sizeof(char)*(strlen(string)+1));
    if(matrix->data[row][column]!=NULL) strcpy(matrix->data[row][column],string);
}

void mlfw_mat_string_get_dimension(mlfw_mat_string *matrix,dimension_t *rows,dimension_t *columns)
{
    if(matrix==NULL) return;
    if(rows) *rows=matrix->rows;
    if(columns) *columns=matrix->columns;
}

void mlfw_mat_string_to_csv(mlfw_mat_string *matrix,const char *csv_file_name)
{
    if(matrix==NULL || csv_file_name==NULL) return;
    FILE *file;
    file=fopen(csv_file_name,"w");
    if(file==NULL) return;
    for(index_t r=0;r<matrix->rows;r++)
    {
        for(index_t c=0;c<matrix->columns;c++)
        {
            fputs(matrix->data[r][c],file);
            if(c<matrix->columns-1) fputc(',',file);
            else fputc('\n',file);
        }
    }
    fclose(file);
}