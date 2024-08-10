#include <stdio.h>
#include <mlfw_matrix.h>
#include <mlfw_set.h>
#include <stdlib.h>
#include <mlfw_encoder.h>
#include <string.h>
void mlfw_encoder_encode_one_hot(char *source, char *target, int *encode_columns, int size, int header)
{
    FILE *target_file;
    if (source == NULL || target == NULL || encode_columns == NULL || size == 0) return;
    mlfw_mat_string *matrix;
    mlfw_set_string **sets;
    dimension_t matrix_rows, matrix_columns;
    index_t data_start_row_index;
    index_t i, c, j, r;
    char *string, *set_string;
    dimension_t set_size;

    matrix = mlfw_mat_string_from_csv(source);
    mlfw_mat_string_get_dimension(matrix, &matrix_rows, &matrix_columns);
    for (i = 0; i < size; i++)
    {
        c = encode_columns[i];
        if (c < 0 || c >= matrix_columns)
        {
            mlfw_mat_string_destroy(matrix);
            return;
        }
    }
    sets = (mlfw_set_string **)malloc(sizeof(mlfw_set_string *) * size);
    if (sets == NULL)
    {
        mlfw_mat_string_destroy(matrix);
        return;
    }
    for (i = 0; i < size; i++)
    {
        sets[i] = mlfw_set_string_create_new();
        if (sets[i] == NULL)
        {
            for (j = 0; j < i; j++)
                mlfw_set_string_destroy(sets[j]);
            mlfw_mat_string_destroy(matrix);
            return;
        }
    }
    if (header)
        data_start_row_index = 1;
    else
        data_start_row_index = 0;
    for (r = data_start_row_index; r < matrix_rows; r++)
    {
        for (i = 0; i < size; i++)
        {
            c = encode_columns[i];
            mlfw_mat_string_get(matrix, r, c, &string);
            if (string == NULL)
            {
                for (j = 0; j < size; j++)
                    mlfw_set_string_destroy(sets[j]);
                free(sets);
                mlfw_mat_string_destroy(matrix);
                return;
            }
            else
            {
                if (mlfw_set_string_add(sets[i], string) == -1)
                {
                    free(string);
                    for (j = 0; j < size; j++)
                        mlfw_set_string_destroy(sets[j]);
                    free(sets);
                    mlfw_mat_string_destroy(matrix);
                    return;
                }
                free(string);
            }
        }
    }
    // for(i=0;i<size;i++)
    // {
    //     set_size=mlfw_set_string_get_size(sets[i]);
    //     for(j=0;j<set_size;j++)
    //     {
    //         mlfw_set_string_get(sets[i],j,&string);
    //         if(string!=NULL)
    //         {
    //             printf("%s\n",string);
    //             free(string);
    //         }
    //     }
    // }
    target_file = fopen(target, "w");
    if (target_file == NULL)
    {
        for (j = 0; j < size; j++)
            mlfw_set_string_destroy(sets[j]);
        free(sets);
        mlfw_mat_string_destroy(matrix);
        return;
    }
    if (header)
    {
        for (c = 0; c < matrix_columns; c++)
        {
            mlfw_mat_string_get(matrix, 0, c, &string);
            if (string == NULL)
            {
                for (j = 0; j < size; j++)
                    mlfw_set_string_destroy(sets[j]);
                free(sets);
                mlfw_mat_string_destroy(matrix);
                fclose(target_file);
                target_file = fopen(target, "w");
                fclose(target_file);
                return;
            }
            for (i = 0; i < size; i++)
            {
                if (c == encode_columns[i])
                    break;
            }
            if (i < size)
            {
                set_size = mlfw_set_string_get_size(sets[i]);
                for (j = 0; j < set_size; j++)
                {
                    mlfw_set_string_get(sets[i], j, &set_string);
                    if (set_string == NULL)
                    {
                        free(string);
                        for (j = 0; j < size; j++)
                            mlfw_set_string_destroy(sets[j]);
                        free(sets);
                        mlfw_mat_string_destroy(matrix);
                        fclose(target_file);
                        target_file = fopen(target, "w");
                        fclose(target_file);
                        return;
                    }
                    fputs(string, target_file);
                    fputc('_', target_file);
                    fputs(set_string, target_file);
                    free(set_string);
                    if (j < set_size - 1)
                        fputc(',', target_file);
                }
            }
            else
            {
                fputs(string, target_file);
            }
            free(string);
            if (c == matrix_columns - 1)
                fputc('\n', target_file);
            else
                fputc(',', target_file);
        }
    }
    for (r = data_start_row_index; r < matrix_rows; r++)
    {
        for (c = 0; c < matrix_columns; c++)
        {
            mlfw_mat_string_get(matrix, r, c, &string);
            if (string == NULL)
            {
                for (j = 0; j < size; j++) mlfw_set_string_destroy(sets[j]);
                free(sets);
                mlfw_mat_string_destroy(matrix);
                fclose(target_file);
                target_file = fopen(target, "w");
                fclose(target_file);
                return;
            }
            for (i = 0; i < size; i++)
            {
                if (c == encode_columns[i]) break;
            }
            if (i < size)
            {
                set_size = mlfw_set_string_get_size(sets[i]);
                for (j = 0; j < set_size; j++)
                {
                    mlfw_set_string_get(sets[i], j, &set_string);
                    if (set_string == NULL)
                    {
                        free(string);
                        for (j = 0; j < size; j++) mlfw_set_string_destroy(sets[j]);
                        free(sets);
                        mlfw_mat_string_destroy(matrix);
                        fclose(target_file);
                        target_file = fopen(target, "w");
                        fclose(target_file);
                        return;
                    }
                    if(strcmp(string,set_string)==0)
                    {
                        fputc('1',target_file);
                    }
                    else
                    {
                        fputc('0',target_file);
                    }
                    free(set_string);
                    if (j < set_size - 1) fputc(',', target_file);
                }
            }
            else
            {
                fputs(string, target_file);
            }
            free(string);
            if (c == matrix_columns - 1)
                fputc('\n', target_file);
            else
                fputc(',', target_file);
        }
    }
    fclose(target_file);
}
