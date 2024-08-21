#include<mlfw_set.h>
#include<stdio.h>
#include<stdlib.h>
int main()
{
    mlfw_set_string *set;
    set=mlfw_set_string_create_new();
    mlfw_set_string_add(set,"Good");
    mlfw_set_string_add(set,"Bad");
    mlfw_set_string_add(set,"Tanishq");
    mlfw_set_string_add(set,"Tanishq");
    dimension_t sz=mlfw_set_string_get_size(set);
    printf("Size: %u\n",sz);
    char *tmp;
    for(index_t i=0;i<sz;i++)
    {
        mlfw_set_string_get(set,i,&tmp);
        if(tmp)
        {
            printf("%s\n",tmp);
            free(tmp);
        }
    }
    mlfw_set_string_destroy(set);
    return 0;
}