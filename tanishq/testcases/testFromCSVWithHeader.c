#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<stdio.h>
#include<stdlib.h>
int main()
{
    mlfw_mat_double *matrix;
    mlfw_row_vec_string *header;
    matrix=mlfw_mat_double_from_csv("IceCreamSales.csv",NULL,&header);
    if(matrix==NULL)
    {
        printf("Unable to create matrix\n");
        return 0;
    }
    dimension_t size=mlfw_row_vec_string_get_size(header);
    char *ptr;
    for(index_t i=0;i<size;i++)
    {
        mlfw_row_vec_string_get(header,i,&ptr);
        if(ptr!=NULL)
        {
            printf("%s ",ptr);
            free(ptr);
        }
    }
    printf("\n");
    dimension_t rows,columns;
    mlfw_mat_double_get_dimension(matrix,&rows,&columns);
    for(index_t e=0;e<rows;e++)
    {
        for(index_t f=0;f<columns;f++)
        {
            printf("%lf ",mlfw_mat_double_get(matrix,e,f));
        }
        printf("\n");
    }
    mlfw_mat_double_destroy(matrix);
    mlfw_row_vec_string_destroy(header);
    return 0;
}