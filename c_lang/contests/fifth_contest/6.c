#include <stdio.h>
#include <stdlib.h>

#define COMMA ,

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

status_code_t parse_input(
    char **input_str, size_t *input_str_len);
status_code_t check_pal(char *str, size_t str_len);

int 
main(void)
{
    char *input_str = NULL;
    size_t input_str_len = 0;

    if (parse_input(&input_str, &input_str_len) != SUCCESS_CODE)
        return EXIT_FAILURE;

    if (check_pal(input_str, input_str_len) != SUCCESS_CODE)
        return EXIT_FAILURE;

    free(input_str);

    return EXIT_SUCCESS;
}

status_code_t
check_pal(char *str, size_t str_len)
{
    SOFT_ASSERT(str == NULL,
        "str was received as a NULL pointer", NULL_PTR_ARG_CODE);

    size_t half_str_len = str_len / 2;

    for (size_t idx = 0; idx < half_str_len; ++idx)
        if (str[idx] != str[str_len - idx - 1])
        {
            puts("NO");
            return SUCCESS_CODE;
        }

    puts("YES");
    return SUCCESS_CODE;
}

status_code_t 
parse_input(
    char **input_str, size_t *input_str_len)
{
    SOFT_ASSERT(input_str == NULL,
        "ptr to input_str was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(input_str_len == NULL,
        "input_str_len was received as a NULL pointer", NULL_PTR_ARG_CODE);

    char *fgets_rt_ptr = NULL;

    SOFT_ASSERT(scanf(" %zu ", input_str_len) != 1,
        "Invalid input", INVALID_INPUT_CODE);

    *input_str = calloc(*input_str_len + 1, sizeof(**input_str));

    SOFT_ASSERT(input_str == NULL,
        "memory allocation for input_str failed", MEM_ALLOC_ERROR_CODE);

    fgets_rt_ptr = fgets(*input_str, *input_str_len + 1, stdin);

    SOFT_ASSERT(fgets_rt_ptr == NULL,
        "Invalid input", free(input_str) COMMA INVALID_INPUT_CODE);

    return SUCCESS_CODE;
}

