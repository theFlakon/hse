#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

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

typedef struct s_list_node 
{
    struct s_list_node *next;
    int value;
} ListNode;

typedef struct 
{
    ListNode *head;
    size_t size;
} List;

typedef struct Node 
{
    int value;
    int cnt;
    int height;
    struct Node* left;
    struct Node* right;
} TreeNode;

ListNode* list_node_create(int value); 
status_code_t list_destroy(List **list);

status_code_t parse_input(List *list, FILE *input_file);

TreeNode* tree_create_node(int value);
void tree_destroy(TreeNode *root);

int tree_get_height(TreeNode* node);

int tree_get_balance_factor(TreeNode* node);
TreeNode* tree_right_rotate(TreeNode* node);
TreeNode* tree_left_rotate(TreeNode* node);

TreeNode* tree_insert_node(TreeNode* node, int value);

TreeNode* generate_sorted_tree(List *list);
status_code_t write_sorted_tree_to_file(
    TreeNode *root_tree_node, FILE *output_file);

int max(int fst_num, int sec_num);

int 
main(void)
{
    #define CHECK_AND_EXIT(condition, message,                                 \
        list, root_tree_node, input_file, output_file, return_value)           \
    do                                                                         \
    {                                                                          \
        if (condition)                                                         \
        {                                                                      \
            if (message != NULL)                                               \
                fprintf(stderr, "An error occurred: %s\n", (char*)message);    \
            if (input_file != NULL)                                            \
                fclose(input_file);                                            \
            if (output_file != NULL)                                           \
                fclose(output_file);                                           \
            list_destroy(&list);                                               \
            tree_destroy(root_tree_node);                                      \
            return return_value;                                               \
        }                                                                      \
    } while(0);                                                                \

    List *list = NULL;
    TreeNode *root_tree_node = NULL;

    FILE *input_file = NULL;
    FILE *output_file = NULL;

    double v = sqrt(4);
    printf("%lf\n", v);

    input_file = fopen("input.txt", "r");

    CHECK_AND_EXIT(input_file == NULL, "Failed to open an input file",
        list, root_tree_node, input_file, output_file, EXIT_FAILURE);

    output_file = fopen("output.txt", "w");

    CHECK_AND_EXIT(output_file == NULL, "Failed to open an output file",
        list, root_tree_node, input_file, output_file, EXIT_FAILURE);

    list = calloc(1, sizeof(*list));

    CHECK_AND_EXIT(list == NULL, "Failed to allocate memory for list",
        list, root_tree_node, input_file, output_file, EXIT_FAILURE);
    
    status_code_t parsing_input_code = parse_input(list, input_file);

    CHECK_AND_EXIT(parsing_input_code != SUCCESS_CODE, NULL,
        list, root_tree_node, input_file, output_file, EXIT_FAILURE);

    root_tree_node = generate_sorted_tree(list);

    CHECK_AND_EXIT(root_tree_node == NULL, NULL,
        list, root_tree_node, input_file, output_file, EXIT_FAILURE);

    status_code_t writing_code = write_sorted_tree_to_file(
        root_tree_node, output_file);

    CHECK_AND_EXIT(writing_code != SUCCESS_CODE, NULL,
        list, root_tree_node, input_file, output_file, EXIT_FAILURE);

    CHECK_AND_EXIT(true, NULL,
        list, root_tree_node, input_file, output_file, EXIT_SUCCESS);

    #undef CHECK_AND_EXIT
}

status_code_t 
parse_input(List *list, FILE *input_file)
{
    SOFT_ASSERT(list == NULL,
        "list was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(input_file == NULL,
        "input_file was received as a NULL pointer", NULL_PTR_ARG_CODE);

    ListNode *curr_node = NULL;
    int curr_num = 0;

    if (fscanf(input_file, "%d", &curr_num) == EOF)
        return SUCCESS_CODE;

    list->head = list_node_create(curr_num);

    SOFT_ASSERT(list->head == NULL, 
        "Failed to creade list head", MEM_ALLOC_ERROR_CODE);

    ++list->size;

    curr_node = list->head;

    while (fscanf(input_file, "%d", &curr_num) != EOF)
    {
        curr_node->next = list_node_create(curr_num);
        
        SOFT_ASSERT(curr_node->next == NULL,
            "Failed to create a list node", MEM_ALLOC_ERROR_CODE);
        curr_node = curr_node->next;
        
        ++list->size;
    }

    curr_node->next = list->head;

    return SUCCESS_CODE;
}

TreeNode*
generate_sorted_tree(List *list)
{
    SOFT_ASSERT(list == NULL,
        "list was received as a NULL pointer", NULL);

    ListNode *curr_node = list->head->next;

    int curr_value = list->head->value;

    TreeNode *result_tree = tree_create_node(curr_value);

    SOFT_ASSERT(result_tree == NULL,
        "Failed to create tree", NULL);

    for (size_t idx = 1; idx < list->size; ++idx)
    {
        curr_value = curr_node->value;

        TreeNode *tree_tmp_ptr = tree_insert_node(result_tree, curr_value);

        SOFT_ASSERT(tree_tmp_ptr == NULL,
            "Failed to insert a node to a tree", 
            tree_destroy(result_tree) COMMA NULL);

        result_tree = tree_tmp_ptr;

        curr_node = curr_node->next;
    }

    return result_tree;
}

