#include <stdio.h>
#include <stdlib.h>

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
    int *arr;
    size_t len;
    size_t capacity;
} dynamic_arr;

status_code_t parse_input(
    dynamic_arr *even_pos_arr, dynamic_arr *odd_pos_arr);
status_code_t print_answer(
    dynamic_arr *even_pos_arr, dynamic_arr *odd_pos_arr);

status_code_t realloc_dynamic_arr(dynamic_arr *d_arr);
status_code_t dynamic_arr_destr(dynamic_arr **d_arr);
status_code_t dynamic_arr_constr(dynamic_arr **d_arr, 
    const size_t start_capacity);

int
main(void)
{
    #define FREE_MEMORY_AND_EXIT(even_pos_arr, odd_pos_arr, exit_code)         \
        do {                                                                   \
            dynamic_arr_destr(&even_pos_arr);                                  \
            dynamic_arr_destr(&odd_pos_arr);                                   \
            return exit_code;                                                  \
        } while(0);                                                            \
    
    const size_t START_CAPACITY = 2;
    status_code_t parse_input_code = SUCCESS_CODE;

    dynamic_arr *even_pos_arr;
    dynamic_arr *odd_pos_arr;

    dynamic_arr_constr(&even_pos_arr, START_CAPACITY);
    dynamic_arr_constr(&odd_pos_arr, START_CAPACITY);

    parse_input_code = parse_input(even_pos_arr, odd_pos_arr);

    if (parse_input_code != SUCCESS_CODE)
        FREE_MEMORY_AND_EXIT(even_pos_arr, odd_pos_arr, EXIT_FAILURE);

    print_answer(even_pos_arr, odd_pos_arr);

    FREE_MEMORY_AND_EXIT(even_pos_arr, odd_pos_arr, EXIT_SUCCESS);

    #undef FREE_MEMORY_AND_EXIT
}


status_code_t print_answer(
    dynamic_arr *even_pos_arr, dynamic_arr *odd_pos_arr)
{
    SOFT_ASSERT(even_pos_arr == NULL,
        "even_pos_arr was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(odd_pos_arr == NULL,
        "odd_pos_arr was received as a NULL pointer", NULL_PTR_ARG_CODE);

    for (size_t odd_idx = 0; odd_idx < odd_pos_arr->len; ++odd_idx)
        printf("%d ", odd_pos_arr->arr[odd_idx]);

    for (size_t even_idx = 0; even_idx < even_pos_arr->len; ++even_idx)
        printf("%d ", even_pos_arr->arr[even_idx]);

    printf("\n");

    return SUCCESS_CODE;
}

status_code_t
parse_input(dynamic_arr *even_pos_arr, dynamic_arr *odd_pos_arr)
{
    SOFT_ASSERT(even_pos_arr == NULL,
        "even_pos_arr was received as a NULL pointer", NULL_PTR_ARG_CODE);

    SOFT_ASSERT(odd_pos_arr == NULL,
        "odd_pos_arr was received as a NULL pointer", NULL_PTR_ARG_CODE);

    status_code_t realloc_d_arr_code = SUCCESS_CODE;
    dynamic_arr *curr_arr = NULL;
    int curr_num = 1;

    for (size_t curr_pos = 1; curr_num; ++curr_pos)
    {
        SOFT_ASSERT(scanf("%d", &curr_num) != 1,
            "Invalid input", INVALID_INPUT_CODE);

        if (!curr_num)
            continue;

        curr_arr = (curr_pos % 2) ? odd_pos_arr : even_pos_arr;

        curr_arr->arr[curr_arr->len++] = curr_num;
        
        if (curr_arr->len != curr_arr->capacity)
            continue;

        realloc_d_arr_code = realloc_dynamic_arr(curr_arr);

        if (realloc_d_arr_code != SUCCESS_CODE)
            return realloc_d_arr_code;
    }

    return SUCCESS_CODE;
}

status_code_t
realloc_dynamic_arr(dynamic_arr *d_arr)
{
    SOFT_ASSERT(d_arr == NULL,
        "d_arr was received as a NULL pointer", NULL_PTR_ARG_CODE);

    int *new_ptr = NULL;

    d_arr->capacity <<= 1;

    new_ptr = realloc(d_arr->arr, sizeof(*new_ptr) * d_arr->capacity);

    SOFT_ASSERT(new_ptr == NULL,
        "reallocation of dynamic arr failed", MEM_ALLOC_ERROR_CODE);

    d_arr->arr = new_ptr;

    return SUCCESS_CODE;
}

status_code_t 
dynamic_arr_constr(dynamic_arr **d_arr, const size_t start_capacity)
{
    SOFT_ASSERT(d_arr == NULL,
        "ptr to d_arr was received as a NULL poinger", NULL_PTR_ARG_CODE);

    *d_arr = calloc(1, sizeof(**d_arr));

    SOFT_ASSERT(*d_arr == NULL,
        "allocation of dynamic arr failed", MEM_ALLOC_ERROR_CODE);

    (*d_arr)->arr = calloc(start_capacity, sizeof(int));

    SOFT_ASSERT((*d_arr)->arr == NULL,
        "allocation of arr inside dynamic arr failed", MEM_ALLOC_ERROR_CODE);

    (*d_arr)->len = 0;
    (*d_arr)->capacity = start_capacity;

    return SUCCESS_CODE;
}

status_code_t 
dynamic_arr_destr(dynamic_arr **d_arr)
{
    SOFT_ASSERT(d_arr == NULL,
        "ptr to d_arr was received as a NULL pointer", NULL_PTR_ARG_CODE);

    if (*d_arr == NULL)
        return SUCCESS_CODE;

    free((*d_arr)->arr);

    // Poisoning       //
    (*d_arr)->arr = NULL;
    (*d_arr)->len = 0;
    (*d_arr)->capacity = 0;
    // End of poisoning //

    free(*d_arr);

    *d_arr = NULL;

    return SUCCESS_CODE;
}
