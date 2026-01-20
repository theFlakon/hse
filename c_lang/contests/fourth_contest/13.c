#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1000000
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

unsigned* z_func(const char *str);
ssize_t read_line(char *buffer, size_t max_len);
unsigned* find_periods(const char *str);
status_code_t print_result(const unsigned *result_arr);

int 
main(void)
{
    char input_str[MAX_INPUT_LEN + 2] = {0};

    ssize_t read_bytes_cnt = 0;

    read_bytes_cnt = read_line(input_str, MAX_INPUT_LEN + 1);

    if (read_bytes_cnt < 0)
        return EXIT_FAILURE;

    int *result_arr = find_periods(input_str);

    if (result_arr == NULL)
    {
        free(result_arr);
        return EXIT_FAILURE;
    }

    print_result(result_arr);

    free(result_arr);

    return EXIT_SUCCESS;
}

status_code_t
print_result(const unsigned *result_arr)
{
    SOFT_ASSERT(result_arr == NULL,
        "result_arr was received as a NULL pointer", NULL_PTR_ARG_CODE);

    for (int idx = 0; result_arr[idx]; ++idx)
        printf("%u ", result_arr[idx]);

    putc('\n', stdout);

    return SUCCESS_CODE;
}

unsigned*
find_periods(const char *str)
{
    SOFT_ASSERT(str == NULL,
        "str was received as a NULL pointer", NULL);

    size_t str_len = strlen(str);
    unsigned *z_arr = z_func(str);

    SOFT_ASSERT(z_arr == NULL,
        "z_arr creating was failed", NULL);

    unsigned *result = (unsigned *)calloc(sizeof(int), str_len + 1);

    SOFT_ASSERT(result == NULL,
        "Memory allocation was failed",
        free(result) COMMA NULL);

    size_t result_idx = 0;

    for (size_t idx = 1; idx < str_len; ++idx)
    {
        if (idx + z_arr[idx] == str_len)
            result[result_idx++] = idx;
    }

    result[result_idx] = str_len;

    free(z_arr); // result will be freed later

    return result;
}

unsigned*
z_func(const char *str)
{
    SOFT_ASSERT(str == NULL,
        "str was received as a NULL pointer", NULL);

    size_t str_len = strlen(str);

    unsigned *z_arr = calloc(str_len, sizeof(int));

    SOFT_ASSERT(z_arr == NULL,
        "memory allocation failed", NULL);

    int left = 0, right = 0;

    for (int idx = 1; idx < str_len; ++idx)
    {
        if (idx <= right)
        {
            int k_idx = idx - left;

            z_arr[idx] = (right - idx + 1 < z_arr[k_idx]) ? 
            right - idx + 1 : z_arr[k_idx];
        }

        while (idx + z_arr[idx] < str_len && 
            str[z_arr[idx]] == str[idx + z_arr[idx]])
        {
            z_arr[idx]++;
        }

        if (idx + z_arr[idx] - 1 > right)
        {
            left = idx;
            right = idx + z_arr[idx] - 1;
        }
    }

    return z_arr;
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
