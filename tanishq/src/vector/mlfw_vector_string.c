#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<mlfw_vector.h>
typedef struct __mlfw_row_vec_string 
{
    dimension_t size;
    char **data;
}mlfw_row_vec_string;

typedef struct __mlfw_column_vec_string
{
    dimension_t size;
    char **data;
}mlfw_column_vec_string;

//row vector functions
mlfw_row_vec_string *mlfw_row_vec_string_create_new(dimension_t columns)
{
    if(columns<=0) return NULL;
    mlfw_row_vec_string *vector;
    vector=(mlfw_row_vec_string *)malloc(sizeof(mlfw_row_vec_string));
    if(vector==NULL) return NULL;
    vector->data=(char **)malloc(sizeof(char *)*columns);
    if(vector->data==NULL)
    {
        free(vector);
        return NULL;
    }
    for(index_t c=0;c<columns;c++) vector->data[c]=NULL;
    vector->size=columns;
    return vector;
}
void mlfw_row_vec_string_destroy(mlfw_row_vec_string *vector)
{
    if(vector==NULL) return;
    for(index_t c=0;c<vector->size;c++)
    {
        if(vector->data[c]!=NULL) free(vector->data[c]);
    }
    free(vector->data);
    free(vector);
}
mlfw_row_vec_string *mlfw_row_vec_string_from_csv(const char *csv_file_name)
{
    if(csv_file_name==NULL) return NULL;
    int index;
    index_t c;
    char m;
    char string[5001];
    FILE *file;
    dimension_t columns;
    file=fopen(csv_file_name,"r");
    if(file==NULL) return NULL;
    columns=0;
    mlfw_row_vec_string *vector;
    while(1)
    {
        m=fgetc(file);
        if(feof(file)) break;
        if(m==',' || m=='\n') columns++;
    }
    printf("Columns: %u\n",columns);
    vector=mlfw_row_vec_string_create_new(columns);
    if(vector==NULL)
    {
        fclose(file);
        return NULL;
    }
    rewind(file);
    c=0;
    while(1)
    {
        m=fgetc(file);
        if(feof(file)) break;
        if(m==',' || m=='\n')
        {
            string[index]='\0';
            vector->data[c]=(char *)malloc(sizeof(char)*(strlen(string)+1));
            if(vector->data[c]!=NULL) strcpy(vector->data[c],string);
            index=0;
            c++;
        }
        else
        {
            string[index]=m;
            index++;
        }
    }
    fclose(file);
    return vector;
}
void mlfw_row_vec_string_to_csv(mlfw_row_vec_string *vector,const char *csv_file_name)
{
    if(csv_file_name==NULL || vector==NULL) return;
    FILE *file;
    file=fopen(csv_file_name,"w");
    if(file==NULL) return;
    for(index_t c=0;c<vector->size;c++)
    {
        if(vector->data[c]!=NULL) fputs(vector->data[c],file);
        if(c<vector->size-1) fputc(',',file);
        else fputc('\n',file);
    }
    fclose(file);
}
void mlfw_row_vec_string_get(mlfw_row_vec_string *vector,index_t index,char **string)
{
    if(string==NULL) return;
    if(vector==NULL)
    {
        *string=NULL;
        return;
    }
    if(index<0 || index>=vector->size)
    {
        *string=NULL;
        return;
    }
    if(vector->data[index]==NULL)
    {
        *string=NULL;
        return;
    }
    *string=(char *)malloc(sizeof(char)*(strlen(vector->data[index])+1));
    if(*string==NULL) return;
    strcpy(*string,vector->data[index]);
}
void mlfw_row_vec_string_set(mlfw_row_vec_string *vector,index_t index,char *string)
{
    if(vector==NULL || string==NULL) return;
    if(index<0 || index>=vector->size) return;
    if(vector->data[index]!=NULL) free(vector->data[index]);
    vector->data[index]=(char *)malloc(sizeof(char)*(strlen(string)+1));
    if(vector->data[index]!=NULL)
    {
        strcpy(vector->data[index],string);
    }
}
dimension_t mlfw_row_vec_string_get_size(mlfw_row_vec_string *vector)
{
    if(vector==NULL) return 0;
    return vector->size;
}
mlfw_column_vec_string * mlfw_row_vec_string_transpose(mlfw_row_vec_string *vector)
{
    char *ptr;
    if(vector==NULL) return NULL;
    mlfw_column_vec_string *transposed_vector;
    transposed_vector=mlfw_column_vec_string_create_new(vector->size);
    if(transposed_vector==NULL) return NULL;
    for(index_t c=0;c<vector->size;c++)
    {
        mlfw_row_vec_string_get(vector,c,&ptr);
        mlfw_column_vec_string_set(transposed_vector,c,ptr);
	free(ptr);
    }
    return transposed_vector;
}

