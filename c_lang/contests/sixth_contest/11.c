#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>

#define START_CAPACITY 2
#define INT_ERROR_VALUE -1
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

union value
{
    long long number;
    char operator;
};

enum tokens_types
{
    NUMBER = 1,
    MULT_OPER = 2,
    DIVIDE_OPER = 3,
    ADD_OPER = 4,
    SUB_OPER = 5,
    OPEN_PAR = 6,
    CLOSE_PAR = 7,
};

typedef struct
{
    enum tokens_types type;
    union value value;
} token_t;

typedef struct
{
    token_t **ptr;
    size_t len;
    size_t capacity;
} dynamic_arr_t;

status_code_t dynamic_arr_constr(dynamic_arr_t **d_arr,
    const size_t start_capacity);
status_code_t dynamic_arr_destr(dynamic_arr_t **d_arr);

status_code_t add_element_to_dynamic_arr(dynamic_arr_t *d_arr, 
        token_t *element);
status_code_t realloc_dynamic_arr(dynamic_arr_t *d_arr);

status_code_t get_tokens_arr(dynamic_arr_t *tokens_arr, char *str);

status_code_t init_token_as_oper(token_t *token, char oper);
status_code_t init_token_as_num(token_t *token, 
    char *str, size_t *str_idx);

int64_t expression(dynamic_arr_t *tokens_arr, size_t *arr_idx, 
    status_code_t *rt_code);
int64_t term(dynamic_arr_t *tokens_arr, size_t *arr_idx,
    status_code_t *rt_code);
int64_t factor(dynamic_arr_t *tokens_arr, size_t *arr_idx, 
    status_code_t *rt_code);

int
main(void)
{
    #define CHECK_AND_EXIT(condition, message, input_file, output_file,        \
        tokens_arr, input_str, exit_code)                                      \
        do                                                                     \
        {                                                                      \
            if (condition)                                                     \
            {                                                                  \
                if (message != NULL)                                           \
                    fprintf(stderr, "An Error Occurred: %s\n", (char*)message);\
                if (input_file != NULL)                                        \
                    fclose(input_file);                                        \
                if (output_file != NULL)                                       \
                    fclose(output_file);                                       \
                dynamic_arr_destr(&tokens_arr);                                \
                free(input_str);                                               \
                input_str = NULL;                                              \
                return exit_code;                                              \
            }                                                                  \
        } while (0);                                                           \

    int64_t result = 0;
    size_t arr_idx = 0;
    status_code_t result_calc_code = SUCCESS_CODE;

    FILE *input_file = NULL;
    FILE *output_file = NULL;

    dynamic_arr_t *tokens_arr = NULL;

    dynamic_arr_constr(&tokens_arr, START_CAPACITY);

    char *input_str = NULL;
    size_t input_str_len = 0;

    input_file = fopen("input.txt", "r");

    CHECK_AND_EXIT(input_file == NULL, "Failed to open an input file",
        input_file, output_file, tokens_arr, input_str, EXIT_FAILURE)

    output_file = fopen("output.txt", "w");

    CHECK_AND_EXIT(output_file == NULL, "Failed to open an output file",
        input_file, output_file, tokens_arr, input_str, EXIT_FAILURE)

    ssize_t line_reading_rt_code = getline(
        &input_str, &input_str_len, input_file);

    CHECK_AND_EXIT(line_reading_rt_code == -1, "Failed to read an input line",
        input_file, output_file, tokens_arr, input_str, EXIT_FAILURE);

    status_code_t getting_tokens_code = get_tokens_arr(tokens_arr, input_str);

    CHECK_AND_EXIT(getting_tokens_code != SUCCESS_CODE, NULL,
        input_file, output_file, tokens_arr, input_str, EXIT_FAILURE);

    result = expression(tokens_arr, &arr_idx, &result_calc_code);

    CHECK_AND_EXIT(result_calc_code != SUCCESS_CODE, NULL,
        input_file, output_file, tokens_arr, input_str, EXIT_FAILURE);

    fprintf(output_file, "%ld", result);

    CHECK_AND_EXIT(true, NULL,
        input_file, output_file, tokens_arr, input_str, EXIT_SUCCESS);

    #undef CHECK_AND_EXIT 
}

