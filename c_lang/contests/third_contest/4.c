#include <stdio.h>
#include <stdbool.h>

#define soft_assert(condition, error_message, return_value) \
    do { \
        if (condition) { \
            fprintf(stderr, "Soft assertion failed: %s\n", error_message); \
            fprintf(stderr, "Condition: %s\n", #condition); \
            fprintf(stderr, "File: %s, Line: %d\n", __FILE__, __LINE__); \
            return return_value; \
        } \
    } while(0)

#define user_assert(condition, error_message, return_value) \
    do { \
        if (condition) { \
            fprintf(stderr, "An error occurred: %s\n", error_message); \
            return return_value; \
        } \
    } while(0)

// Codes for return in funcs != main()
typedef enum status_codes 
{
    INVALID_INPUT_CODE = -1,
    MEM_ALLOC_ERROR_CODE = -2,
    NULL_PTR_ARG_CODE = -3,
    SUCCESS_CODE = 1,
} status_code_t; 

// Codes for return in main()
enum exit_status_codes 
{
    EXIT_SUCCESS_CODE = 0,
    EXIT_ERROR_CODE = 1,
}; 

typedef struct num_ind_data
{
    int num;
    int ind;
} num_ind_data;

int array_reversal(num_ind_data *curr_data);
status_code_t init_num_ind_data(num_ind_data *data);

int
main(void)
{
    num_ind_data curr_data;

    init_num_ind_data(&curr_data);

    user_assert(scanf("%d", &curr_data.num) != 1,
        "Invalid input", EXIT_ERROR_CODE);

    array_reversal(&curr_data);

    putc('\n', stdout);

    return EXIT_SUCCESS_CODE;
}

int
array_reversal(num_ind_data *curr_data)
{
    int rt_code = 0;

    num_ind_data next_data;

    init_num_ind_data(&next_data);

    next_data.ind = curr_data->ind + 1;

    if (curr_data->num == 0)
        return SUCCESS_CODE;

    if (curr_data->ind % 2 == 0)
        printf("%d ", curr_data->num);

    user_assert(scanf("%d", &next_data.num) != 1,
       "Invalid input", INVALID_INPUT_CODE);
    
    rt_code = array_reversal(&next_data);

    if (rt_code == SUCCESS_CODE && 
        curr_data->ind % 2)
        printf("%d ", curr_data->num);

    return rt_code;
}

status_code_t
init_num_ind_data(num_ind_data *data)
{
    soft_assert(data == NULL,
        "data was received as a NULL pointer", NULL_PTR_ARG_CODE);

    data->num = 0;
    data->ind = 0;

    return SUCCESS_CODE;
}

