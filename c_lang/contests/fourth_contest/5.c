#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define COMMA ,
#define MAX_STR_LEN 80

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
    SUCCESS_CODE = 1,
} status_code_t; 

status_code_t reverse_str_len(char *str, size_t len);
status_code_t left_shift(char *str, size_t shift_pos_cnt);
status_code_t swap_chars(char *fst_char, char *sec_char);
ssize_t read_line(char *buffer, size_t max_len);
status_code_t parse_input(
    char *str_buffer, size_t buffer_size, size_t *shift_pos_cnt);

int 
main(void)
{
    char input_str[MAX_STR_LEN + 1] = {0};
    size_t shift_pos_cnt = 0;

    status_code_t parse_input_rt_code = SUCCESS_CODE;
    status_code_t left_shift_rt_code = SUCCESS_CODE;

    parse_input_rt_code = parse_input(
        input_str, MAX_STR_LEN + 1, &shift_pos_cnt);

    if (parse_input_rt_code != SUCCESS_CODE)
        return EXIT_FAILURE;

    left_shift_rt_code = left_shift(input_str, shift_pos_cnt);

    if (left_shift_rt_code != SUCCESS_CODE)
        return EXIT_FAILURE;

    printf("%s\n", input_str);

    return EXIT_SUCCESS;
}

status_code_t 
parse_input(
    char *str_buffer, size_t buffer_size, size_t *shift_pos_cnt)
{
    SOFT_ASSERT(str_buffer == NULL,
        "str_buffer was received as a NULL pointer", NULL_PTR_ARG_CODE);

    SOFT_ASSERT(shift_pos_cnt == NULL,
        "shift_pos_cnt was received as a NULL pointer", NULL_PTR_ARG_CODE);

    SOFT_ASSERT(scanf(" %zu ", shift_pos_cnt) != 1,
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
    const ssize_t ERROR_VALUE = -1;

    SOFT_ASSERT(buffer == NULL,
        "buffer was received as a NULL pointer", ERROR_VALUE);

    SOFT_ASSERT(fgets(buffer, max_len, stdin) == NULL,
        "invalid input", buffer[0] = '\0' COMMA ERROR_VALUE);

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
left_shift(char *str, size_t shift_pos_cnt)
{
    SOFT_ASSERT(str == NULL,
        "str was received as a NULL pointer", NULL_PTR_ARG_CODE);

    size_t str_len = strlen(str);
    shift_pos_cnt %= str_len;

    reverse_str_len(str, str_len);
    reverse_str_len(str, str_len - shift_pos_cnt);
    reverse_str_len(str + str_len - shift_pos_cnt, shift_pos_cnt);

    return SUCCESS_CODE;
}

status_code_t
reverse_str_len(char *str, size_t len)
{
    SOFT_ASSERT(str == NULL,
        "str was received as a NULL pointer", NULL_PTR_ARG_CODE);

    for (int idx = 0; idx < len / 2; ++idx)
        swap_chars(&str[idx], &str[len - idx - 1]);

    return SUCCESS_CODE;
}

status_code_t
swap_chars(char *fst_char, char *sec_char)
{
    SOFT_ASSERT(fst_char == NULL,
        "fst_char was received as a NULL pointer", NULL_PTR_ARG_CODE);

    SOFT_ASSERT(sec_char == NULL,
        "sec_char was received as a NULL pointer", NULL_PTR_ARG_CODE);

    char tmp_var = *fst_char;
    
    *fst_char = *sec_char;
    *sec_char = tmp_var;

    return SUCCESS_CODE;
}