int64_t
expression(dynamic_arr_t *tokens_arr, size_t *arr_idx, status_code_t *rt_code)
{
    SOFT_ASSERT(rt_code == NULL,
        "rt_code was received as a NULL pointer", 
        INT_ERROR_VALUE);

    SOFT_ASSERT(tokens_arr == NULL,
        "tokens_arr was received as a NULL pointer", 
        *rt_code = NULL_PTR_ARG_CODE COMMA INT_ERROR_VALUE);

    SOFT_ASSERT(arr_idx == NULL,
        "arr_idx was received as a NULL pointer", 
        *rt_code = NULL_PTR_ARG_CODE COMMA INT_ERROR_VALUE);

    status_code_t first_rt_code = SUCCESS_CODE;
    status_code_t second_rt_code = SUCCESS_CODE;

    int64_t first = term(tokens_arr, arr_idx, &first_rt_code);

    SOFT_ASSERT(first_rt_code != SUCCESS_CODE,
        "Error with calculating the first num occurred", 
        *rt_code = first_rt_code COMMA INT_ERROR_VALUE);

    while (*arr_idx < tokens_arr->len)
    {
        enum tokens_types curr_token_type = tokens_arr->ptr[*arr_idx]->type;

        if (curr_token_type != ADD_OPER && curr_token_type != SUB_OPER)
            break;

        ++(*arr_idx);

        int64_t second = term(tokens_arr, arr_idx, &second_rt_code);

        SOFT_ASSERT(second_rt_code != SUCCESS_CODE,
            "Error with calculating the second num occurred",
            *rt_code = second_rt_code COMMA INT_ERROR_VALUE);

        if (curr_token_type == ADD_OPER)
            first += second;

        else
            first -= second;
    }

    return first;
}

int64_t
term(dynamic_arr_t *tokens_arr, size_t *arr_idx, status_code_t *rt_code)
{
    SOFT_ASSERT(rt_code == NULL,
        "rt_code was received as a NULL pointer", 
        INT_ERROR_VALUE);

    SOFT_ASSERT(tokens_arr == NULL,
        "tokens_arr was received as a NULL pointer", 
        *rt_code = NULL_PTR_ARG_CODE COMMA INT_ERROR_VALUE);

    SOFT_ASSERT(arr_idx == NULL,
        "arr_idx was received as a NULL pointer", 
        *rt_code = NULL_PTR_ARG_CODE COMMA INT_ERROR_VALUE);

    status_code_t first_rt_code = SUCCESS_CODE;
    status_code_t second_rt_code = SUCCESS_CODE;

    int64_t first = factor(tokens_arr, arr_idx, &first_rt_code);

    SOFT_ASSERT(first_rt_code != SUCCESS_CODE,
        "Error with calculating the first num occurred",
        *rt_code = first_rt_code COMMA INT_ERROR_VALUE);

    while (*arr_idx < tokens_arr->len)
    {
        enum tokens_types curr_token_type = tokens_arr->ptr[*arr_idx]->type;

        if (curr_token_type != MULT_OPER && curr_token_type != DIVIDE_OPER)
            break;

        ++(*arr_idx);

        int64_t second = factor(tokens_arr, arr_idx, &second_rt_code);

        SOFT_ASSERT(second_rt_code != SUCCESS_CODE,
            "Error with calculating the first num occurred",
            *rt_code = first_rt_code COMMA INT_ERROR_VALUE);

        if (curr_token_type == MULT_OPER)
            first *= second;

        else if (second == 0)
            SOFT_ASSERT(true,
                "Error: division by zero",
                *rt_code = DIVISION_BY_ZERO_CODE COMMA INT_ERROR_VALUE);

        else
            first /= second;
    }

    return first;
}

