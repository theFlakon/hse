#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define COMMA ,
#define MAX_LEN 1000000

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

typedef struct
{
    char *ptr;
    size_t len;
} string_t;

typedef struct list_t
{
    struct list_t *next_node;
    string_t *string;
} list_t;

status_code_t list_destr(list_t **head_node);
status_code_t list_add_node(list_t *head, string_t *string);
status_code_t list_truncate_mid_strs(list_t *head);

status_code_t string_constr(string_t **string, 
    char *str_buffer, size_t str_len);
status_code_t string_destr(string_t **string);
status_code_t string_truncate_mid(string_t *string);

status_code_t get_strs_list(list_t *head_node, FILE *input_file);
status_code_t write_answ_to_file(list_t *head_node, FILE *output_file);

int 
main(void)
{
    #define FREE_MEMORY_AND_EXIT(fst_file, sec_file, head_node, exit_code)     \
        do                                                                     \
        {                                                                      \
            if (fst_file != NULL)                                              \
                fclose(fst_file);                                              \
            if (sec_file != NULL)                                              \
                fclose(sec_file);                                              \
            list_destr(&head_node);                                            \
            return exit_code;                                                  \
        } while (0)                                                            \

    list_t pre_head_node;
    pre_head_node.next_node = NULL;

    list_t *head_node = NULL;
    
    FILE *input_file = fopen("words.in", "r");
    FILE *output_file = fopen("words.out", "w");

    SOFT_ASSERT(input_file == NULL, 
        "Couldn't open an input file", EXIT_FAILURE);

    SOFT_ASSERT(output_file == NULL,
        "Couldn't open an output file", EXIT_FAILURE);

    if (get_strs_list(&pre_head_node, input_file) != SUCCESS_CODE)
        FREE_MEMORY_AND_EXIT(input_file, output_file, 
            head_node, EXIT_FAILURE);

    head_node = pre_head_node.next_node;

    if (list_truncate_mid_strs(head_node) != SUCCESS_CODE) 
        FREE_MEMORY_AND_EXIT(input_file, output_file,
            head_node, EXIT_FAILURE);

    if (write_answ_to_file(head_node, output_file) != SUCCESS_CODE)
        FREE_MEMORY_AND_EXIT(input_file, output_file,
            head_node, EXIT_FAILURE);

    FREE_MEMORY_AND_EXIT(input_file, output_file, head_node, EXIT_SUCCESS);

    #undef FREE_MEMORY_AND_EXIT
}

status_code_t
get_strs_list(list_t *head_node, FILE *input_file)
{
    #define FREE_MEMORY_AND_RETURN(head_node, word, return_val) \
        do \
        { \
            list_destr(&head_node->next_node); \
            head_node->next_node = NULL; \
            string_destr(&word); \
            return return_val; \
        } while(0); \

    SOFT_ASSERT(head_node == NULL,
        "head_node was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(input_file == NULL,
        "input_file was received as a NULL pointer", NULL_PTR_ARG_CODE);

    char word_buffer[MAX_LEN + 1] = {0};
    char curr_ch = fgetc(input_file);

    size_t word_buffer_idx = 0;

    list_t *curr_node = head_node;
    string_t *curr_word = NULL;

    status_code_t str_constr_rt_code = SUCCESS_CODE;
    status_code_t adding_node_rt_code = SUCCESS_CODE;

    while (curr_ch != '.')
    {
        while (curr_ch != ' ' && curr_ch != '.')
        {
            word_buffer[word_buffer_idx++] = curr_ch;
            curr_ch = fgetc(input_file);
        }

        str_constr_rt_code = string_constr(&curr_word, word_buffer, 
            word_buffer_idx);

        if (str_constr_rt_code != SUCCESS_CODE)
            FREE_MEMORY_AND_RETURN(head_node, curr_word, str_constr_rt_code);

        adding_node_rt_code = list_add_node(curr_node, curr_word);

        if (adding_node_rt_code != SUCCESS_CODE)
            FREE_MEMORY_AND_RETURN(head_node, curr_word, adding_node_rt_code);

        curr_node = curr_node->next_node;
        curr_word = NULL;

        word_buffer_idx = 0;

        curr_ch = (curr_ch == '.') ? curr_ch : fgetc(input_file);
    }

    return SUCCESS_CODE;

    #undef FREE_MEMORY_AND_RETURN
}

