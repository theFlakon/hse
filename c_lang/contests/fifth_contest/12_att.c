#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    int *arr;
    size_t rows_cnt;
    size_t cols_cnt;
} matrix_t;

void get_matrix_element(matrix_t *matrix, size_t row_idx, size_t col_idx,
    int *rt_value);
void set_matrix_element(matrix_t *matrix, size_t row_idx, size_t col_idx,
    int element);

void swap_matrix_rows(matrix_t *matrix,
    size_t fst_row_idx, size_t sec_row_idx);
void operator_to_matrix_row(matrix_t *matrix, size_t row_idx, 
    int value, char operator);
void add_row_to_another(matrix_t *matrix, 
    size_t fst_row_idx, size_t sec_row_idx, int coeff);

void transf_matrix_to_step(matrix_t *matrix);

void swap(int *fst_var, int *sec_var);

int
main(void)
{
    matrix_t matrix;

    matrix.arr = calloc(4, 4);
    matrix.rows_cnt = 2;
    matrix.cols_cnt = 2;

    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            set_matrix_element(&matrix, i, j, (i + j) + 3);

    transf_matrix_to_step(&matrix);

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            int el;
            get_matrix_element(&matrix, i, j, &el);
            printf("%d ", el);
        }

        printf("\n");
    }

    return EXIT_SUCCESS;
}

void
transf_matrix_to_step(matrix_t *matrix)
{
    int curr_row_element = 0, curr_element = 0;

    size_t col_idx = 0;
    bool is_not_zero_found = false;

    while (col_idx < matrix->rows_cnt)
    {
        get_matrix_element(matrix, col_idx, col_idx, &curr_row_element);
        
        if (curr_row_element == 0)
        {
            for (size_t row_idx = col_idx + 1; row_idx < matrix->cols_cnt; ++row_idx)
            {
                get_matrix_element(matrix, row_idx, col_idx, &curr_element);

                if (curr_element)
                {
                    swap_matrix_rows(matrix, col_idx, row_idx);
                    is_not_zero_found = true;
                    break;
                }
            }

            if (!is_not_zero_found)
                col_idx++;
        }

        else
        {
            for (size_t row_idx = col_idx + 1; row_idx < matrix->cols_cnt; ++row_idx)
            {
                get_matrix_element(matrix, row_idx, col_idx, &curr_element);

                // printf("%d\n", curr_element);
                
                int coeff = (-curr_element) / curr_row_element;
                
                if (!curr_element)
                    continue;

                add_row_to_another(matrix, col_idx, row_idx, coeff);
            }

            col_idx++;
        }
    }
}

void
swap_matrix_rows(matrix_t *matrix, size_t fst_row_idx, size_t sec_row_idx)
{
    int *fst_row_curr_element = NULL;
    int *sec_row_curr_element = NULL;

    for (size_t col_idx = 0; col_idx < matrix->cols_cnt; ++col_idx)
    {
        fst_row_curr_element = &matrix->arr[
            fst_row_idx * matrix->cols_cnt + col_idx];

        sec_row_curr_element = &matrix->arr[
            sec_row_idx * matrix->cols_cnt + col_idx];

        swap(fst_row_curr_element, sec_row_curr_element);
    }
}

void
add_row_to_another(matrix_t *matrix, 
    size_t fst_row_idx, size_t sec_row_idx, int coeff)
{
    int *fst_row_curr_element = NULL;
    int *sec_row_curr_element = NULL;

    for (size_t col_idx = 0; col_idx < matrix->cols_cnt; ++col_idx)
    {
        fst_row_curr_element = &matrix->arr[
            fst_row_idx * matrix->cols_cnt + col_idx];

        sec_row_curr_element = &matrix->arr[
            sec_row_idx * matrix->cols_cnt + col_idx];

        *sec_row_curr_element += coeff * (*fst_row_curr_element);
    }
}

void 
operator_to_matrix_row(matrix_t *matrix, size_t row_idx, 
    int value, char operator)
{
    int *curr_element = NULL;

    for (size_t col_idx = 0; col_idx < matrix->cols_cnt; ++col_idx)
    {
        curr_element = &matrix->arr[
            row_idx * matrix->cols_cnt + col_idx];

        switch (operator)
        {
            case '+':
                *curr_element += value;
                break;

            case '-':
                *curr_element -= value;
                break;

            case '*':
                *curr_element *= value;
                break;

            case '/':
                *curr_element /= value;
                break;

            default:
                break;
        }
    }
}

void
get_matrix_element(matrix_t *matrix, size_t row_idx, size_t col_idx,
    int *rt_value)
{
    *rt_value = matrix->arr[row_idx * matrix->cols_cnt + col_idx];
}

void
set_matrix_element(matrix_t *matrix, size_t row_idx, size_t col_idx,
    int element)
{
    matrix->arr[row_idx * matrix->cols_cnt + col_idx] = element;
}

void
swap(int *fst_var, int *sec_var)
{
    int tmp_var = *fst_var;

    *fst_var = *sec_var;
    *sec_var = tmp_var;
}
