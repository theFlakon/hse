#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define COMMA ,
#define MAX_STR_LEN 100

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

status_code_t get_syllables_arr(size_t **syllables_arr, 
    size_t *syllables_arr_len);
status_code_t print_suit_sylls_lines(size_t *syllables_arr, 
    size_t syllables_arr_len);
bool is_vowel(int character);
ssize_t read_line(char *buffer, size_t max_len);

int main(void)
{
    status_code_t sylls_getting_code = SUCCESS_CODE;
    size_t *syllables = NULL;
    size_t syllables_arr_len = 0;

    sylls_getting_code = get_syllables_arr(&syllables, &syllables_arr_len);

    if (sylls_getting_code != SUCCESS_CODE)
        return EXIT_FAILURE;

    print_suit_sylls_lines(syllables, syllables_arr_len);

    free(syllables);

    return EXIT_SUCCESS;
}

status_code_t 
print_suit_sylls_lines(size_t *syllables_arr, size_t syllables_arr_len)
{
    SOFT_ASSERT(syllables_arr == NULL,
        "syllables_arr was received as a NULL pointer", NULL_PTR_ARG_CODE);

    char curr_line[MAX_STR_LEN + 2] = {0};

    ssize_t read_bytes_cnt = 0;

    for (size_t line_idx = 0; line_idx < syllables_arr_len; ++line_idx)
    {
        read_bytes_cnt = read_line(curr_line, MAX_STR_LEN + 2);

        SOFT_ASSERT(read_bytes_cnt < 0,
            "Invalid input", INVALID_INPUT_CODE);

        size_t syllables_cnt = 0;
        bool in_vowel_sequence = false;

        for (size_t idx = 0; curr_line[idx]; ++idx)
        {
            int curr_ch = tolower(curr_line[idx]);
            bool is_vowel_now = is_vowel(curr_ch);

            if (is_vowel_now && !in_vowel_sequence)
            {
                ++syllables_cnt;
                in_vowel_sequence = true;
            }
            else if (!is_vowel_now)
            {
                in_vowel_sequence = false;
            }
        }

        if (syllables_cnt == syllables_arr[line_idx])
            printf("%s\n", curr_line);
    }

    return SUCCESS_CODE;
}

status_code_t get_syllables_arr(size_t **syllables_arr, size_t *syllables_arr_len)
{
    SOFT_ASSERT(syllables_arr == NULL,
        "syllables_arr was received as a NULL pointer", NULL_PTR_ARG_CODE);
    
    SOFT_ASSERT(syllables_arr_len == NULL,
        "syllables_arr_len was received as a NULL pointer", NULL_PTR_ARG_CODE);

    size_t arr_len = 0, curr_input_num = 0;

    SOFT_ASSERT(scanf("%zu", &arr_len) != 1,
        "Invalid input", INVALID_INPUT_CODE);

    *syllables_arr_len = arr_len;
    *syllables_arr = calloc(arr_len, sizeof(size_t));

    SOFT_ASSERT(*syllables_arr == NULL,
        "memory allocation failed", MEM_ALLOC_ERROR_CODE);

    for (size_t idx = 0; idx < arr_len; ++idx)  
    {
        SOFT_ASSERT(scanf(" %zu ", &curr_input_num) != 1,
            "Invalid input", 
            free(*syllables_arr) COMMA *syllables_arr = NULL
            COMMA INVALID_INPUT_CODE);

        (*syllables_arr)[idx] = curr_input_num;
    }

    return SUCCESS_CODE;
}

bool is_vowel(int character)
{
    return (character == 'a' || character == 'e' || character == 'i' ||
        character == 'o' || character == 'u' || character == 'y');
}

ssize_t read_line(char *buffer, size_t max_len)
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

