#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STR_LEN 1000000

int calc_max_pref_suff(
    const char *fst_str, const char *sec_str);

int
main(void)
{
    char fst_str[MAX_STR_LEN + 1];
    char sec_str[MAX_STR_LEN + 1];

    int fst_result = 0;
    int second_result = 0;
    
    scanf("%[^\n] %[^\n]", 
        fst_str, sec_str);


    fst_result = calc_max_pref_suff(fst_str, sec_str);
    second_result = calc_max_pref_suff(sec_str, fst_str);

    printf("%d %d\n", fst_result, second_result);
}

int
calc_max_pref_suff(const char *fst_str, const char *sec_str) 
{
    size_t fst_len = strlen(fst_str);
    size_t sec_len = strlen(sec_str);
    size_t max_len = fst_len < sec_len ? fst_len : sec_len;

    for (size_t len = max_len; len > 0; --len) {
        if (strncmp(fst_str, sec_str + sec_len - len, len) == 0)
            return len;
    }
    return 0;
}
