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

int32_t **get_matrix_arr(
    uint32_t **matrix_size_arr, uint32_t *matrix_cnt, status_code_t *rt_code);
int32_t *find_matrix_max_trace(
        int32_t **matrix_arr, uint32_t *matrix_size_arr, 
        uint32_t matrix_cnt, uint32_t *found_matrix_size, 
        status_code_t *rt_code);

status_code_t calс_trace(
    int64_t *rt_trace, int32_t *matrix, uint32_t matrix_size);
status_code_t print_matrix(int32_t *matrix, uint32_t matrix_size);
status_code_t free_matrix_arr(
    int32_t ***matrix_arr, uint32_t matrix_cnt);

int 
main(void)
{
    #define FREE_MEMORY_AND_EXIT(matrix_arr, matrix_cnt, exit_code)            \
        do {                                                                   \
            free_matrix_arr(&matrix_arr, matrix_cnt);                          \
            free(matrix_size_arr);                                             \
            matrix_size_arr = NULL;                                            \
            return exit_code;                                                  \
        } while(0);                                                            \

    int32_t **matrix_arr = NULL;
    uint32_t *matrix_size_arr = NULL, matrix_cnt = 0;

    int32_t *matrix_max_trace = NULL;
    uint32_t matrix_max_trace_size = 0;
    
    status_code_t getting_matrix_arr_code = SUCCESS_CODE;
    status_code_t finding_matrix_max_trace_code = SUCCESS_CODE;

    matrix_arr = get_matrix_arr(
        &matrix_size_arr, &matrix_cnt, &getting_matrix_arr_code);

    if (matrix_arr == NULL)
        FREE_MEMORY_AND_EXIT(matrix_arr, matrix_cnt, EXIT_FAILURE);

    matrix_max_trace = find_matrix_max_trace(
        matrix_arr, matrix_size_arr, matrix_cnt, &matrix_max_trace_size, 
        &finding_matrix_max_trace_code);

    if (finding_matrix_max_trace_code != SUCCESS_CODE)
        FREE_MEMORY_AND_EXIT(matrix_arr, matrix_cnt, EXIT_FAILURE);

    print_matrix(matrix_max_trace, matrix_max_trace_size);

    FREE_MEMORY_AND_EXIT(matrix_arr, matrix_cnt, EXIT_SUCCESS);

    #undef FREE_MEMORY_AND_EXIT
}

int32_t**
get_matrix_arr(
    uint32_t **matrix_size_arr, uint32_t *matrix_cnt, status_code_t *rt_code) 
{
    SOFT_ASSERT(rt_code == NULL,
        "rt_code was received as a NULL pointer", NULL);

    SOFT_ASSERT(matrix_size_arr == NULL,
        "matrix_size_arr was received as a NULL pointer", 
        *rt_code = INVALID_INPUT_CODE COMMA NULL);

    SOFT_ASSERT(matrix_cnt == NULL,
        "matrix_cnt was received as a NULL pointer", 
        *rt_code = INVALID_INPUT_CODE COMMA NULL);

    uint32_t curr_matrix_size = 0;
    int32_t **matrix_arr = NULL, *curr_matrix = NULL;
    int32_t curr_num = 0;

    SOFT_ASSERT(scanf("%u", matrix_cnt) != 1,
        "Invalid input", 
        *rt_code = INVALID_INPUT_CODE COMMA NULL);

    *matrix_size_arr = calloc(*matrix_cnt, sizeof(**matrix_size_arr));

    SOFT_ASSERT(*matrix_size_arr == NULL,
        "Memory allocation was failed", 
        *rt_code = MEM_ALLOC_ERROR_CODE COMMA NULL);
        
    matrix_arr = calloc(*matrix_cnt, sizeof(*matrix_arr));

    SOFT_ASSERT(matrix_arr == NULL,
        "Memory allocation failed", 
        free_matrix_arr(&matrix_arr, *matrix_cnt) COMMA 
        *rt_code = MEM_ALLOC_ERROR_CODE COMMA NULL);

    for (size_t matrix_idx = 0; matrix_idx < *matrix_cnt; ++matrix_idx)
    {
        SOFT_ASSERT(scanf("%u", &curr_matrix_size) != 1,
            "Invalid input", 
            free_matrix_arr(&matrix_arr, *matrix_cnt) COMMA
            *rt_code = INVALID_INPUT_CODE COMMA NULL);

        (*matrix_size_arr)[matrix_idx] = curr_matrix_size;

        matrix_arr[matrix_idx] = calloc(curr_matrix_size * curr_matrix_size, 
            sizeof(*matrix_arr[matrix_idx]));

        SOFT_ASSERT(matrix_arr[matrix_idx] == NULL,
            "Memory allocation failed", 
            free_matrix_arr(&matrix_arr, *matrix_cnt) COMMA
            *rt_code = MEM_ALLOC_ERROR_CODE COMMA NULL);

        curr_matrix = matrix_arr[matrix_idx];
        
        for (size_t row_idx = 0; row_idx < curr_matrix_size; ++row_idx)
            for (size_t col_idx = 0; col_idx < curr_matrix_size; ++col_idx)
            {
                SOFT_ASSERT(scanf("%d", &curr_num) != 1,
                    "Invalid input", 
                    free_matrix_arr(&matrix_arr, *matrix_cnt) COMMA
                    *rt_code = INVALID_INPUT_CODE COMMA NULL);

                curr_matrix[row_idx * curr_matrix_size + col_idx] = curr_num;
            }
    }

    return matrix_arr;
}