status_code_t
write_sorted_tree_to_file(TreeNode *tree_node, FILE *output_file)
{
    SOFT_ASSERT(output_file == NULL,
        "output_file was received as a NULL pointer", NULL_PTR_ARG_CODE);

    if (tree_node == NULL)
        return SUCCESS_CODE;

    write_sorted_tree_to_file(tree_node->left, output_file);

    for (size_t idx = 0; idx < tree_node->cnt; ++idx)
        fprintf(output_file, "%d ", tree_node->value);

    write_sorted_tree_to_file(tree_node->right, output_file);

    return SUCCESS_CODE;
}

ListNode*
list_node_create(int value) 
{
    ListNode *node = calloc(1, sizeof(*node));

    SOFT_ASSERT(node == NULL, 
        "Failed to allocate memory for a node", NULL);

    node->value = value;
	node->next = NULL;
    
    return node;
}

status_code_t
list_destroy(List **list)
{
    SOFT_ASSERT(list == NULL,
        "ptr to a list was received as a NULL pointer", NULL_PTR_ARG_CODE);

	if ((*list)->head)
	{
		ListNode *node = (*list)->head;
		
		for (size_t idx = 0; idx < (*list)->size; ++idx)
		{
			ListNode *next = node->next;

            node->value = 0;
            node->next = NULL;

			free(node);
			node = next;
		}
	}
	
	(*list)->head = NULL;
	(*list)->size = 0;
	
	free(*list);
	*list = NULL;

    return SUCCESS_CODE;
}

int
tree_get_height(TreeNode* node)
{
    if (node == NULL)
        return 0;

    return node->height;
}

TreeNode*
tree_create_node(int value)
{
    TreeNode *node = calloc(1, sizeof(TreeNode));

    SOFT_ASSERT(node == NULL,
        "Failed to allocate memory for a node", NULL);

    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->height = 1; 
    node->cnt = 1;

    return node;
}

int
max(int fst_num, int sec_num) 
{
    return (fst_num > sec_num) ? fst_num : sec_num; 
}

int
tree_get_balance_factor(TreeNode* node)
{
    if (node == NULL)
        return 0;

    return tree_get_height(node->left) - tree_get_height(node->right);
}

TreeNode*
tree_right_rotate(TreeNode* node)
{
    SOFT_ASSERT(node == NULL,
        "node was received as a NULL pointer", NULL);

    TreeNode* left = node->left;
    TreeNode* right = left->right;

    left->right = node;
    node->left = right;

    node->height = max(
        tree_get_height(node->left), tree_get_height(node->right)) + 1;
    left->height = max(
        tree_get_height(left->left), tree_get_height(left->right)) + 1;

    return left;
}

TreeNode*
tree_left_rotate(TreeNode* node)
{
    SOFT_ASSERT(node == NULL,
        "node was received as a NULL pointer", NULL);

    TreeNode* right = node->right;
    TreeNode* left = right->left;

    right->left = node;
    node->right = left;

    node->height = max(
        tree_get_height(node->left), tree_get_height(node->right)) + 1;
    right->height = max(
        tree_get_height(right->left), tree_get_height(right->right)) + 1;

    return right;
}

TreeNode*
tree_insert_node(TreeNode* node, int value)
{
    if (node == NULL)
    {
        TreeNode *tmp_node_ptr = tree_create_node(value);

        SOFT_ASSERT(tmp_node_ptr == NULL,
            "Failed to create a node", NULL);

        return tmp_node_ptr;
    }

    if (value < node->value)
    {
        node->left = tree_insert_node(node->left, value);

        SOFT_ASSERT(node->left == NULL, 
            "Failed to insert a node", NULL);
    }

    else if (value > node->value)
    {
        node->right = tree_insert_node(node->right, value);

        SOFT_ASSERT(node->right == NULL,
            "Failed to insert a node", NULL);
    }

    else 
    {
        node->cnt++;
        return node;
    }

    node->height = 1 + max(
        tree_get_height(node->left), tree_get_height(node->right));

    int balance = tree_get_balance_factor(node);

    if (balance > 1 && value < node->left->value)
        return tree_right_rotate(node);

    if (balance < -1 && value > node->right->value)
        return tree_left_rotate(node);

    if (balance > 1 && value > node->left->value) 
    {
        node->left = tree_left_rotate(node->left);
        return tree_right_rotate(node);
    }

    if (balance < -1 && value < node->right->value) 
    {
        node->right = tree_right_rotate(node->right);
        return tree_left_rotate(node);
    }

    return node;
}

void
tree_destroy(TreeNode *node)
{
    if (node == NULL)
        return;

    tree_destroy(node->left);
    tree_destroy(node->right);

    free(node);
}
