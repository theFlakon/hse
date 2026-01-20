#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMMA ,
#define MAX_STR_LEN 1000000

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

status_code_t add_space(const char *str, int *spaces_series, size_t new_len);
ssize_t read_line(char *buffer, size_t max_len);
status_code_t parse_input(
    char *str_buffer, size_t buffer_size, size_t *new_str_len);
status_code_t print_result(const char *str, int *spaces_series);
void char_to_int(const char ch);

int
main(void)
{
    char input_str[MAX_STR_LEN + 1] = {0};
    int spaces_series_buffer[MAX_STR_LEN + 1] = {0};

    size_t new_str_len = 0;

    status_code_t parse_input_rt_code = SUCCESS_CODE;
    status_code_t add_space_rt_code = SUCCESS_CODE;

    parse_input_rt_code = parse_input(
        input_str, MAX_STR_LEN + 1, &new_str_len);

    if (parse_input_rt_code != SUCCESS_CODE)
        return EXIT_FAILURE;

    add_space_rt_code = add_space(input_str, spaces_series_buffer,
        new_str_len);

    if (add_space_rt_code != SUCCESS_CODE)
        return EXIT_FAILURE;

    print_result(input_str, spaces_series_buffer);

    return EXIT_SUCCESS;
}

status_code_t 
parse_input(
    char *str_buffer, size_t buffer_size, size_t *new_str_len)
{
    SOFT_ASSERT(str_buffer == NULL,
        "str_buffer was received as a NULL pointer", NULL_PTR_ARG_CODE);

    SOFT_ASSERT(new_str_len == NULL,
        "shift_pos_cnt was received as a NULL pointer", NULL_PTR_ARG_CODE);

    SOFT_ASSERT(scanf(" %zu ", new_str_len) != 1,
        "Invalid input", INVALID_INPUT_CODE);

    ssize_t read_bytes_cnt = 0;

    read_bytes_cnt = read_line(str_buffer, buffer_size);

    SOFT_ASSERT(read_bytes_cnt < 0,
        "Invalid input", INVALID_INPUT_CODE);

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

status_code_t
add_space(const char *str, int *spaces_series, size_t new_len)
{
    SOFT_ASSERT(str == NULL,
        "str was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(spaces_series == NULL,
        "spaces_series was received as a NULL pointer", NULL_PTR_ARG_CODE);

    size_t spaces_idx = 0;

    size_t str_len = strlen(str);
    size_t new_spaces = new_len - str_len;

    size_t every_add = 0;
    size_t part_add = 0;

    for (size_t idx = 0; idx < str_len; ++idx)
    {
        if (str[idx] == ' ')
            spaces_series[spaces_idx++] = 1;
    }

    SOFT_ASSERT(spaces_idx == 0,
        "spaces_idx equals to zero it leads to division by zero",
        DIVISION_BY_ZERO_CODE);

    every_add = new_spaces / spaces_idx;
    part_add = new_spaces % spaces_idx;

    for (size_t idx = 0; idx < spaces_idx; ++idx)
    {
        spaces_series[idx] += every_add;

        if (part_add)
        {
            ++spaces_series[idx];
            --part_add;
        }
    }

    return SUCCESS_CODE;
}

status_code_t
print_result(const char *str, int *spaces_series)
{
    SOFT_ASSERT(str == NULL,
        "str was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(spaces_series == NULL,
        "spaces_series was received as a NULL pointer", NULL_PTR_ARG_CODE);

    size_t str_len = strlen(str), spaces_idx = 0;

    for (size_t idx = 0; idx < str_len; ++idx)
    {
        if (str[idx] != ' ')
        {
            putc(str[idx], stdout);
            continue;
        }

        size_t spaces_to_print = spaces_series[spaces_idx];

        while (spaces_to_print)
        {
            putc(' ', stdout);
            --spaces_to_print;
        }

        ++spaces_idx;
    }

    putc('\n', stdout);

    return SUCCESS_CODE;
}

