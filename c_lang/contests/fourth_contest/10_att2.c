#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STR_LEN 1000000

int calc_max_pref_suff(
    const char *fst_str, const char *sec_str);

int
main(void)
{
    char fst_str[MAX_STR_LEN + 1] = {0};
    char sec_str[MAX_STR_LEN + 1] = {0};

    int fst_result = 0;
    int second_result = 0;
    
    scanf("%[^\n] %[^\n]", 
        fst_str, sec_str);

    fst_result = calc_max_pref_suff(fst_str, sec_str);
    second_result = calc_max_pref_suff(sec_str, fst_str);

    printf("%d %d\n", fst_result, second_result);
}

int
calc_max_pref_suff(
    const char *fst_str, const char *sec_str)
{
    size_t fst_str_len = strlen(fst_str);
    size_t sec_str_len = strlen(sec_str);

    int start_ind = (fst_str_len < sec_str_len) ? sec_str_len - fst_str_len : 0;
    int fst_str_idx = 0;

    printf("start ind: %d\n", start_ind);

    for (int sec_str_idx = start_ind;
        sec_str_idx < sec_str_len; ++sec_str_idx)
    {
        if (sec_str[sec_str_idx] != fst_str[fst_str_idx])
        {
            printf("hi\n");
            fst_str_idx = (sec_str[sec_str_idx] == fst_str[0]) ? 1 : 0;
            continue;
        }

        ++fst_str_idx;
    }

    exit(1);

    return fst_str_idx;
}
