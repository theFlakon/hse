#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_STR_LEN 1000000

int calc_periods(const char *str);

int
main(void)
{
    char input_str[MAX_STR_LEN + 1] = {0};

    scanf("%[^\n]", input_str);

    calc_periods(input_str);
}

int
calc_periods(const char *str)
{
    size_t str_len = strlen(str);

    for (int potential_period = 1;
        potential_period < str_len;
        ++potential_period)
    {
        char period_char = str[0];

        for (int str_idx = potential_period;
            str_idx < str_len - potential_period + 1;
            ++str_idx)
        {
            if ()
        }
    }
}
