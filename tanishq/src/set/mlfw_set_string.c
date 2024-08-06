#include<mlfw_set.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct __mlfw_set_string
{
    char **data;
    dimension_t size;
}mlfw_set_string;
mlfw_set_string * mlfw_set_string_create_new()
{
    mlfw_set_string *set;
    set=(mlfw_set_string *)malloc(sizeof(mlfw_set_string));
    if(set==NULL) return NULL;
    set->data=NULL;
    set->size=0;
    return set;
}
void mlfw_set_string_destroy(mlfw_set_string *set)
{
    if(set==NULL) return;
    for(index_t i=0;i<set->size;i++)
    {
        free(set->data[i]);
    }
    free(set->data);
    free(set);
}

void mlfw_set_string_get(mlfw_set_string *set,index_t i,char **string)
{
    if(set==NULL) return;
    if(string==NULL) return;
    if(i<0 || i>=set->size)
    {
        *string=NULL;
        return;
    }
    *string=(char *)malloc(sizeof(char)*(strlen(set->data[i])+1));
    if(*string) strcpy(*string,set->data[i]);
}
int mlfw_set_string_add(mlfw_set_string *set,char *string)
{
    if(set==NULL || string==NULL) return -1;
    if(set->size==0)
    {
        set->data=(char **)malloc(sizeof(char *)*1);
        if(set->data==NULL) return -1;
        set->data[0]=(char *)malloc(sizeof(char)*strlen(string)+1);
        if(set->data[0]==NULL)
        {
            free(set->data);
            set->data=NULL;
            return -1;
        }
        strcpy(set->data[0],string);
        set->size++;
        return 0;
    }
    else
    {
        for(index_t i=0;i<set->size;i++)
        {
            if(strcmp(set->data[i],string)==0) return 0;
        }
        char *str;
        str=(char *)malloc(sizeof(char)*strlen(string)+1);
        if(str==NULL) return -1;
        strcpy(str,string);
        char **tmp;//for reallocation memory for data array in set;
        tmp=(char **)realloc(set->data,sizeof(char **)*(set->size+1));
        if(tmp==NULL)
        {
            free(str);
            return -1;
        }
        set->data=tmp;
        set->data[set->size]=str;
        set->size++;
        return 0;
    }
}
dimension_t mlfw_set_string_get_size(mlfw_set_string *set)
{
    if(set==NULL) return 0;
    return set->size;
}