status_code_t
write_answ_to_file(list_t *head_node, FILE *output_file)
{
    SOFT_ASSERT(head_node == NULL,
        "head_node was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(output_file == NULL,
        "output_file was received as a NULL pointer", NULL_PTR_ARG_CODE);

    bool was_fst = false;

    while (head_node != NULL)
    {
        if (was_fst && head_node->string->len > 1)
            fprintf(output_file, " ");

        fprintf(output_file, "%s", head_node->string->ptr);
        head_node = head_node->next_node;

        was_fst = true;
    }

    fprintf(output_file, ".");

    return SUCCESS_CODE;
}

status_code_t
list_truncate_mid_strs(list_t *head)
{
    SOFT_ASSERT(head == NULL,
        "head was received as a NULL pointer", NULL_PTR_ARG_CODE);

    status_code_t truncate_rt_code = SUCCESS_CODE;

    while (head != NULL)
    {
        truncate_rt_code = string_truncate_mid(head->string);

        if (truncate_rt_code != SUCCESS_CODE)
            return truncate_rt_code;

        head = head->next_node;
    }

    return SUCCESS_CODE;
}

status_code_t
string_truncate_mid(string_t *string)
{
    SOFT_ASSERT(string == NULL,
        "string was received as a NULL pointer", NULL_PTR_ARG_CODE);

    if (string->len % 2 == 0)
        return SUCCESS_CODE;

    if (string->len == 1)
    {
        string->ptr[0] = '\0';
        return SUCCESS_CODE;
    }

    for (size_t idx = (string->len / 2) + 1; idx < string->len; ++idx)
    {
        string->ptr[idx - 1] = string->ptr[idx];
        string->ptr[idx] = '\0';
    }

    return SUCCESS_CODE;
}

status_code_t 
list_add_node(list_t *head, string_t *string)
{
    SOFT_ASSERT(head == NULL,
        "head was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(string == NULL,
        "string was received as a NULL pointer", NULL_PTR_ARG_CODE);

    while (head->next_node != NULL)
        head = head->next_node;

    head->next_node = calloc(1, sizeof(list_t));

    SOFT_ASSERT(head->next_node == NULL,
        "Memory allocation for next node failed", MEM_ALLOC_ERROR_CODE);

    head = head->next_node;

    head->string = string;
    head->next_node = NULL;

    return SUCCESS_CODE;
}

status_code_t
list_destr(list_t **head)
{
    SOFT_ASSERT(head == NULL,
        "ptr to head was received as a NULL pointer", NULL_PTR_ARG_CODE);

    list_t *curr_node = *head;
    list_t *next_node = NULL;

    while (curr_node != NULL)
    {
        next_node = curr_node->next_node;

        string_destr(&curr_node->string);
        curr_node->string = NULL;

        free(curr_node);

        curr_node = next_node;
    }

    *head = NULL;

    return SUCCESS_CODE;
}

status_code_t
string_constr(string_t **string, char *str_buffer, size_t str_len)
{
    SOFT_ASSERT(string == NULL,
        "ptr to string was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(str_buffer == NULL,
        "str_buffer was received as a NULL pointer", NULL_PTR_ARG_CODE);

    *string = calloc(1, sizeof(**string));

    SOFT_ASSERT(*string == NULL,
        "Memory allocation for string failed", MEM_ALLOC_ERROR_CODE);

    (*string)->ptr = strndup(str_buffer, str_len);

    SOFT_ASSERT((*string)->ptr == NULL,
        "Memory allocation using strndup failed", 
        free(*string) COMMA *string = NULL COMMA MEM_ALLOC_ERROR_CODE);

    (*string)->len = str_len;

    return SUCCESS_CODE;
}

status_code_t 
string_destr(string_t **string)
{
    SOFT_ASSERT(string == NULL,
        "ptr to string was received as a NULL pointer", NULL_PTR_ARG_CODE);

    if (*string == NULL)
        return SUCCESS_CODE;

    free((*string)->ptr);

    (*string)->ptr = NULL;
    (*string)->len = 0;

    free(*string);

    *string = NULL;

    return SUCCESS_CODE;
}
