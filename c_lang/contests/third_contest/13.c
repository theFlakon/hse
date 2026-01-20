#include <stdio.h>
#include <stdbool.h>

#define MAX_INPUT_NUM 14

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

typedef struct
{
    int curr_num;
    int curr_cnt;
    int nums_cnt;
    int need_cnt;
} nums_data_t;

status_code_t init_nums_data(nums_data_t *nums_data);
status_code_t parse_input(nums_data_t *nums_data);
status_code_t print_subsequences(nums_data_t *curr_data);
status_code_t print_arr_idxs(bool *arr, size_t arr_size);

int
main(void)
{
    nums_data_t curr_data;

    init_nums_data(&curr_data);

    if (parse_input(&curr_data) != SUCCESS_CODE)
        return EXIT_ERROR_CODE;

    print_subsequences(&curr_data);
}

status_code_t
parse_input(nums_data_t *nums_data)
{
    soft_assert(nums_data == NULL, 
        "nums_data was received as a NULL pointer", NULL_PTR_ARG_CODE);

    int input_nums_cnt = 0, input_need_cnt = 0;

    int read_input_cnt = scanf("%d %d", &input_nums_cnt, &input_need_cnt);

    user_assert(read_input_cnt != 2,
        "Invalid input", INVALID_INPUT_CODE);

    nums_data->nums_cnt = input_nums_cnt;
    nums_data->need_cnt = input_need_cnt;

    return SUCCESS_CODE;
}

status_code_t
init_nums_data(nums_data_t *nums_data)
{
    soft_assert(nums_data == NULL,
        "nums_data was received as a NULL pointer", NULL_PTR_ARG_CODE);

    nums_data->curr_num = 0;
    nums_data->curr_cnt = 0;
    nums_data->nums_cnt = 0;
    nums_data->need_cnt = 0;

    return SUCCESS_CODE;
}

status_code_t
print_subsequences(nums_data_t *curr_data)
{
    soft_assert(curr_data == NULL,
        "curr data was received as a NULL pointer", NULL_PTR_ARG_CODE);

    static bool printing_nums_arr[MAX_INPUT_NUM];

    if (curr_data->curr_cnt == curr_data->need_cnt)
    {
        print_arr_idxs(printing_nums_arr, MAX_INPUT_NUM);
        return SUCCESS_CODE;
    }

    for (int loop_num = curr_data->curr_num;
         loop_num < curr_data->nums_cnt; ++loop_num)
    {
        printing_nums_arr[loop_num] = true;

        nums_data_t next = *curr_data;
        next.curr_num = loop_num + 1;
        next.curr_cnt++;

        print_subsequences(&next);

        printing_nums_arr[loop_num] = false;
    }

    return SUCCESS_CODE;
}

status_code_t
print_arr_idxs(bool *arr, size_t arr_size)
{
    soft_assert(arr == NULL,
        "arr was received as a NULL pointer", NULL_PTR_ARG_CODE);

    for (int idx = 0; idx < arr_size; ++idx)
    {
        if (arr[idx])
        {
            printf("%d ", idx);
        }
    }

    printf("\n");
    
    return SUCCESS_CODE;
}
