#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_LEN 26
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

typedef struct TreeNode 
{
    struct TreeNode* children[ALPHABET_LEN];
    int is_common; 
} TreeNode;

TreeNode* tree_create_node(void);
status_code_t insert_substring(
    TreeNode* root, const char* str, size_t start, size_t len);
status_code_t add_substrs_to_tree(TreeNode* root, const char* str);
status_code_t mark_common_substrings(TreeNode* root, const char* str);
int count_common_nodes(TreeNode* node);
void tree_destroy(TreeNode* node);

int main(void) 
{
    char *fst_str = NULL;
    char *sec_str = NULL;

    size_t fst_str_buff = 0, sec_str_buff = 0;
    
    getline(&fst_str, &fst_str_buff, stdin);
    getline(&sec_str, &sec_str_buff, stdin);
    
    fst_str[strcspn(fst_str, "\n")] = '\0';
    sec_str[strcspn(sec_str, "\n")] = '\0';
    
    TreeNode* root = tree_create_node();
    
    if (add_substrs_to_tree(root, fst_str) != SUCCESS_CODE)
        return EXIT_FAILURE;
    
    if (mark_common_substrings(root, sec_str) != SUCCESS_CODE)
        return EXIT_FAILURE;
    
    int common_count = count_common_nodes(root);
    
    printf("%d\n", common_count + 1); 
    
    tree_destroy(root);
    free(fst_str);
    free(sec_str);
    
    return 0;
}

TreeNode*
tree_create_node(void) 
{
    TreeNode* node = calloc(1, sizeof(TreeNode));

    SOFT_ASSERT(node == NULL,
        "Memory allocation for node failed", NULL);

    for (int i = 0; i < ALPHABET_LEN; i++) 
        node->children[i] = NULL;

    node->is_common = 0;

    return node;
}

status_code_t
insert_substring(TreeNode* root, const char* str, size_t start, size_t len) 
{
    SOFT_ASSERT(root == NULL,
        "root was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(str == NULL,
        "str was received as a NULL pointer", NULL_PTR_ARG_CODE);

    TreeNode* curr = root;

    for (int i = start; i < start + len; i++) 
    {
        int index = str[i] - 'a';

        if (curr->children[index] == NULL) 
            curr->children[index] = tree_create_node();

        curr = curr->children[index];
    }

    return SUCCESS_CODE;
}

status_code_t
add_substrs_to_tree(TreeNode* root, const char* str)
{
    SOFT_ASSERT(str == NULL,
        "str was received as a NULL pointer", NULL_PTR_ARG_CODE);

    size_t str_len = strlen(str);

    for (size_t i = 0; i < str_len; i++) 
        for (size_t j = i; j < str_len; j++) 
            insert_substring(root, str, i, j - i + 1);

    return SUCCESS_CODE;
    
}

status_code_t
mark_common_substrings(TreeNode* root, const char* str) 
{
    SOFT_ASSERT(root == NULL,
        "root was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(str == NULL,
        "str was received as a NULL pointer", NULL_PTR_ARG_CODE);

    int str_len = strlen(str);

    for (int idx = 0; idx < str_len; idx++) 
    {
        TreeNode* curr = root;

        for (int j = idx; j < str_len; j++) 
        {
            int index = str[j] - 'a';

            if (curr->children[index] == NULL) 
                break;
            
            curr = curr->children[index];
            curr->is_common = 1; 
        }
    }

    return SUCCESS_CODE;
}

int 
count_common_nodes(TreeNode* node) 
{
    if (node == NULL) 
        return 0;

    int count = node->is_common ? 1 : 0;
    
    for (int idx = 0; idx < ALPHABET_LEN; idx++) 
        if (node->children[idx] != NULL) 
            count += count_common_nodes(node->children[idx]);
    
    return count;
}

void
tree_destroy(TreeNode* node) 
{
    if (node == NULL)
        return;

    for (int idx = 0; idx < ALPHABET_LEN; idx++) 
        tree_destroy(node->children[idx]);
    
    free(node);
}

