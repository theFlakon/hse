#include <stdio.h>
#include <stdlib.h>

#define COMMA ,

#ifdef LOG_ALL
    #define SOFT_ASSERT(condition, error_message, return_value) \
        do { \
            if (condition) { \
                fprintf(stderr, "Soft assertion failed: %s\n", error_message); \
                fprintf(stderr, "Condition: %s\n", #condition); \
                fprintf(stderr, "File: %s, Line: %d\n", __FILE__, __LINE__); \
                return return_value; \
            } \
        } while(0)
#else
#define SOFT_ASSERT(condition, error_message, return_value) \
    do { \
        if (condition) { \
            fprintf(stderr, "An Error occurred: %s\n", error_message); \
            return return_value; \
        } \
    } while(0)
#endif

typedef enum status_codes 
{
    INVALID_INPUT_CODE = -1,
    MEM_ALLOC_ERROR_CODE = -2,
    NULL_PTR_ARG_CODE = -3,
    SUCCESS_CODE = 1,
} status_code_t; 

status_code_t calc_median(int *nums_arr, int *rt_median);
status_code_t calc_iter_median(int *nums_arr, size_t size, 
    int *rt_iter_median);
int* create_nums_arr(size_t *nums_amount, status_code_t *rt_status_code);

int
main(void)
{
    status_code_t rt_iter_med_code = SUCCESS_CODE;
    status_code_t rt_nums_arr_code = SUCCESS_CODE;

    size_t nums_amount = 0; 
    int iter_median = 0;
    int *nums_arr = NULL;

    nums_arr = create_nums_arr(&nums_amount, &rt_nums_arr_code);

    if (nums_arr == NULL)
        return EXIT_FAILURE;

    rt_iter_med_code = calc_iter_median(nums_arr, nums_amount, &iter_median);

    free(nums_arr);

    if (rt_iter_med_code != SUCCESS_CODE)
        return EXIT_FAILURE;

    printf("%d\n", iter_median);

    return EXIT_SUCCESS;
}

int*
create_nums_arr(size_t *nums_amount, status_code_t *rt_status_code)
{
    SOFT_ASSERT(nums_amount == NULL,
        "nums_amount was received as a NULL pointer", NULL);

    SOFT_ASSERT(rt_status_code == NULL,
        "rt_status_code was received as a NULL pointer", NULL);

    int *rs_arr = NULL;

    SOFT_ASSERT(scanf("%zu", nums_amount) != 1,
        "Invalid input", *rt_status_code = INVALID_INPUT_CODE COMMA NULL);

    rs_arr = calloc(*nums_amount, sizeof(int));

    SOFT_ASSERT(rs_arr == NULL,
        "Memory allocation failed", 
        *rt_status_code = MEM_ALLOC_ERROR_CODE COMMA NULL);

    for (int idx = 0; idx < *nums_amount; ++idx)
    {
        SOFT_ASSERT(scanf("%d", &rs_arr[idx]) != 1,
            "Invalid input", 
        free(rs_arr) COMMA *rt_status_code = INVALID_INPUT_CODE COMMA NULL);
    }

    return rs_arr;
}

status_code_t
calc_median(int *nums_arr, int *rt_median)
{
    SOFT_ASSERT(nums_arr == NULL,
        "nums_arr was received as a NULL pointer", NULL_PTR_ARG_CODE);

    SOFT_ASSERT(rt_median == NULL,
        "rt_median was received as a NULL pointer", NULL_PTR_ARG_CODE);

    const int NUMS_CNT = 3;

    int greatest_num = 0;
    int middle_num = 0;

    for (int idx = 0; idx < NUMS_CNT; ++idx)
    {
        if (nums_arr[idx] > greatest_num)
        {
            middle_num = greatest_num;
            greatest_num = nums_arr[idx];
        }

        else if (nums_arr[idx] > middle_num)
        {
            middle_num = nums_arr[idx];
        }
    }

    *rt_median = middle_num;

    return SUCCESS_CODE;
}

status_code_t
calc_iter_median(int *nums_arr, size_t size, int *rt_iter_median)
{
    SOFT_ASSERT(nums_arr == NULL,
        "nums_arr was received as a NULL pointer", NULL_PTR_ARG_CODE);

    SOFT_ASSERT(rt_iter_median == NULL,
        "rt_iter_median was received as a NULL pointer", NULL_PTR_ARG_CODE);

    if (size == 1)
    {
        *rt_iter_median = nums_arr[0];
        return SUCCESS_CODE;
    }

    status_code_t result = SUCCESS_CODE;
    int median = 0;
    int *new_arr = NULL;

    new_arr = calloc(size - 2, sizeof(int));

    SOFT_ASSERT(new_arr == NULL,
        "memory allocation failed", MEM_ALLOC_ERROR_CODE);

    for (int idx = 0; idx < size - 2; ++idx)
    {
        calc_median(nums_arr + idx, &median);
        new_arr[idx] = median;
    }

    result = calc_iter_median(new_arr, size - 2, rt_iter_median);

    free(new_arr);

    return result;
}

