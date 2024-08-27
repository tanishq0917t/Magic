#include<mlfw_vector.h>
#include<stdio.h>
#include<stdlib.h>
int main()
{
    dimension_t size;
    mlfw_row_vec_string *vector;
    vector=mlfw_row_vec_string_from_csv("a.csv");
    if(vector==NULL)
    {
        printf("unable to load a.csv\n");
        return 0;
    }
    size=mlfw_row_vec_string_get_size(vector);
    char *ptr;
    for(index_t c=0;c<size;c++)
    {
	    mlfw_row_vec_string_get(vector,c,&ptr);
	    if(ptr) printf("%s\n",ptr);
	    free(ptr);
    }
    mlfw_row_vec_string_to_csv(vector,"b.csv");
    mlfw_column_vec_string *col_vector;
    col_vector=mlfw_row_vec_string_transpose(vector);
    mlfw_column_vec_string_to_csv(col_vector,"c.csv");
    mlfw_row_vec_string_destroy(vector);
    mlfw_column_vec_string_destroy(col_vector);
    return 0;
}
