#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef struct node {
    char *val;
    struct node *next;
} Node;

typedef struct {
    Node *head;
    size_t size;
} List;

Node *node_create(char *val);
List* new_lst(void);
Node *get_node(List *lst, size_t index);
void push(List *lst, size_t ind, char *val);

int main(void){
    FILE *f = fopen("words.in", "r");
    FILE *out = fopen("words.out", "w");
    char *str = NULL;
    size_t buf = 0;
    List *lst = new_lst();
    int i = 0;

    getline(&str, &buf, f);
    str[strcspn(str, "\n")] = '\0';
    char *tok = strtok(str, " ");
    while(tok){
        int len = strlen(tok);

        if (len > 0 && tok[len - 1] == '.') {
            tok[len - 1] = '\0';
        }

        push(lst, i, strdup(tok));
        tok = strtok(NULL, " ");
        i++;
    }

    Node *cur = lst->head;
    while(cur != NULL){
        int len = strlen(cur->val);


        if (len && len % 2 != 0 && len > 0) {
            int mid = len / 2;
            memmove(cur->val + mid, cur->val + mid + 1, len - mid);
        }

        if (len && cur->val[0] != '\0'){
            if (cur->next == NULL){
                fprintf(out, "%s", cur->val);
            } else {
                fprintf(out, "%s ", cur->val);
            }
        }

        cur = cur->next;
    }

    fprintf(out, ".");
    return 0;
}

Node *node_create(char *val){
    Node *node = calloc(1, sizeof(*node));
    node->val = val;
    return node;
}

List* new_lst(void) {
    List *lst = calloc(1, sizeof(*lst));
    lst->head = NULL;
    lst->size = 0;
    return lst;
}

Node *get_node(List *lst, size_t index){
    if (index >= lst->size) return NULL;

    Node *node = lst->head;
    for(size_t i = 0; i < index; i++){
        node = node->next;
    }
    return node;
}

void push(List *lst, size_t ind, char *val) {
    if (ind > lst->size) ind = lst->size;
    Node *node = node_create(val);

    if (ind == 0) {
        node->next = lst->head;
        lst->head = node;
    } else {
        Node *prev = get_node(lst, ind - 1);
        if (prev == NULL) prev = get_node(lst, lst->size - 1);
        node->next = prev->next;
        prev->next = node;
    }

    lst->size++;
}
