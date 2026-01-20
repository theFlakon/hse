#include <stdio.h>
#include <stdbool.h>

#define MAX_SIZE 10001

#define soft_assert(condition, error_message, return_value) \
    do { \
        if (condition) { \
            fprintf(stderr, "Soft assertion failed: %s\n", error_message); \
            fprintf(stderr, "Condition: %s\n", #condition); \
            fprintf(stderr, "File: %s, Line: %d\n", __FILE__, __LINE__); \
            return return_value; \
        } \
    } while(0)

enum status_codes
{
    EXIT_SUCCESS_CODE = 0,
    EXIT_ERROR_CODE = 1,
};


void print_result(bool is_permutation);
bool parse_input(int *nums_cnt_arr, size_t *nums_cnt);
int check_permutation(
    int *nums_cnt_arr, size_t nums_cnt, bool *is_permutation);

int
main(void)
{
    int nums_cnt_arr[MAX_SIZE] = {0};
    bool is_permutation = true;

    size_t nums_cnt = 0;

    soft_assert(!parse_input(nums_cnt_arr, &nums_cnt),
        "Ivalid input", EXIT_ERROR_CODE);

    check_permutation(nums_cnt_arr, nums_cnt, &is_permutation);

    print_result(is_permutation);

    return EXIT_SUCCESS_CODE;
}

void
print_result(bool is_permutation)
{
    if (is_permutation)
    {
        printf("Yes\n");
    }

    else
    {
        printf("No\n");
    }
}

int 
check_permutation(
    int *nums_cnt_arr, size_t nums_cnt, bool *is_permutation)
{
    const int ERROR_CODE = -1;
    const int SUCCESS_CODE = 1;

    soft_assert(nums_cnt_arr == NULL,
        "nums_cnt_arr was received as a NULL pointer", ERROR_CODE);

    soft_assert(is_permutation == NULL,
        "is_permutation was received as a NULL pointer", ERROR_CODE);

    for (int idx = 0; idx < nums_cnt; ++idx)
    {
        if (nums_cnt_arr[idx] != 1)
        {
            *is_permutation = false;
            break;
        }
    }

    return SUCCESS_CODE;
}

bool
parse_input(int *nums_cnt_arr, size_t *nums_cnt)
{
    soft_assert(nums_cnt_arr == NULL,
        "nums_cnt_arr was received as a NULL pointer", false);

    soft_assert(nums_cnt == NULL, 
        "nums_cnt was received as a NULL pointer", false);

    int curr_num = 0;

    soft_assert(scanf("%zu", nums_cnt) != 1,
        "Invalid Input", false);

    for (size_t inp_num_ind = 0; inp_num_ind < *nums_cnt; ++inp_num_ind)
    {
        soft_assert(scanf("%d", &curr_num) != 1,
            "Invalid Input", false);

        if (curr_num < 1)
            continue;

        nums_cnt_arr[curr_num - 1]++;
    }

    return true;
}
