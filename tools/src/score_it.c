#include <mlfw_matrix.h>
#include <mlfw_vector.h>
#include <mlfw_operations.h>
#include <stdlib.h>
#include <stdio.h>

void score_it(char *results_file_name)
{
    mlfw_mat_double *dataset;
    mlfw_row_vec_string *dataset_header;
    dimension_t dataset_rows,dataset_columns;
    mlfw_column_vector_double *A;
    mlfw_column_vector_double *P;
    mlfw_column_vector_double *R; // R=A-P
    mlfw_row_vector_double *RT;   // Transpose of R
    mlfw_column_vector_double *RTR; // Sqaure of R that is R*RT
    mlfw_column_vector_double *M; //mean of actual values filled in vector
    mlfw_column_vector_double *AM;// AM=A-M (denominator or R2Score)
    mlfw_row_vector_double *AMT; //Transpose of AM
    mlfw_column_vector_double *AMTAM; //AMT * AM (Finding square of AM)
    double SSR;
    double SST;
    double R2Score,mean_of_actuals;

    dataset=mlfw_mat_double_from_csv(results_file_name,NULL,&dataset_header);
    if(dataset==NULL)
    {
        printf("Unable to load data from %s\n file\n",results_file_name);
        return;
    }
    mlfw_mat_double_get_dimension(dataset,&dataset_rows,&dataset_columns);
    A=mlfw_mat_double_create_column_vec(dataset,dataset_columns-2,NULL);
    if(A==NULL)
    {
        printf("Low Memory\n");
        mlfw_mat_double_destroy(dataset);
        mlfw_row_vec_string_destroy(dataset_header);    
        return;
    }
    P=mlfw_mat_double_create_column_vec(dataset,dataset_columns-1,NULL);
    if(P==NULL)
    {
        printf("Low Memory\n");
        mlfw_mat_double_destroy(dataset);
        mlfw_column_vector_double_destroy(A);
        mlfw_row_vec_string_destroy(dataset_header);
        return;
    }

    R=mlfw_subtract_double_column_vector(A,P,NULL);
    if(R==NULL)
    {
        printf("Low Memory\n");
        mlfw_mat_double_destroy(dataset);
        mlfw_column_vector_double_destroy(A);
        mlfw_column_vector_double_destroy(P);
        mlfw_row_vec_string_destroy(dataset_header);
        return;
    }
    RT=mlfw_column_vector_double_transpose(R,NULL);
    if(RT==NULL)
    {
        printf("Low Memory\n");
        mlfw_mat_double_destroy(dataset);
        mlfw_column_vector_double_destroy(A);
        mlfw_column_vector_double_destroy(P);
        mlfw_column_vector_double_destroy(R);
        mlfw_row_vec_string_destroy(dataset_header);
        return;
    }
    RTR=mlfw_multiply_double_row_vector_with_column_vector(RT,R,NULL);
    if(RTR==NULL)
    {
        printf("Low Memory\n");
        mlfw_mat_double_destroy(dataset);
        mlfw_column_vector_double_destroy(A);
        mlfw_column_vector_double_destroy(P);
        mlfw_column_vector_double_destroy(R);
        mlfw_row_vector_double_destroy(RT);
        mlfw_row_vec_string_destroy(dataset_header);
        return;
    }
    SSR=mlfw_column_vector_double_get(RTR,0);
    mean_of_actuals=mlfw_column_vector_double_mean(A);
    M=mlfw_column_vector_create_new_filled(dataset_rows,mean_of_actuals,NULL);
    if(M==NULL)
    {
        printf("Low Memory\n");
        mlfw_mat_double_destroy(dataset);
        mlfw_column_vector_double_destroy(A);
        mlfw_column_vector_double_destroy(P);
        mlfw_column_vector_double_destroy(R);
        mlfw_row_vector_double_destroy(RT);
        mlfw_column_vector_double_destroy(RTR);
        mlfw_row_vec_string_destroy(dataset_header);
        return;
    }
    AM=mlfw_subtract_double_column_vector(A,M,NULL);
    if(AM==NULL)
    {
        printf("Low Memory\n");
        mlfw_mat_double_destroy(dataset);
        mlfw_column_vector_double_destroy(A);
        mlfw_column_vector_double_destroy(P);
        mlfw_column_vector_double_destroy(R);
        mlfw_row_vector_double_destroy(RT);
        mlfw_column_vector_double_destroy(RTR);
        mlfw_column_vector_double_destroy(M);
        mlfw_row_vec_string_destroy(dataset_header);
        return;
    }
    AMT=mlfw_column_vector_double_transpose(AM,NULL);
    if(AMT==NULL)
    {
        printf("Low Memory\n");
        mlfw_mat_double_destroy(dataset);
        mlfw_column_vector_double_destroy(A);
        mlfw_column_vector_double_destroy(P);
        mlfw_column_vector_double_destroy(R);
        mlfw_row_vector_double_destroy(RT);
        mlfw_column_vector_double_destroy(RTR);
        mlfw_column_vector_double_destroy(M);
        mlfw_column_vector_double_destroy(AM);
        mlfw_row_vec_string_destroy(dataset_header);
        return;
    }
    AMTAM=mlfw_multiply_double_row_vector_with_column_vector(AMT,AM,NULL);
    if(AMTAM==NULL)
    {
        printf("Low Memory\n");
        mlfw_mat_double_destroy(dataset);
        mlfw_column_vector_double_destroy(A);
        mlfw_column_vector_double_destroy(P);
        mlfw_column_vector_double_destroy(R);
        mlfw_row_vector_double_destroy(RT);
        mlfw_column_vector_double_destroy(RTR);
        mlfw_column_vector_double_destroy(M);
        mlfw_column_vector_double_destroy(AM);
        mlfw_row_vector_double_destroy(AMT);
        mlfw_row_vec_string_destroy(dataset_header);
        return;
    }
    SST=mlfw_column_vector_double_get(AMTAM,0);
    R2Score=1-(SSR/SST);
    printf("R2Score of the test is %lf\n",R2Score);
    //Releasing resources
    mlfw_mat_double_destroy(dataset);
    mlfw_column_vector_double_destroy(A);
    mlfw_column_vector_double_destroy(P);
    mlfw_column_vector_double_destroy(R);
    mlfw_column_vector_double_destroy(RTR);
    mlfw_row_vector_double_destroy(RT);
    mlfw_column_vector_double_destroy(M);
    mlfw_column_vector_double_destroy(AM);
    mlfw_row_vector_double_destroy(AMT);
    mlfw_column_vector_double_destroy(AMTAM);
    mlfw_row_vec_string_destroy(dataset_header);
}

int main(int argc,char *argv[])
{
    if(argc!=2)
    {
        printf("Usage: [score_it.out results_file]\n");
        return 0;
    }
    score_it(argv[1]);
    return 0;
}
