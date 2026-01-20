#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

typedef struct list_t
{
    int val;
    struct list_t *next_node;
} list_t;

status_code_t list_constr(list_t **head_node);
status_code_t list_destr(list_t **head_node);
status_code_t list_add_node(list_t **last_node, int val);
status_code_t list_check_element(list_t *head_node, int element,
        bool *is_in_list);

status_code_t write_answer_to_file(list_t *fst_head, list_t *sec_head,
    FILE *output_file);

status_code_t get_list(list_t **head, FILE *input_file);

int 
main(void)
{
    #define FREE_MEMORY_AND_EXIT(fst_head, sec_head,                           \
            fst_file, sec_file, exit_code)                                     \
        do {                                                                   \
            list_destr(&fst_head);                                             \
            list_destr(&sec_head);                                             \
            if (fst_file != NULL)                                              \
                fclose(fst_file);                                              \
            if (sec_file != NULL)                                              \
                fclose(sec_file);                                              \
            return exit_code;                                                  \
        } while(0);                                                            \

    list_t *fst_head = NULL;
    list_t *sec_head = NULL;

    FILE *input_file = fopen("input.txt", "r");
    FILE *output_file = fopen("output.txt", "w+");

    SOFT_ASSERT(input_file == NULL,
        "Couldn't open an input file", EXIT_FAILURE);

    SOFT_ASSERT(output_file == NULL,
        "Couldn't open an output file", EXIT_FAILURE);

    if (list_constr(&fst_head) != SUCCESS_CODE)
        FREE_MEMORY_AND_EXIT(fst_head, sec_head, input_file, output_file, 
            EXIT_FAILURE);

    if (list_constr(&sec_head) != SUCCESS_CODE)
        FREE_MEMORY_AND_EXIT(fst_head, sec_head, input_file, output_file, 
            EXIT_FAILURE);

    if (get_list(&fst_head, input_file) != SUCCESS_CODE)
        FREE_MEMORY_AND_EXIT(fst_head, sec_head, input_file, output_file, 
            EXIT_FAILURE);

    if (get_list(&sec_head, input_file) != SUCCESS_CODE)
        FREE_MEMORY_AND_EXIT(fst_head, sec_head, input_file, output_file, 
            EXIT_FAILURE);

    if (write_answer_to_file(fst_head, sec_head, output_file) != SUCCESS_CODE)
        FREE_MEMORY_AND_EXIT(fst_head, sec_head, input_file, output_file, 
            EXIT_FAILURE);

    FREE_MEMORY_AND_EXIT(fst_head, sec_head, 
        input_file, output_file, EXIT_SUCCESS);

    #undef FREE_MEMORY_AND_EXIT
}

status_code_t 
write_answer_to_file(list_t *fst_head, list_t *sec_head, FILE *output_file)
{
    SOFT_ASSERT(fst_head == NULL,
        "fst_head was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(sec_head == NULL,
        "sec_head was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(output_file == NULL,
        "output_file was received as a NULL pointer", NULL_PTR_ARG_CODE);

    bool is_in_sec_head = true;

    while (fst_head != NULL)
    {
        list_check_element(sec_head, fst_head->val, &is_in_sec_head);

        if (!is_in_sec_head)
            fprintf(output_file, "%d ", fst_head->val);

        fst_head = fst_head->next_node;
    }

    return SUCCESS_CODE;
}

status_code_t 
list_check_element(list_t *head, int element, bool *is_in_list)
{
    SOFT_ASSERT(head == NULL,
        "head was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(is_in_list == NULL,
        "is_in_list was received as a NULL pointer", NULL_PTR_ARG_CODE);

    while (head != NULL)
    {
        if (head->val == element)
        {
            *is_in_list = true;
            return EXIT_SUCCESS;
        }

        head = head->next_node;
    }

    *is_in_list = false;

    return EXIT_SUCCESS;
}

status_code_t
get_list(list_t **head, FILE *input_file)
{
    SOFT_ASSERT(head == NULL,
        "head was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(input_file == NULL,
        "input_file was received as a NULL pointer", NULL_PTR_ARG_CODE);

    int curr_num = 0;
    status_code_t adding_list_node_rt_code = SUCCESS_CODE;

    list_t *head_cpy = *head;
    
    SOFT_ASSERT(fscanf(input_file, "%d", &curr_num) != 1,
        "Invalid input data", INVALID_INPUT_CODE);

    (*head)->val = curr_num;

    while (curr_num != -1)
    {
        SOFT_ASSERT(fscanf(input_file, "%d", &curr_num) != 1, 
            "Invalid input data",
            list_destr(head) COMMA INVALID_INPUT_CODE);

        if (curr_num != -1)
        {
            adding_list_node_rt_code = list_add_node(&head_cpy, curr_num);

            if (adding_list_node_rt_code != SUCCESS_CODE)
                return adding_list_node_rt_code;
        }
    }

    return SUCCESS_CODE;
}

status_code_t 
list_add_node(list_t **last_node, int val)
{
    (*last_node)->next_node = calloc(1, sizeof(list_t));

    SOFT_ASSERT((*last_node)->next_node == NULL,
        "Memory allocation for list node failed", MEM_ALLOC_ERROR_CODE);

    *last_node = (*last_node)->next_node;

    (*last_node)->val = val;
    (*last_node)->next_node = NULL;

    return SUCCESS_CODE;
}

status_code_t
list_constr(list_t **head)
{
    SOFT_ASSERT(head == NULL,
        "ptr to head was received as a NULL pointer", NULL_PTR_ARG_CODE);

    *head = calloc(1, sizeof(**head));

    SOFT_ASSERT(*head == NULL,
        "memory allocation for head failed", MEM_ALLOC_ERROR_CODE);

    (*head)->val = 0;
    (*head)->next_node = NULL;

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

        curr_node->val = 0;
        free(curr_node);

        curr_node = next_node;
    }

    *head = NULL;

    return SUCCESS_CODE;
}
