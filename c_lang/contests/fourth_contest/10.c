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

status_code_t calc_max_pref_suff(
    const char *fst_str, const char *sec_str, unsigned *rt_result);
unsigned* prefix_function(const char *str);
ssize_t read_line(char *buffer, size_t max_len);

int
main(void)
{
    char fst_str[MAX_STR_LEN + 1] = {0};
    char sec_str[MAX_STR_LEN + 1] = {0};

    unsigned fst_result = 0;
    unsigned sec_result = 0;

    ssize_t read_fst_bytes = 0;
    ssize_t read_sec_bytes = 0;

    status_code_t res_fst_rt_code = SUCCESS_CODE;
    status_code_t res_sec_rt_code = SUCCESS_CODE;

    read_fst_bytes = read_line(fst_str, MAX_STR_LEN + 1);

    if (read_fst_bytes < 0)
        return EXIT_FAILURE;

    read_sec_bytes = read_line(sec_str, MAX_STR_LEN + 1);

    if (read_sec_bytes < 0)
        return EXIT_FAILURE;
    
    res_fst_rt_code = calc_max_pref_suff(fst_str, sec_str, &fst_result);

    if (res_fst_rt_code != SUCCESS_CODE)
        return EXIT_FAILURE;

    res_sec_rt_code = calc_max_pref_suff(sec_str, fst_str, &sec_result);

    if (res_sec_rt_code != SUCCESS_CODE)
        return EXIT_FAILURE;

    printf("%d %d\n", fst_result, sec_result);

    return EXIT_SUCCESS;
}

status_code_t
calc_max_pref_suff(
    const char *fst_str, const char *sec_str, unsigned *rt_result)
{
    SOFT_ASSERT(fst_str == NULL,
        "fst_str was received as a NULL pointer", NULL_PTR_ARG_CODE);

    SOFT_ASSERT(sec_str == NULL,
        "sec_str was received as a NULL pointer", NULL_PTR_ARG_CODE);

    SOFT_ASSERT(rt_result == NULL,
        "rt_result was received as a NULL pointer", NULL_PTR_ARG_CODE);

    size_t new_str_len = strlen(fst_str) + strlen(sec_str) + 2;

    char *new_str = calloc(new_str_len, sizeof(char));

    SOFT_ASSERT(new_str == NULL,
        "memory allocation was failed", MEM_ALLOC_ERROR_CODE);

    int *prefix_suff_arr = NULL;

    strcpy(new_str, fst_str);
    strcat(new_str, " ");
    strcat(new_str, sec_str);

    prefix_suff_arr = prefix_function(new_str);

    SOFT_ASSERT(prefix_suff_arr == NULL,
        "An error was occured in prefix_function",
        free(new_str) COMMA MEM_ALLOC_ERROR_CODE);

    *rt_result = prefix_suff_arr[new_str_len - 2]; 

    free(prefix_suff_arr);
    free(new_str);

    return SUCCESS_CODE;
}

// This func. returns NULL ptr in case of an error
unsigned*
prefix_function(const char *str)
{
    SOFT_ASSERT(str == NULL,
        "str was received as a NULL pointer", NULL);

    size_t str_len = strlen(str);
    unsigned *prefix_suff = calloc(str_len, sizeof(unsigned));

    SOFT_ASSERT(prefix_suff == NULL,
        "Memory allocation was failed", NULL);

    for (int idx = 1; idx < str_len; ++idx)
    {
        int len = prefix_suff[idx - 1];

        while (len > 0 &&
            str[idx] != str[len])
        {
            len = prefix_suff[len - 1];
        }

        if (str[idx] == str[len])
        {
            len++;
        }

        prefix_suff[idx] = len;
    }

    return prefix_suff;
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

