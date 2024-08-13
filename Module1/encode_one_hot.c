#include<mlfw_encoder.h>
#include<stdio.h>
#include<stdlib.h>
int main(int argc,char *argv[])
{
    int *encoded_columns;
    char *column_string;
    char *s,*e;
    dimension_t size=0;
    char *output_file,*input_file, has_header;
    int header_exists;
    if(argc!=5)
    {
        printf("Usage: [encode_one_hot.out input_file output_file columns_to_encode has_header]\n");
        return 0;
    }
    input_file=argv[1];
    output_file=argv[2];
    column_string=argv[3];
    has_header=argv[4][0];
    if(has_header=='Y' || has_header=='y') header_exists=1;
    else header_exists=0;
    size=0;
    for(index_t i=0;column_string[i]!='\0';i++)
    {
        if(column_string[i]==',') size++;
    }
    size++;
    encoded_columns=(int *)malloc(sizeof(int)*size);
    if(encoded_columns==NULL)
    {
        printf("Low memory\n");
        return 0;
    }
    index_t i=0;
    s=column_string;
    while(1)
    {
        e=s+1;
        while(*e!='\0' && *e!=',') e++;
        if(*e=='\0')
        {
            encoded_columns[i]=atoi(s);
            break;
        }
        else
        {
            *e='\0';
            encoded_columns[i]=atoi(s);
            i++;
            s=e+1;
        }
    }
    printf("%u\n",size);
    for(int e=0;e<size;e++) printf("%d\n",encoded_columns[e]);
    mlfw_encoder_encode_one_hot(input_file,output_file,encoded_columns,size,header_exists);
    free(encoded_columns);
    return 0;
}
