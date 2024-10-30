#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<stdio.h>
#include<stdlib.h>
int main()
{
    mlfw_column_vec_string * string_vec;
    mlfw_row_vec_string *header;
    string_vec=mlfw_column_vec_string_from_csv("string.csv",NULL,&header);
    int header_size=mlfw_row_vec_string_get_size(header);
    int vector_size=mlfw_column_vec_string_get_size(string_vec);
    printf("Header Size: %d\n",header_size);
    printf("Vector Size: %d\n",vector_size);
    char *ptr;
    for(int e=0;e<header_size;e++)
    {
        mlfw_row_vec_string_get(header,e,&ptr);
        if(ptr!=NULL)
        {
            printf("Index: %d Value: %s\n",e,ptr);
        }
    }
    printf("\n");
    return 0;
}