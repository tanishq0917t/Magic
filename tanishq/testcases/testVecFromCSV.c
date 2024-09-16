#include<mlfw_vector.h>
#include<stdio.h>
#include<stdlib.h>
int main()
{
    mlfw_row_vec_string *header;
    mlfw_row_vector_double *vector;
    vector=mlfw_row_vector_double_from_csv("double.csv",NULL,&header);
    if(header==NULL)
    {
        printf("Header is NULL\n");
        return 0;
    }
    if(vector==NULL)
    {
        printf("Unable to load file\n");
        mlfw_row_vec_string_destroy(header);
        return 0;
    }
    dimension_t size=mlfw_row_vec_string_get_size(header);
    char *ptr;
    for(index_t i=0;i<size;i++)
    {
        mlfw_row_vec_string_get(header,i,&ptr);
        if(ptr!=NULL)
        {
            printf("Index: %d, Header: %s\n",i,ptr);
            free(ptr);
        }
    }
    size=mlfw_row_vector_double_get_size(vector);
    for(index_t i=0;i<size;i++)
    {
        printf("%lf ",mlfw_row_vector_double_get(vector,i));
    }
    printf("\n");
    mlfw_row_vector_double_destroy(vector);
    mlfw_row_vec_string_destroy(header);
    mlfw_column_vector_double *cvector;
    cvector=mlfw_column_vector_double_from_csv("double.csv",NULL,&header);
    if(header==NULL)
    {
        printf("Header is NULL\n");
        return 0;
    }
    if(cvector==NULL)
    {
        printf("Unable to load file\n");
        mlfw_row_vec_string_destroy(header);
        return 0;
    }
    size=mlfw_row_vec_string_get_size(header);
    ptr=NULL;
    for(index_t i=0;i<size;i++)
    {
        mlfw_row_vec_string_get(header,i,&ptr);
        if(ptr!=NULL)
        {
            printf("Index: %d, Header: %s\n",i,ptr);
            free(ptr);
        }
    }
    size=mlfw_column_vector_double_get_size(cvector);
    for(index_t i=0;i<size;i++)
    {
        printf("%lf ",mlfw_column_vector_double_get(cvector,i));
    }
    printf("\n");
    mlfw_column_vector_double_destroy(cvector);
    mlfw_row_vec_string_destroy(header);   
    return 0;
}