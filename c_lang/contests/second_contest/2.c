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

enum arr_consts
{
    MAX_NUMS_CNT = 10000,
    FORWARD_SHIFT = 2,
    BACKWARD_SHIFT = 2,
};

enum status_codes
{
    EXIT_SUCCESS_CODE = 0,
    EXIT_ERROR_CODE = 0,
};

bool print_result(int arr[], int arr_len);
bool parse_input(int inp_nums_arr[], int *arr_len);

int
main(void)
{
    int arr[MAX_NUMS_CNT] = {0};
    int arr_len = 0;

    soft_assert(!parse_input(arr, &arr_len),
        "An error with input occurred", EXIT_ERROR_CODE);

    soft_assert(!print_result(arr, arr_len),
        "An error with printing occerred", EXIT_ERROR_CODE);

    return EXIT_SUCCESS_CODE;
}

bool
print_result(int arr[], int arr_len)
{
    soft_assert(
        arr_len < 0, "Len of array can't be below zero", false);


    for (int idx = 0; idx < arr_len; idx += FORWARD_SHIFT)
    {
        printf("%d ", arr[idx]);
    }

    if (arr_len % 2 != 0)
    {
        --arr_len;
    }

    for (int idx = arr_len - 1; idx > 0; idx -= BACKWARD_SHIFT)
    {
        printf("%d ", arr[idx]);
    }

    return true;
}

bool
parse_input(int inp_nums_arr[], int *arr_len)
{
    soft_assert(
        arr_len == NULL, "A NULL pointer was received", false);
    soft_assert(
        *arr_len < 0, "Len of array can't be below zero", false);

    int input_num = 0;

    do
    {
        soft_assert(scanf("%d", &input_num) != 1,
            "Error: Invalid input", false);

        if (input_num != 0)
        {
            inp_nums_arr[(*arr_len)++] = input_num;
        }

    } while (input_num != 0);

    return true;
}
