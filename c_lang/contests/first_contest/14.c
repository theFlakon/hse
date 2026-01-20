#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

void err_msg(const char* msg);
bool find_min_max_nums(
        int *max_fst, int *max_sec, int *min_fst, int *min_sec, int nums_cnt
        );

int
main(void) 
{
    int nums_cnt = 0;

    int max_fst = INT_MIN;
    int max_sec = INT_MIN;

    int min_fst = INT_MAX;
    int min_sec = INT_MAX;

    long long prod_max = 0LL;
    long long prod_min = 0LL;

    if (scanf("%d", &nums_cnt) != 1)
    {
        err_msg("invalid input arguments");
        return 1;
    }

    if (!find_min_max_nums(&max_fst, &max_sec, &min_fst, &min_sec, nums_cnt))
    {
        err_msg("invalid input arguments");
        return 1;
    }

    prod_max = (long long)max_fst * (long long)max_sec;
    prod_min = (long long)min_fst * (long long)min_sec;

    if (prod_max > prod_min)
        printf("%d %d\n", max_sec, max_fst);

    else
        printf("%d %d\n", min_fst, min_sec);

    return 0;
}

void
err_msg(const char* msg)
{
    fprintf(stderr, "An error occurred: %s\n", msg);
}

bool find_min_max_nums(
        int *max_fst, int *max_sec, int *min_fst, int *min_sec, int nums_cnt
)
{
    int cur_num = 0;

    for (int i = 0; i < nums_cnt; ++i)
    {
        if (scanf("%d", &cur_num) != 1)
            return false;

        if (cur_num > *max_fst)
        {
            *max_sec = *max_fst;
            *max_fst = cur_num;
        }

        else if (cur_num > *max_sec)
        {
            *max_sec = cur_num;
        }

        if (cur_num < *min_fst)
        {
            *min_sec = *min_fst;
            *min_fst = cur_num;
        }

        else if (cur_num < *min_sec)
        {
            *min_sec = cur_num;
        }
    }

    return true;
}
