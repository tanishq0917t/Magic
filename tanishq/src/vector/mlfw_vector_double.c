#include<stdio.h>
#include<stdlib.h>
#include<mlfw_vector.h>
typedef struct __mlfw_row_vector_double
{
    double *data;
    dimension_t size;
} mlfw_row_vector_double;
typedef struct __mlfw_column_vector_double
{
    double *data;
    dimension_t size;   
} mlfw_column_vector_double;


//Column vector functions
mlfw_column_vector_double * mlfw_column_vector_double_create_new(dimension_t size)
{
    if(size<=0) return NULL;
    mlfw_column_vector_double *vector;
    vector=(mlfw_column_vector_double *)malloc(sizeof(mlfw_column_vector_double));
    if(vector==NULL) return NULL;
    vector->data=(double *)malloc(sizeof(double)*size);
    if(vector->data==NULL) 
    {
        free(vector);
        return NULL;
    }
    vector->size=size;
    return vector;
}
void mlfw_column_vector_double_destroy(mlfw_column_vector_double *vector)
{
    if(vector==NULL) return;
    free(vector->data);
    free(vector);
}
double mlfw_column_vector_double_get(mlfw_column_vector_double *vector,index_t index)
{
    if(vector==NULL) return 0.0;
    if(index<0 || index>=vector->size) return 0.0;
    return vector->data[index];
}
void mlfw_column_vector_double_set(mlfw_column_vector_double *vector,index_t index,double value)
{
    if(vector==NULL) return;
    if(index<0 || index>=vector->size) return;
    vector->data[index]=value;
}
mlfw_column_vector_double * mlfw_column_vector_create_new_filled(dimension_t size,double value,mlfw_column_vector_double *vector)
{
    if(size<=0) return NULL;
    if(vector==NULL)
    {
        vector=mlfw_column_vector_double_create_new(size);
        if(vector==NULL) return NULL;
    }
    else
    {
        if(vector->size!=size) return NULL;
    }
    for(index_t i=0;i<size;i++) vector->data[i]=value;
    return vector;
}
mlfw_row_vector_double * mlfw_column_vector_double_transpose(mlfw_column_vector_double *vector,mlfw_row_vector_double *transposed_vector)
{
    if(vector==NULL) return NULL;
    if(transposed_vector==NULL)
    {
        transposed_vector=mlfw_row_vector_double_create_new(vector->size);
        if(transposed_vector==NULL) return NULL;
    }
    else
    {
        if(transposed_vector->size!=vector->size) return NULL;
    }
    for(index_t i=0;i<vector->size;i++) transposed_vector->data[i]=vector->data[i];
    return transposed_vector;
}
dimension_t mlfw_column_vector_double_get_size(mlfw_column_vector_double *vector)
{
    if(vector==NULL) return 0;
    return vector->size;
}
double mlfw_column_vector_double_mean(mlfw_column_vector_double *vector)
{
    double sum=0.0;
    for(index_t i=0;i<vector->size;i++) sum+=vector->data[i];
    return sum/(double)vector->size;
}
void mlfw_column_vector_double_to_csv(mlfw_column_vector_double *vector,char *CSV_FILE_NAME)
{
    FILE *file;
    if(vector==NULL || CSV_FILE_NAME==NULL) return;
    file=fopen(CSV_FILE_NAME,"w");
    if(file==NULL) return;
    for(index_t i=0;i<vector->size;i++)
    {
        fprintf(file,"%lf",vector->data[i]);
        if(i==vector->size-1) fputc('\n',file);
        else fputc(',',file);
    }
    fclose(file);
}
mlfw_column_vector_double * mlfw_column_vector_double_from_csv(char *CSV_FILE_NAME,mlfw_column_vector_double *vector,mlfw_row_vec_string **header)
{
    if(CSV_FILE_NAME==NULL) return NULL;
    FILE *file;
    dimension_t size;
    dimension_t columns=0,index;
    char m,doubleString[1025];
    index_t i,j;
    double value;
    char header_string[1025];
    index_t header_index;

    file=fopen(CSV_FILE_NAME,"r");
    if(file==NULL) return NULL;
    //header logic
    columns=0;
    while(1)
    {
        m=fgetc(file);
        if(feof(file)) break;
        if(m=='\r') continue;
        if(m==',') columns++;
        if(m=='\n') break;
    }
    columns++;
    //Logic to read first line ends here
    *header=mlfw_row_vec_string_create_new(columns);
    if(*header==NULL) return NULL;
    rewind(file);
    index=0;
    header_index=0;
    while(1) //logic to prepare header
    {
        m=fgetc(file);
        if(feof(file)) break;
        if(m=='\r') continue;
        if(m==',')
        {
            header_string[index]='\0';
            mlfw_row_vec_string_set(*header,header_index,header_string);
            header_index++;
            index=0;
            continue;
        }
        if(m=='\n')
        {
            header_string[index]='\0';
            mlfw_row_vec_string_set(*header,header_index,header_string);
            break;
        }
        header_string[index]=m;
        index++;
    }



    size=0;
    while(1)
    {
        m=fgetc(file);
        if(feof(file)) break;
        if(m==',') size++;
    }
    size++;
    if(vector==NULL)
    {
        vector=mlfw_column_vector_double_create_new(size);
        if(vector==NULL)
        {
            fclose(file);
            mlfw_row_vec_string_destroy(*header);
            header=NULL;
            return NULL;
        }
    }
    else
    {
        if(vector->size!=size) 
        {
            mlfw_row_vec_string_destroy(*header);
            header=NULL;
            return NULL;
        }
    }
    rewind(file);
    //logic to skip the header
    while(1)
    {
        m=fgetc(file);
        if(feof(file)) break;
        if(m=='\r') continue;
        if(m=='\n') break;
    }
    index=0;
    i=0;
    j=0;
    while(1)
    {
        m=fgetc(file);
        if(feof(file)) break;
        if(m==',' || m=='\n')
        {
            doubleString[i]='\0';
            value=strtod(doubleString,NULL);
            mlfw_column_vector_double_set(vector,j,value);
            i=0;
            j++;
        }
        else
        {
            doubleString[i]=m;
            i++;
        }
    }
    fclose(file);
    return vector;
}