//column vector functions
mlfw_column_vec_string *mlfw_column_vec_string_create_new(dimension_t columns)
{
    if(columns<=0) return NULL;
    mlfw_column_vec_string *vector;
    vector=(mlfw_column_vec_string *)malloc(sizeof(mlfw_column_vec_string));
    if(vector==NULL) return NULL;
    vector->data=(char **)malloc(sizeof(char *)*columns);
    if(vector->data==NULL)
    {
        free(vector);
        return NULL;
    }
    for(index_t c=0;c<columns;c++) vector->data[c]=NULL;
    vector->size=columns;
    return vector;
}
void mlfw_column_vec_string_destroy(mlfw_column_vec_string *vector)
{
    if(vector==NULL) return;
    for(index_t c=0;c<vector->size;c++)
    {
        if(vector->data[c]!=NULL) free(vector->data[c]);
    }
    free(vector->data);
    free(vector);
}
mlfw_column_vec_string *mlfw_column_vec_string_from_csv(const char *csv_file_name)
{
    if(csv_file_name==NULL) return NULL;
    int index;
    index_t c;
    char m;
    char string[5001];
    FILE *file;
    dimension_t columns;
    file=fopen(csv_file_name,"r");
    if(file==NULL) return NULL;
    columns=0;
    mlfw_column_vec_string *vector;
    while(1)
    {
        m=fgetc(file);
        if(feof(file)) break;
        if(m==',' || m=='\n') columns++;
    }
    vector=mlfw_column_vec_string_create_new(columns);
    if(vector==NULL)
    {
        fclose(file);
        return NULL;
    }
    rewind(file);
    c=0;
    while(1)
    {
        m=fgetc(file);
        if(feof(file)) break;
        if(m==',' || m=='\n')
        {
            string[index]='\0';
            vector->data[c]=(char *)malloc(sizeof(char)*(strlen(string)+1));
            if(vector->data[c]!=NULL) strcpy(vector->data[c],string);
            index=0;
            c++;
        }
        else
        {
            string[index]=m;
            index++;
        }
    }
    fclose(file);
    return vector;
}
void mlfw_column_vec_string_to_csv(mlfw_column_vec_string *vector,const char *csv_file_name)
{
    if(csv_file_name==NULL || vector==NULL) return;
    FILE *file;
    file=fopen(csv_file_name,"w");
    if(file==NULL) return;
    for(index_t c=0;c<vector->size;c++)
    {
        if(vector->data[c]!=NULL) fputs(vector->data[c],file);
        if(c<vector->size-1) fputc(',',file);
        else fputc('\n',file);
    }
    fclose(file);
}
void mlfw_column_vec_string_get(mlfw_column_vec_string *vector,index_t index,char **string)
{
    if(string==NULL) return;
    if(vector==NULL)
    {
        *string=NULL;
        return;
    }
    if(index<0 || index>=vector->size)
    {
        *string=NULL;
        return;
    }
    if(vector->data[index]==NULL)
    {
        *string=NULL;
        return;
    }
    *string=(char *)malloc(sizeof(char)*(strlen(vector->data[index])+1));
    if(*string==NULL) return;
    strcpy(*string,vector->data[index]);
}
void mlfw_column_vec_string_set(mlfw_column_vec_string *vector,index_t index,char *string)
{
    if(vector==NULL || string==NULL) return;
    if(index<0 || index>=vector->size) return;
    if(vector->data[index]!=NULL) free(vector->data[index]);
    vector->data[index]=(char *)malloc(sizeof(char)*(strlen(string)+1));
    if(vector->data[index]!=NULL)
    {
        strcpy(vector->data[index],string);
    }
}
dimension_t mlfw_column_vec_string_get_size(mlfw_column_vec_string *vector)
{
    if(vector==NULL) return 0;
    return vector->size;
}
mlfw_row_vec_string * mlfw_column_vec_string_transpose(mlfw_column_vec_string *vector)
{
    char *ptr;
    if(vector==NULL) return NULL;
    mlfw_row_vec_string *transposed_vector;
    transposed_vector=mlfw_row_vec_string_create_new(vector->size);
    if(transposed_vector==NULL) return NULL;
    for(index_t c=0;c<vector->size;c++)
    {
        mlfw_column_vec_string_get(vector,c,&ptr);
        mlfw_row_vec_string_set(transposed_vector,c,ptr);
	free(ptr);
    }
    return transposed_vector;
}