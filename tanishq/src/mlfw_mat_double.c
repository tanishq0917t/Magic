#include<mlfw_matrix.h>
#include<stdlib.h>
#include<stdio.h>
typedef struct __mlfw_mat_double
{
    double **data;
    dimension_t rows;
    dimension_t columns;
}mlfw_mat_double;
mlfw_mat_double * mlfw_mat_double_create_new(dimension_t rows,dimension_t columns)
{
    mlfw_mat_double *matrix;
    index_t r,k;
    if(rows<0 || columns<0) return NULL;
    matrix=(mlfw_mat_double *)malloc(sizeof(mlfw_mat_double));
    if(matrix==NULL) return NULL;m
    matrix->data=(double **)malloc(sizeof(double *)*rows);
    if(matrix->data==NULL)
    {
        free(matrix);
        return NULL;
    }
    for(r=0;r<rows;r++)
    {
        matrix->data[r]=(double *)malloc(sizeof(double)*columns);
        if(matrix->data[r]==NULL)
        {
            for(k=0;k<r;k++) free(matrix->data[k]);
            free(matrix->data);
            free(matrix);
        }
    }
    matrix->rows=rows;
    matrix->columns=columns;
    return matrix;
}
void mlfw_mat_double_destroy(mlfw_mat_double *matrix)
{
    if(!matrix) return;
    index_t r;
    for(r=0;r<matrix->rows;r++) free(matrix->data[r]);
    free(matrix->data);
    free(matrix);
}
mlfw_mat_double * mlfw_mat_double_from_csv(const char *csv_file_name)
{
    FILE *file; //to open file
    if(csv_file_name==NULL) return NULL;
    file=fopen(csv_file_name,"r");
    if(!file) return NULL;
    mlfw_mat_double *matrix; //to create new matrix
    char m; //to reach each character of file
    dimension_t rows=0,columns=0;
    while(1) //counting number of rows and columns
    {
        m=fgetc(file);
        if(feof(file)) break;
        if(rows==0 && m==',') columns++;
        if(m=='\n') rows++;
    }
    columns++; // a,b,c means 2 commas and 2+1 columns
    matrix=mlfw_mat_double_create_new(rows,columns);
    if(matrix==NULL)
    {
        fclose(file);
        return NULL;
    }
    rewind(file); //move the internal pointer to first byte of file
    index_t r=0,c=0;
    char double_string[1025];
    int index=0;
    double value;
    while(1) //populating matrix
    {
        m=fgetc(file);
        if(feof(file)) break;
        if(m==',' || m=='\n')
        {
            double_string[index]='\0';
            value=strtod(double_string,NULL);
            index=0;
            matrix->data[r][c]=value;
            c++;
            if(c==matrix->columns)
            {
                r++;
                c=0;
            }
        }
        else
        {
            double_string[index]=m;
            index++;
        }
    }
    fclose(file);
    return matrix;
}


// void test_new_and_create()
// {
//     mlfw_mat_double *matrix;
//     matrix=mlfw_mat_double_create_new(3,4);
//     index_t r,c;
//     double value=10.23;
//     for(r=0;r<matrix->rows;r++)
//     {
//         for(c=0;c<matrix->columns;c++)
//         {
//             matrix->data[r][c]=value;
//             value+=10.20;
//         }
//     }
//     printf("Printing matrix contents");
//     for(r=0;r<matrix->rows;r++)
//     {
//         for(c=0;c<matrix->columns;c++)
//         {
//             printf("%20.10lf ",matrix->data[r][c]);
//         }
//         printf("\n");
//     }
//     mlfw_mat_double_destroy(matrix);
// }
// void test_load_from_csv()
// {
//     index_t r,c;
//     mlfw_mat_double *matrix;
//     printf("Test case from CSV\n");
//     matrix=mlfw_mat_double_from_csv("IceCreamSales.csv");
//     if(matrix==NULL)
//     {
//         printf("Unable to load CSV file: IceCreamSales.csv");
//         return;
//     }
//     printf("Printing matrix contents");
//     for(r=0;r<matrix->rows;r++)
//     {
//         for(c=0;c<matrix->columns;c++)
//         {
//             printf("%20.10lf ",matrix->data[r][c]);
//         }
//         printf("\n");
//     }
//     mlfw_mat_double_destroy(matrix);
// }
// int main()
// {
//     //test_new_and_create();
//     test_load_from_csv();
//     return 0;
// }