//Row vector functions
mlfw_row_vector_double * mlfw_row_vector_double_create_new(dimension_t size)
{
    if(size<=0) return NULL;
    mlfw_row_vector_double *vector;
    vector=(mlfw_row_vector_double *)malloc(sizeof(mlfw_row_vector_double));
    if(vector==NULL) return NULL;
    vector->data=(double *)malloc(sizeof(double)*size);
    if(vector->data==NULL) 
    {
        free(vector);
        return NULL;
    }
    vector->size=size;
    return vector;
}
void mlfw_row_vector_double_destroy(mlfw_row_vector_double *vector)
{
    if(vector==NULL) return;
    free(vector->data);
    free(vector);
}
double mlfw_row_vector_double_get(mlfw_row_vector_double *vector,index_t index)
{
    if(vector==NULL) return 0.0;
    if(index<0 || index>=vector->size) return 0.0;
    return vector->data[index];
}
void mlfw_row_vector_double_set(mlfw_row_vector_double *vector,index_t index,double value)
{
    if(vector==NULL) return;
    if(index<0 || index>=vector->size) return;
    vector->data[index]=value;
}
mlfw_row_vector_double * mlfw_row_vector_create_new_filled(dimension_t size,double value,mlfw_row_vector_double *vector)
{
    if(size<=0) return NULL;
    if(vector==NULL)
    {
        vector=mlfw_row_vector_double_create_new(size);
        if(vector==NULL) return NULL;
    }
    else
    {
        if(vector->size!=size) return NULL;
    }
    for(index_t i=0;i<size;i++) vector->data[i]=value;
    return vector;
}
mlfw_column_vector_double * mlfw_row_vector_double_transpose(mlfw_row_vector_double *vector,mlfw_column_vector_double *transposed_vector)
{
    if(vector==NULL) return NULL;
    if(transposed_vector==NULL)
    {
        transposed_vector=mlfw_column_vector_double_create_new(vector->size);
        if(transposed_vector==NULL) return NULL;
    }
    else
    {
        if(transposed_vector->size!=vector->size) return NULL;
    }
    for(index_t i=0;i<vector->size;i++) transposed_vector->data[i]=vector->data[i];
    return transposed_vector;
}
dimension_t mlfw_row_vector_double_get_size(mlfw_row_vector_double *vector)
{
    if(vector==NULL) return 0;
    return vector->size;
}