status_code_t
print_matrix(int32_t *matrix, uint32_t matrix_size)
{
    SOFT_ASSERT(matrix == NULL,
        "matrix_max_trace was received as a NULL pointer", NULL_PTR_ARG_CODE);

    for (int row_idx = 0; row_idx < matrix_size; ++row_idx)
    {
        for (int col_idx = 0; col_idx < matrix_size; ++col_idx)
            printf("%d ", matrix[row_idx * matrix_size + col_idx]);

        printf("\n");
    }

    return SUCCESS_CODE;
}

int32_t 
*find_matrix_max_trace(
        int32_t **matrix_arr, uint32_t *matrix_size_arr, 
        uint32_t matrix_cnt, uint32_t *found_matrix_size, 
        status_code_t *rt_code)
{
    SOFT_ASSERT(rt_code == NULL,
        "rt_code was received as a NULL pointer", NULL);
    SOFT_ASSERT(matrix_arr == NULL,
        "matrix_arr was received as a NULL pointer",
        *rt_code = NULL_PTR_ARG_CODE COMMA NULL);
    SOFT_ASSERT(matrix_size_arr == NULL,
        "matrix_size_arr was received as a NULL pointer",
        *rt_code = NULL_PTR_ARG_CODE COMMA NULL);
    SOFT_ASSERT(found_matrix_size == NULL,
        "found_matrix_size was received as a NULL pointer",
        *rt_code = NULL_PTR_ARG_CODE COMMA NULL);

    int64_t curr_trace = 0, max_trace = 0;
    int32_t *result_matrix = NULL;

    status_code_t calc_trace_rt_code = SUCCESS_CODE;

    for (size_t matrix_idx = 0; matrix_idx < matrix_cnt; ++matrix_idx)
    {
        calc_trace_rt_code =  calс_trace(
            &curr_trace, matrix_arr[matrix_idx], matrix_size_arr[matrix_idx]);

        if (calc_trace_rt_code != SUCCESS_CODE)
        {
            *rt_code = calc_trace_rt_code;
            return NULL;
        }

        if (max_trace == 0 || curr_trace > max_trace)
        {
            max_trace = curr_trace;
            result_matrix = matrix_arr[matrix_idx];
            *found_matrix_size = matrix_size_arr[matrix_idx];
        }
    }

    return result_matrix;
}

status_code_t
calс_trace(int64_t *rt_trace, int32_t *matrix, uint32_t matrix_size)
{
    SOFT_ASSERT(rt_trace == NULL,
        "rt_trace was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(matrix == NULL,
        "matrix was received as a NULL pointer", NULL_PTR_ARG_CODE);

    *rt_trace = 0;

    for (size_t row_col_idx = 0; row_col_idx < matrix_size; ++row_col_idx)
        *rt_trace += matrix[row_col_idx * matrix_size + row_col_idx];

    return SUCCESS_CODE;
}


status_code_t 
free_matrix_arr(
    int32_t ***matrix_arr, uint32_t matrix_cnt)
{
    SOFT_ASSERT(matrix_arr == NULL,
        "matrix_arr was received as a NULL pointer", NULL_PTR_ARG_CODE);

    if (*matrix_arr == NULL)
        return SUCCESS_CODE;

    for (size_t matrix_idx = 0; matrix_idx < matrix_cnt; ++matrix_idx)
    {
        free((*matrix_arr)[matrix_idx]);
        (*matrix_arr)[matrix_idx] = NULL;
    }

    free(*matrix_arr);
    *matrix_arr = NULL;

    return SUCCESS_CODE;
}
