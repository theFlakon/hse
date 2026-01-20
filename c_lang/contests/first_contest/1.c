#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

void err_msg(const char* msg);
bool parse_input(int* out_min, int* out_max, int nums_cnt);

int
main(void)
{
    int nums_cnt = 0;
    int min = INT_MAX;
    int max = INT_MIN;

    if (scanf("%d", &nums_cnt) != 1)
    {
        err_msg("invalid input arguments");
        return 1;
    }

    if (!parse_input(&min, &max, nums_cnt))
    {
        err_msg("invalid input arguments");
        return 1;
    }
    
    printf("%d\n", max - min);
    
    return 0;
}

void
err_msg(const char* msg)
{
    fprintf(stderr, "An error occurred: %s\n", msg);
}

bool
parse_input(int* out_min, int* out_max, int nums_cnt)
{
    int input_num = 0;

    for (int i = 0; i < nums_cnt; ++i)
    {
        if (scanf("%d", &input_num) != 1)
            return false;

        if (input_num < *out_min)
            *out_min = input_num;

        if (input_num > *out_max)
            *out_max = input_num;
    }

    return true;
}
