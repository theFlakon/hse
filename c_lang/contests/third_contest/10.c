#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define COMMA ,

#ifdef LOG_ALL
    #define SOFT_ASSERT(condition, error_message, return_value) \
        do { \
            if (condition) { \
                fprintf(stderr, "Soft assertion failed: %s\n", error_message); \
                fprintf(stderr, "Condition: %s\n", #condition); \
                fprintf(stderr, "File: %s, Line: %d\n", __FILE__, __LINE__); \
                return return_value; \
            } \
        } while(0)
#else
#define SOFT_ASSERT(condition, error_message, return_value) \
    do { \
        if (condition) { \
            fprintf(stderr, "An Error occurred: %s\n", error_message); \
            return return_value; \
        } \
    } while(0)
#endif

typedef enum status_codes 
{
    INVALID_INPUT_CODE = -1,
    MEM_ALLOC_ERROR_CODE = -2,
    NULL_PTR_ARG_CODE = -3,
    SUCCESS_CODE = 1,
} status_code_t; 

typedef struct
{
    char operator;
    int32_t num;
} input_data_t;

status_code_t parse_input(input_data_t *input_data);
status_code_t init_input_data(input_data_t *input_data);
int32_t prefix_polish_not(input_data_t *curr_input);

int
main(void)
{
    int32_t result = 0;

    input_data_t curr_input;

    init_input_data(&curr_input);

    if (parse_input(&curr_input) != SUCCESS_CODE) 
        return EXIT_FAILURE;

    result = prefix_polish_not(&curr_input);

    printf("%d\n", result);
    
    return EXIT_SUCCESS;
}

int32_t
prefix_polish_not(input_data_t *curr_input)
{
    const int ERROR_VALUE = 0; // Func prints -0 in case of an error

    SOFT_ASSERT(curr_input == NULL,
        "curr_input was received as a NULL pointer", ERROR_VALUE);

    if (!curr_input->operator)
    {
        return curr_input->num;
    }

    int32_t fst_num = 0, sec_num = 0, result = 0;

    input_data_t next_input;

    init_input_data(&next_input);

    SOFT_ASSERT(parse_input(&next_input) != SUCCESS_CODE,
        "Invalid input", putc('-', stdout) COMMA ERROR_VALUE);

    fst_num = prefix_polish_not(&next_input);

    SOFT_ASSERT(parse_input(&next_input) != SUCCESS_CODE, 
        "Invalid input", putc('-', stdout) COMMA ERROR_VALUE);
    
    sec_num = prefix_polish_not(&next_input);

    SOFT_ASSERT(curr_input->operator == '/' && sec_num == 0,
        "Division by zero", putc('-', stdout) COMMA ERROR_VALUE);

    result = (curr_input->operator == '*') ? fst_num * sec_num :
        fst_num / sec_num;

    return result;
}

status_code_t
init_input_data(input_data_t *input_data)
{
    SOFT_ASSERT(input_data == NULL,
        "input_data was received as a NULL pointer", NULL_PTR_ARG_CODE);

    input_data->operator = 0;
    input_data->num = 0;

    return SUCCESS_CODE;
}

status_code_t
parse_input(input_data_t *input_data)
{
    SOFT_ASSERT(input_data == NULL,
        "input_data was received as a NULL pointer", NULL_PTR_ARG_CODE);

    if (scanf("%d", &input_data->num) == 1)
    {
        input_data->operator = 0;  
        return SUCCESS_CODE;
    }
    
    if (scanf(" %c", &input_data->operator) == 1)
    {
        SOFT_ASSERT(input_data->operator != '/' && 
            input_data->operator != '*',"Invalid operator", INVALID_INPUT_CODE);

        return SUCCESS_CODE;
    }
    
    SOFT_ASSERT(true, "Failed to read input", INVALID_INPUT_CODE);
}
