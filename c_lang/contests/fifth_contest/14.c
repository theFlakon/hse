#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>

#define START_CAPACITY 2

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
    uint64_t *ptr;
    size_t len;
    size_t capacity;
} dynamic_arr_t;

status_code_t add_element_to_dynamic_arr(dynamic_arr_t *d_arr, uint64_t element);
status_code_t add_elements_to_dynamic_arr(dynamic_arr_t *d_arr, 
    uint64_t *elements, size_t elements_size);
status_code_t dynamic_arr_destr(dynamic_arr_t **d_arr);
status_code_t dynamic_arr_constr(dynamic_arr_t **d_arr,
        const size_t start_capacity);
status_code_t realloc_dynamic_arr(dynamic_arr_t *d_arr);

status_code_t get_new_nums_arr(dynamic_arr_t *curr_nums_arr, 
        dynamic_arr_t *new_nums_arr);
status_code_t find_hyperprimes(size_t exp, dynamic_arr_t **result);
status_code_t print_result(dynamic_arr_t *result);

bool is_prime(size_t num); 

int main(void) 
{
    #define FREE_MEMORY_AND_EXIT(result, exit_code)                            \
    do                                                                         \
    {                                                                          \
        dynamic_arr_destr(&result);                                            \
        return exit_code;                                                      \
    } while(0);                                                                \

    dynamic_arr_t *result = NULL;
    size_t exp = 0;

    SOFT_ASSERT(scanf("%zu", &exp) != 1,
        "Invalid input", EXIT_FAILURE);

    if (find_hyperprimes(exp, &result) != SUCCESS_CODE)
        FREE_MEMORY_AND_EXIT(result, EXIT_FAILURE);

    if (print_result(result) != SUCCESS_CODE)
        FREE_MEMORY_AND_EXIT(result, EXIT_FAILURE);
    
    FREE_MEMORY_AND_EXIT(result, EXIT_SUCCESS);

    #undef FREE_MEMORY_AND_EXIT
}

status_code_t
print_result(dynamic_arr_t *result)
{
    SOFT_ASSERT(result == NULL,
        "result was received as a NULL pointer", NULL_PTR_ARG_CODE);

    for (size_t idx = 0; idx < result->len; ++idx)
        printf("%lu ", result->ptr[idx]);

    printf("\n");

    return SUCCESS_CODE;
}

status_code_t 
get_new_nums_arr(dynamic_arr_t *curr_nums_arr, dynamic_arr_t *new_nums_arr)
{
    SOFT_ASSERT(curr_nums_arr == NULL,
        "curr_nums_arr was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(new_nums_arr == NULL,
        "new_nums_arr was received as a NULL pointer", NULL_PTR_ARG_CODE);

    status_code_t adding_rt_code = SUCCESS_CODE;

    uint64_t curr_num = 0;

    const uint64_t DIGITS_ARR[] = {1, 3, 7, 9};
    const size_t DIGITS_ARR_SIZE = 4;

    for (size_t curr_nums_idx = 0; curr_nums_idx < curr_nums_arr->len;
        ++curr_nums_idx)
    {
        for (size_t digits_idx = 0; digits_idx < DIGITS_ARR_SIZE; 
            ++digits_idx)
        {
            curr_num = curr_nums_arr->ptr[curr_nums_idx] * 10 
                + DIGITS_ARR[digits_idx];

            if (is_prime(curr_num))
            {
                adding_rt_code = add_element_to_dynamic_arr(new_nums_arr,
                    curr_num);

                if (adding_rt_code != SUCCESS_CODE)
                    return adding_rt_code;
            }
        }
    }

    return SUCCESS_CODE;
}

status_code_t
find_hyperprimes(size_t exp, dynamic_arr_t **result)
{
    SOFT_ASSERT(result == NULL,
        "ptr to result was received as a NULL pointer", NULL_PTR_ARG_CODE);

    dynamic_arr_t *prev_nums_arr = NULL;
    dynamic_arr_t *new_nums_arr = NULL;

    status_code_t prev_nums_arr_constr_code = SUCCESS_CODE;
    status_code_t new_nums_arr_constr_code = SUCCESS_CODE;
    status_code_t adding_rt_code = SUCCESS_CODE;
    status_code_t getting_rt_code = SUCCESS_CODE;

    prev_nums_arr_constr_code = dynamic_arr_constr(&prev_nums_arr, START_CAPACITY);

    if (prev_nums_arr_constr_code != SUCCESS_CODE)
        return prev_nums_arr_constr_code;

    uint64_t base_nums[] = {2, 3, 5, 7};
    const size_t BASE_NUMS_CNT = 4;

    for (size_t loop_exp = 1; loop_exp <= exp; ++loop_exp)
    {
        new_nums_arr_constr_code = dynamic_arr_constr(&new_nums_arr,
            prev_nums_arr->capacity);

        if (new_nums_arr_constr_code != SUCCESS_CODE)
        {
            dynamic_arr_destr(&prev_nums_arr);

            return new_nums_arr_constr_code;
        }

        if (loop_exp == 1)
            adding_rt_code = add_elements_to_dynamic_arr(new_nums_arr, 
                    base_nums, BASE_NUMS_CNT);

        else
            getting_rt_code = get_new_nums_arr(prev_nums_arr, new_nums_arr);

        dynamic_arr_destr(&prev_nums_arr);

        if (adding_rt_code != SUCCESS_CODE || getting_rt_code != SUCCESS_CODE)
        {
            dynamic_arr_destr(&new_nums_arr);

            return (adding_rt_code != SUCCESS_CODE) ? 
                adding_rt_code : getting_rt_code;
        }

        prev_nums_arr = new_nums_arr;
        new_nums_arr = NULL;
    }

    *result = prev_nums_arr;

    return SUCCESS_CODE;
}

