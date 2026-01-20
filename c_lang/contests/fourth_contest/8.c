#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define COMMA ,
#define MAX_INPUT_LEN 100
#define POSS_NUMS_CNT 1000

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

status_code_t calc_three_dig_nums(const char *str, int *rs_cnt);
ssize_t read_line(char *buffer, size_t max_len);
int char_to_int(char ch);

int
main(void)
{
    char str_buffer[MAX_INPUT_LEN + 1] = {0};
    int rs_cnt = 0;

    status_code_t rt_calc_code = SUCCESS_CODE;
    ssize_t read_bytes_cnt = 0;

    read_bytes_cnt = read_line(str_buffer, MAX_INPUT_LEN + 1);

    if (read_bytes_cnt < 0)
        return EXIT_FAILURE;

    if (read_bytes_cnt < 3)
    {
        printf("0\n");
        return EXIT_SUCCESS;
    }

    rt_calc_code = calc_three_dig_nums(str_buffer, &rs_cnt);

    if (rt_calc_code < 0)
        return EXIT_FAILURE;

    printf("%d\n", rs_cnt);

    return EXIT_SUCCESS;
}

status_code_t
calc_three_dig_nums(const char *str, int *rs_cnt)
{
    SOFT_ASSERT(str == NULL,
        "str was received as a NULL pointer", NULL_PTR_ARG_CODE);

    SOFT_ASSERT(rs_cnt == NULL,
        "rs_cnt was received as a NULL pointer", NULL_PTR_ARG_CODE);

    bool poss_nums[POSS_NUMS_CNT] = {false};

    size_t str_len = strlen(str);
    int curr_num = 0;

    int fst_digit = 0;
    int sec_digit = 0;
    int third_digit = 0;

    for (int fst_idx = 0; fst_idx < str_len - 2; ++fst_idx)
    {
        fst_digit = char_to_int(str[fst_idx]);

        if (fst_digit == 0)
            continue;

        for (int sec_idx = fst_idx + 1; sec_idx < str_len - 1; ++sec_idx)
        {
            sec_digit = char_to_int(str[sec_idx]);

            for (int third_idx = sec_idx + 1; third_idx < str_len; ++third_idx)
            {
                third_digit = char_to_int(str[third_idx]);

                curr_num = 100 * fst_digit + 10 * sec_digit + third_digit;

                if (!poss_nums[curr_num])
                {
                    ++(*rs_cnt);
                    poss_nums[curr_num] = true;
                }

            }
        }
    }

    return SUCCESS_CODE;
}

ssize_t
read_line(char *buffer, size_t max_len)
{
    SOFT_ASSERT(buffer == NULL,
        "buffer was received as a NULL pointer", INVALID_INPUT_CODE);

    SOFT_ASSERT(fgets(buffer, max_len, stdin) == NULL,
        "invalid input", buffer[0] = '\0' COMMA INVALID_INPUT_CODE);

    size_t buffer_len = strlen(buffer);

    if (buffer_len > 0 && buffer[buffer_len - 1] == '\n')
    {
        buffer[buffer_len - 1] = '\0';  
        return buffer_len - 1;
    }

    int ch = 0;
    while ((ch = getchar()) != '\n' && ch != EOF);

    return buffer_len;
}

int
char_to_int(char ch)
{
    return ch - '0';
}
