#include <stdio.h>

typedef enum status_codes 
{
    INVALID_INPUT_CODE = -1,
    MEM_ALLOC_ERROR_CODE = -2,
    NULL_PTR_ARG_CODE = -3,
    SUCCESS_CODE = 1,
} status_code_t; 

typedef enum token_type
{
    ADDITION_SIGN = 1, 
    SUBTRACTION_SIGN = 2,
} token_type_t;

typedef struct tree_node
{
    int value;
    struct TreeNode *left;
    struct TreeNode *right;
} tree_node_t;

typedef struct token
{
    
}token_t;

status_code_t
parse(const char *expression)