status_code_t 
add_element_to_dynamic_arr(dynamic_arr_t *d_arr, uint64_t element)
{
    SOFT_ASSERT(d_arr == NULL,
        "d_arr was received as a NULL pointer", NULL_PTR_ARG_CODE);

    status_code_t realloc_rt_code = SUCCESS_CODE;

    if (d_arr->len == d_arr->capacity)
    {
        realloc_rt_code = realloc_dynamic_arr(d_arr);

        if (realloc_rt_code != SUCCESS_CODE)
            return realloc_rt_code;
    }

    d_arr->ptr[d_arr->len++] = element;

    return SUCCESS_CODE;
}

status_code_t 
add_elements_to_dynamic_arr(dynamic_arr_t *d_arr, 
    uint64_t *elements, size_t elements_size)
{
    SOFT_ASSERT(d_arr == NULL,
        "d_arr was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(elements == NULL,
        "elements was received as a NULL pointer", NULL_PTR_ARG_CODE);

    status_code_t adding_rt_code = SUCCESS_CODE;

    for (size_t idx = 0; idx < elements_size; ++idx)
    {
        adding_rt_code = add_element_to_dynamic_arr(d_arr, elements[idx]);

        if (adding_rt_code != SUCCESS_CODE)
            return adding_rt_code;
    }

    return SUCCESS_CODE;
}

status_code_t
realloc_dynamic_arr(dynamic_arr_t *d_arr)
{
    SOFT_ASSERT(d_arr == NULL,
        "d_arr was received as a NULL pointer", NULL_PTR_ARG_CODE);

    uint64_t *new_ptr = NULL;

    d_arr->capacity <<= 1;

    new_ptr = realloc(d_arr->ptr, sizeof(*new_ptr) * d_arr->capacity);

    SOFT_ASSERT(new_ptr == NULL,
        "reallocation of dynamic arr failed", MEM_ALLOC_ERROR_CODE);

    d_arr->ptr = new_ptr;

    return SUCCESS_CODE;
}

status_code_t 
dynamic_arr_constr(dynamic_arr_t **d_arr, const size_t start_capacity)
{
    SOFT_ASSERT(d_arr == NULL,
        "ptr to d_arr was received as a NULL pointer", NULL_PTR_ARG_CODE);

    *d_arr = calloc(1, sizeof(**d_arr));

    SOFT_ASSERT(*d_arr == NULL,
        "allocation of dynamic arr failed", MEM_ALLOC_ERROR_CODE);

    (*d_arr)->ptr = calloc(start_capacity, sizeof(uint64_t));

    SOFT_ASSERT((*d_arr)->ptr == NULL,
        "allocation of arr inside dynamic arr failed", MEM_ALLOC_ERROR_CODE);

    (*d_arr)->len = 0;
    (*d_arr)->capacity = start_capacity;

    return SUCCESS_CODE;
}

status_code_t 
dynamic_arr_destr(dynamic_arr_t **d_arr)
{
    SOFT_ASSERT(d_arr == NULL,
        "ptr to d_arr was received as a NULL pointer", NULL_PTR_ARG_CODE);

    if (*d_arr == NULL)
        return SUCCESS_CODE;

    free((*d_arr)->ptr);

    // Poisoning       //
    (*d_arr)->ptr = NULL;
    (*d_arr)->len = 0;
    (*d_arr)->capacity = 0;
    // End of poisoning //

    free(*d_arr);

    *d_arr = NULL;

    return SUCCESS_CODE;
}

bool 
is_prime(size_t num) 
{
    if (num < 2)
        return false;

    if (num == 2)
        return true;

    if (num % 2 == 0)
        return false;
    
    size_t limit = (size_t)sqrt((double)num) + 1;

    for (size_t divisor = 3; divisor < limit; divisor += 2) 
        if (num % divisor == 0) 
            return false;
    
    return true;
}

