#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define COMMA ,

#ifdef LOG_ALL
    #define SOFT_ASSERT(condition, error_message, return_value)                \
        do {                                                                   \
            if (condition) {                                                   \
                fprintf(stderr, "Soft assertion failed: %s\n", error_message); \
                fprintf(stderr, "Condition: %s\n", #condition);                \
                fprintf(stderr, "File: %s, Line: %d\n", __FILE__, __LINE__);   \
                return return_value;                                           \
            }                                                                  \
        } while(0)
#else
#define SOFT_ASSERT(condition, error_message, return_value)                    \
    do {                                                                       \
        if (condition) {                                                       \
            fprintf(stderr, "An Error occurred: %s\n", error_message);         \
            return return_value;                                               \
        }                                                                      \
    } while(0)
#endif

typedef enum  
{
    INVALID_INPUT_CODE = -1,
    MEM_ALLOC_ERROR_CODE = -2,
    NULL_PTR_ARG_CODE = -3,
    DIVISION_BY_ZERO_CODE = -4,
    SUCCESS_CODE = 1,
} status_code_t; 

typedef struct
{
    uint32_t *arr;
    uint32_t rows_cnt;
    uint32_t cols_cnt;
} matrix_t;

status_code_t matrix_constr(matrix_t **matrix,
        uint32_t rows_cnt, uint32_t cols_cnt);
status_code_t matrix_destr(matrix_t **matrix);

status_code_t parse_input(matrix_t **matrix);

// This func transform the matrix from right to left column
// selecting an appropriate value (the greatest one) 
// from the nearest rows and making sum with the curr. element
status_code_t transform_matrix(matrix_t *matrix);

status_code_t print_result(matrix_t *matrix);

status_code_t get_matrix_element(
    matrix_t *matrix, uint32_t *rt_value,
    uint32_t row_idx, uint32_t col_idx);

status_code_t set_matrix_element(
    matrix_t *matrix, uint32_t element, 
    uint32_t row_idx, uint32_t col_idx);

uint32_t get_max(
    uint32_t fst_num, uint32_t sec_num, uint32_t third_num);

int 
main(void)
{
    #define FREE_MEMORY_AND_EXIT(matrix, exit_code)                            \
        do {                                                                   \
            matrix_destr(&matrix);                                             \
            return exit_code;                                                  \
        } while(0);                                                            \

    matrix_t *matrix = NULL;

    if (parse_input(&matrix) != SUCCESS_CODE)
        FREE_MEMORY_AND_EXIT(matrix, EXIT_FAILURE);

    if (transform_matrix(matrix) != SUCCESS_CODE)
        FREE_MEMORY_AND_EXIT(matrix, EXIT_FAILURE);

    if (print_result(matrix) != SUCCESS_CODE)
        FREE_MEMORY_AND_EXIT(matrix, EXIT_FAILURE);

    FREE_MEMORY_AND_EXIT(matrix, EXIT_SUCCESS);

    #undef FREE_MEMORY_AND_EXIT
}

status_code_t
transform_matrix(matrix_t *matrix)
{
    SOFT_ASSERT(matrix == NULL,
        "matrix was received as a NULL pointer", NULL_PTR_ARG_CODE);

    uint32_t curr_cell_num = 0;
    uint32_t high_num = 0, mid_num = 0, low_num = 0;
    uint32_t max_num = 0;

    for (int col_idx = matrix->cols_cnt - 2; col_idx >= 0; --col_idx)
    {
        for (int row_idx = 0; row_idx < matrix->rows_cnt; ++row_idx)
        {
            get_matrix_element(
                matrix, &curr_cell_num, row_idx, col_idx);

            get_matrix_element(
                matrix, &mid_num, row_idx, col_idx + 1);

            if (row_idx == 0)
            {
                high_num = 0;
                get_matrix_element(
                    matrix, &low_num, row_idx + 1, col_idx + 1);
            }

            else if (row_idx == matrix->rows_cnt - 1)
            {
                low_num = 0;
                get_matrix_element(
                    matrix, &high_num, row_idx - 1, col_idx + 1);
            }

            else
            {
                get_matrix_element(
                    matrix, &low_num, row_idx + 1, col_idx + 1);

                get_matrix_element(
                    matrix, &high_num, row_idx - 1, col_idx + 1);
            }

            max_num = get_max(high_num, mid_num, low_num);

            set_matrix_element(
                matrix, curr_cell_num + max_num, row_idx, col_idx);
        }
    }

    return SUCCESS_CODE;
}
status_code_t
print_result(matrix_t *matrix)
{
    SOFT_ASSERT(matrix == NULL,
        "matrix was received as a NULL pointer", NULL_PTR_ARG_CODE);

    ssize_t result_to_print = -1;
    uint32_t curr_num = 0;

    for (size_t row_idx = 0; row_idx < matrix->rows_cnt; ++row_idx)
    {
        get_matrix_element(
            matrix, &curr_num, row_idx, 0);

        result_to_print = (curr_num > result_to_print) ? curr_num : result_to_print;
    }

    result_to_print = (result_to_print == -1) ? 0 : result_to_print;

    printf("%zd\n", result_to_print);

    return SUCCESS_CODE;
}

status_code_t 
parse_input(matrix_t **matrix)
{
    SOFT_ASSERT(matrix == NULL,
        "ptr to matrix was received as a NULL pointer", NULL_PTR_ARG_CODE);
    
    uint32_t rows_cnt = 0, cols_cnt = 0, curr_num = 0;
    status_code_t matrix_constr_rt_code = SUCCESS_CODE;

    SOFT_ASSERT(scanf("%u %u", &rows_cnt, &cols_cnt) != 2,
        "Invalid input", INVALID_INPUT_CODE);

    matrix_constr_rt_code = matrix_constr(matrix, rows_cnt, cols_cnt);

    if (matrix_constr_rt_code != SUCCESS_CODE)
        return matrix_constr_rt_code;

    for (size_t row_idx = 0; row_idx < rows_cnt; ++row_idx)
        for (size_t col_idx = 0; col_idx < cols_cnt; ++col_idx)
        {
            SOFT_ASSERT(scanf("%u", &curr_num) != 1,
                "Invalid input", 
                matrix_destr(matrix) COMMA INVALID_INPUT_CODE);

            set_matrix_element(*matrix, curr_num, row_idx, col_idx);
        }

    return SUCCESS_CODE;
}

uint32_t get_max(
    uint32_t fst_num, uint32_t sec_num, uint32_t third_num)
{
    uint32_t result = (fst_num > sec_num) ? fst_num : sec_num;
    result = (result > third_num) ? result : third_num;

    return result;
}

status_code_t
matrix_constr(matrix_t **matrix, uint32_t rows_cnt, uint32_t cols_cnt)
{
    SOFT_ASSERT(matrix == NULL,
        "ptr to matrix was received as a NULL pointer", NULL_PTR_ARG_CODE);

    *matrix = calloc(1, sizeof(**matrix));

    SOFT_ASSERT(*matrix == NULL,
        "Memory allocation of matrix struct failed", MEM_ALLOC_ERROR_CODE);

    (*matrix)->arr = calloc(rows_cnt * cols_cnt, sizeof(*((*matrix)->arr)));

    SOFT_ASSERT((*matrix)->arr == NULL,
        "Memory allocation of matrix array failed", 
        matrix_destr(matrix) COMMA MEM_ALLOC_ERROR_CODE);

    (*matrix)->rows_cnt = rows_cnt;
    (*matrix)->cols_cnt = cols_cnt;

    return SUCCESS_CODE;
}

status_code_t
matrix_destr(matrix_t **matrix)
{
    SOFT_ASSERT(matrix == NULL,
        "ptr to matrix was received as a NULL pointer", NULL_PTR_ARG_CODE);

    if (*matrix == NULL)
        return SUCCESS_CODE;

    free((*matrix)->arr);

    // Poisoning        //
    (*matrix)->arr = NULL;
    (*matrix)->rows_cnt = 0;
    (*matrix)->cols_cnt = 0;
    // End of poisoning //

    free(*matrix);
    *matrix = NULL;

    return SUCCESS_CODE;
}

status_code_t
get_matrix_element(matrix_t *matrix, uint32_t *rt_value,
    uint32_t row_idx, uint32_t col_idx)
{
    SOFT_ASSERT(matrix == NULL,
        "matrix was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(rt_value == NULL,
        "rt_value was received as a NULL pointer", NULL_PTR_ARG_CODE);

    *rt_value = matrix->arr[row_idx * (matrix->cols_cnt) + col_idx];

    return SUCCESS_CODE;
}

status_code_t 
set_matrix_element(matrix_t *matrix, uint32_t element, 
    uint32_t row_idx, uint32_t col_idx)
{
    SOFT_ASSERT(matrix == NULL,
        "matrix was received as a NULL pointer", NULL_PTR_ARG_CODE);

    matrix->arr[row_idx * (matrix->cols_cnt) + col_idx] = element;

    return SUCCESS_CODE;
}