double mlfw_row_vector_double_mean(mlfw_row_vector_double *vector)
{
    double sum=0.0;
    for(index_t i=0;i<vector->size;i++) sum+=vector->data[i];
    return sum/(double)vector->size;
}
void mlfw_row_vector_double_to_csv(mlfw_row_vector_double *vector,char *CSV_FILE_NAME)
{
    FILE *file;
    if(vector==NULL || CSV_FILE_NAME==NULL) return;
    file=fopen(CSV_FILE_NAME,"w");
    if(file==NULL) return;
    for(index_t i=0;i<vector->size;i++)
    {
        fprintf(file,"%lf",vector->data[i]);
        if(i==vector->size-1) fputc('\n',file);
        else fputc(',',file);
    }
    fclose(file);
}
mlfw_row_vector_double * mlfw_row_vector_double_from_csv(char *CSV_FILE_NAME,mlfw_row_vector_double *vector,mlfw_row_vec_string **header)
{
    if(CSV_FILE_NAME==NULL) return NULL;
    FILE *file;
    dimension_t size;
    char m,doubleString[1025];
    index_t i,j;
    double value;
    char header_string[1025];
    index_t header_index;
    dimension_t index,columns=0;

    file=fopen(CSV_FILE_NAME,"r");
    if(file==NULL) return NULL;
    //header logic
    columns=0;
    while(1)
    {
        m=fgetc(file);
        if(feof(file)) break;
        if(m=='\r') continue;
        if(m==',') columns++;
        if(m=='\n') break;
    }
    columns++;
    //Logic to read first line ends here
    *header=mlfw_row_vec_string_create_new(columns);
    if(*header==NULL) return NULL;
    rewind(file);
    index=0;
    header_index=0;
    while(1) //logic to prepare header
    {
        m=fgetc(file);
        if(feof(file)) break;
        if(m=='\r') continue;
        if(m==',')
        {
            header_string[index]='\0';
            mlfw_row_vec_string_set(*header,header_index,header_string);
            header_index++;
            index=0;
            continue;
        }
        if(m=='\n')
        {
            header_string[index]='\0';
            mlfw_row_vec_string_set(*header,header_index,header_string);
            break;
        }
        header_string[index]=m;
        index++;
    }

    size=0;
    while(1)
    {
        m=fgetc(file);
        if(feof(file)) break;
        if(m==',') size++;
    }
    size++;
    if(vector==NULL)
    {
        vector=mlfw_row_vector_double_create_new(size);
        if(vector==NULL)
        {
            fclose(file);
            mlfw_row_vec_string_destroy(*header);
            header=NULL;
            return NULL;
        }
    }
    else
    {
        if(vector->size!=size) 
        {
            mlfw_row_vec_string_destroy(*header);
            header=NULL;
            return NULL;
        }
    }
    rewind(file);
    //logic to skip the header
    while(1)
    {
        m=fgetc(file);
        if(feof(file)) break;
        if(m=='\r') continue;
        if(m=='\n') break;
    }
    index=0;
    i=0;
    j=0;
    while(1)
    {
        m=fgetc(file);
        if(feof(file)) break;
        if(m==',' || m=='\n')
        {
            doubleString[i]='\0';
            value=strtod(doubleString,NULL);
            mlfw_row_vector_double_set(vector,j,value);
            i=0;
            j++;
        }
        else
        {
            doubleString[i]=m;
            i++;
        }
    }
    fclose(file);
    return vector;
}