int64_t
factor(dynamic_arr_t *tokens_arr, size_t *arr_idx, status_code_t *rt_code)
{
    SOFT_ASSERT(rt_code == NULL,
        "rt_code was received as a NULL pointer", 
        INT_ERROR_VALUE);

    SOFT_ASSERT(tokens_arr == NULL,
        "tokens_arr was received as a NULL pointer", 
        *rt_code = NULL_PTR_ARG_CODE COMMA INT_ERROR_VALUE);

    SOFT_ASSERT(arr_idx == NULL,
        "arr_idx was received as a NULL pointer", 
        *rt_code = NULL_PTR_ARG_CODE COMMA INT_ERROR_VALUE);

    token_t *next_token = tokens_arr->ptr[*arr_idx];
    int64_t result = 0;

    status_code_t expr_rt_code = SUCCESS_CODE;

    if (next_token->type == OPEN_PAR)
    {
        ++(*arr_idx);

        result = expression(tokens_arr, arr_idx, &expr_rt_code);

        SOFT_ASSERT(expr_rt_code != SUCCESS_CODE,
            "An error occurred while calculating the result",
            *rt_code = expr_rt_code COMMA INT_ERROR_VALUE);

        enum tokens_types closing_par = CLOSE_PAR;

        SOFT_ASSERT(*arr_idx >= tokens_arr->len,
            "Error: an invalid structure of an input expression",
            *rt_code = INVALID_INPUT_CODE COMMA INT_ERROR_VALUE);

        closing_par = tokens_arr->ptr[*arr_idx]->type;

        if (closing_par == CLOSE_PAR)
        {
            ++(*arr_idx);
            return result;
        }

        SOFT_ASSERT(true,
            "Error: an invalid structure of an input expression",
            *rt_code = INVALID_INPUT_CODE COMMA INT_ERROR_VALUE);
    }

    ++(*arr_idx);

    return next_token->value.number;
}

status_code_t
get_tokens_arr(dynamic_arr_t *tokens_arr, char *str)
{
    SOFT_ASSERT(tokens_arr == NULL,
        "tokens_arr was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(str == NULL,
        "str was received as a NULL pointer", NULL_PTR_ARG_CODE);

    token_t *curr_token = NULL;
    size_t str_idx = 0;

    while (str[str_idx]) 
    {
        if (isspace(str[str_idx])) 
        {
            ++str_idx;
            continue;
        }

        curr_token = calloc(1, sizeof(*curr_token));

        SOFT_ASSERT(curr_token == NULL,
            "allocation for token failed", MEM_ALLOC_ERROR_CODE);

        if (isdigit(str[str_idx])) 
        {
            init_token_as_num(curr_token, str, &str_idx);

            status_code_t adding_rt_code = add_element_to_dynamic_arr(
                tokens_arr, curr_token);

            SOFT_ASSERT(adding_rt_code != SUCCESS_CODE,
                "Failed to add element to dynamic array", adding_rt_code);

            continue; 
        }

        else 
        {
            init_token_as_oper(curr_token, str[str_idx]);

            status_code_t adding_rt_code = add_element_to_dynamic_arr(
                tokens_arr, curr_token);

            SOFT_ASSERT(adding_rt_code != SUCCESS_CODE,
                "Failed to add element to dynamic array", adding_rt_code);
        }

        ++str_idx;
    }

    return SUCCESS_CODE;
}

status_code_t
init_token_as_oper(token_t *token, char oper)
{
    SOFT_ASSERT(token == NULL,
        "token was received as a NULL pointer", NULL_PTR_ARG_CODE);

    switch (oper)
    {
        case '+':
            token->type = ADD_OPER;
            break;

        case '-':
            token->type = SUB_OPER;
            break;

        case '*':
            token->type = MULT_OPER;
            break;

        case '/':
            token->type = DIVIDE_OPER;
            break;

        case '(':
            token->type = OPEN_PAR;
            break;

        case ')':
            token->type = CLOSE_PAR;
            break;

        default:
            break;
    }

    token->value.operator = oper;

    return SUCCESS_CODE;
}

