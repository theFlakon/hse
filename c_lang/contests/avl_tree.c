#include <stdio.h>
#include <stdlib.h>

typedef struct Node 
{
    int value;
    struct Node* left;
    struct Node* right;
    int height;
} TreeNode;

int
get_height(TreeNode* node)
{
    if (node == NULL)
        return 0;

    return node->height;
}

TreeNode*
create_node(int value)
{
    TreeNode *node = calloc(1, sizeof(TreeNode));

    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->height = 1; 

    return node;
}

int
max(int fst_num, int sec_num) 
{
    return (fst_num > sec_num) ? fst_num : sec_num; 
}

int
get_balance_factor(TreeNode* node)
{
    if (node == NULL)
        return 0;

    return get_height(node->left) - get_height(node->right);
}

TreeNode*
right_rotate(TreeNode* node)
{
    TreeNode* left = node->left;
    TreeNode* right = node->right;

    left->right = node;
    node->left = right;

    node->height = max(get_height(node->left), get_height(node->right)) + 1;
    left->height = max(get_height(left->left), get_height(left->right)) + 1;

    return left;
}

TreeNode*
left_rotate(TreeNode* node)
{
    TreeNode* right = node->right;
    TreeNode* left = right->left;

    right->left = node;
    node->right = left;

    node->height = max(get_height(node->left), get_height(node->right)) + 1;
    right->height = max(get_height(right->left), get_height(right->right)) + 1;

    return right;
}

TreeNode*
insert(TreeNode* node, int value)
{
    if (node == NULL)
        return create_node(value);

    if (value < node->value)
        node->left = insert(node->left, value);

    else if (value > node->value)
        node->right = insert(node->right, value);

    else 
        return node;

    node->height = 1 + max(get_height(node->left), get_height(node->right));

    int balance = get_balance_factor(node);

    if (balance > 1 && value < node->left->value)
        return right_rotate(node);

    if (balance < -1 && value > node->right->value)
        return left_rotate(node);

    if (balance > 1 && value > node->left->value) 
    {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }

    if (balance < -1 && value < node->right->value) 
    {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    return node;
}

// Main function
int main()
{
    struct Node* root = NULL;

    // Inserting nodes
    root = insert(root, 1);
    root = insert(root, 2);
    root = insert(root, 4);
    root = insert(root, 5);
    root = insert(root, 6);
    root = insert(root, 3);

    // Print preorder traversal of the AVL tree
    printf("Inorder traversal of AVL tree: ");
    inOrder(root);

    return 0;
}