status_code_t init_token_as_num(token_t *token, 
    char *str, size_t *str_idx)
{
    SOFT_ASSERT(token == NULL,
        "token was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(str == NULL,
        "str was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(str_idx == NULL,
        "str_idx was received as a NULL pointer", NULL_PTR_ARG_CODE);

    bool are_leading_zeros = false;
    int64_t rs_num = 0;

    if (str[*str_idx] == '0' && isdigit(str[*str_idx + 1]))
        are_leading_zeros = true;

    while (are_leading_zeros && str[*str_idx] == '0')
        *str_idx += 1;

    while (isdigit(str[*str_idx]))
    {
        rs_num = rs_num * 10 + (str[*str_idx] - '0');
        *str_idx += 1;
    }

    token->value.number = rs_num;
    token->type = NUMBER;

    return SUCCESS_CODE;
}

status_code_t 
dynamic_arr_constr(dynamic_arr_t **d_arr, const size_t start_capacity)
{
    SOFT_ASSERT(d_arr == NULL,
        "ptr to d_arr was received as a NULL poinger", NULL_PTR_ARG_CODE);

    *d_arr = calloc(1, sizeof(**d_arr));

    SOFT_ASSERT(*d_arr == NULL,
        "allocation of dynamic arr failed", MEM_ALLOC_ERROR_CODE);

    (*d_arr)->ptr = calloc(start_capacity, sizeof(*((*d_arr)->ptr)));

    SOFT_ASSERT((*d_arr)->ptr == NULL,
        "allocation of arr inside dynamic arr failed", MEM_ALLOC_ERROR_CODE);

    (*d_arr)->len = 0;
    (*d_arr)->capacity = start_capacity;

    return SUCCESS_CODE;
}

status_code_t 
dynamic_arr_destr(dynamic_arr_t **d_arr)
{
    SOFT_ASSERT(d_arr == NULL,
        "ptr to d_arr was received as a NULL pointer", NULL_PTR_ARG_CODE);

    if (*d_arr == NULL)
        return SUCCESS_CODE;

    for (size_t token_idx = 0; token_idx < (*d_arr)->len; ++token_idx)
    {
        free((*d_arr)->ptr[token_idx]);
        (*d_arr)->ptr[token_idx] = NULL;
    }

    free((*d_arr)->ptr);

    // Poisoning       //
    (*d_arr)->ptr = NULL;
    (*d_arr)->len = 0;
    (*d_arr)->capacity = 0;
    // End of poisoning //

    free(*d_arr);

    *d_arr = NULL;

    return SUCCESS_CODE;
}

status_code_t
realloc_dynamic_arr(dynamic_arr_t *d_arr)
{
    SOFT_ASSERT(d_arr == NULL,
        "d_arr was received as a NULL pointer", NULL_PTR_ARG_CODE);

    token_t **new_ptr = NULL;

    d_arr->capacity <<= 1;

    new_ptr = realloc(d_arr->ptr, sizeof(*new_ptr) * d_arr->capacity);

    SOFT_ASSERT(new_ptr == NULL,
        "reallocation of dynamic arr failed", MEM_ALLOC_ERROR_CODE);

    d_arr->ptr = new_ptr;

    return SUCCESS_CODE;
}

status_code_t 
add_element_to_dynamic_arr(dynamic_arr_t *d_arr, token_t *element)
{
    SOFT_ASSERT(d_arr == NULL,
        "d_arr was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(element == NULL,
        "element was received as a NULL pointer", NULL_PTR_ARG_CODE);

    if (d_arr->len == d_arr->capacity)
    {
        status_code_t reallocation_code = realloc_dynamic_arr(d_arr);

        SOFT_ASSERT(reallocation_code != SUCCESS_CODE,
            "Failed to reallocate dynamic array", MEM_ALLOC_ERROR_CODE);
    }

    d_arr->ptr[d_arr->len++] = element;

    return SUCCESS_CODE;
}
