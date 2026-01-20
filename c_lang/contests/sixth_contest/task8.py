#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define COMMA ,
#define MAX_STRING_BUFFER 1000000
#define SOFT_ASSERT(condition, error_message, return_value)      \
    {                                                            \
        if (!(condition)) {                                      \
            printf("%s\n", error_message);                       \
            return return_value;                                 \
        }                                                        \
    }                                                            \

typedef struct Element {
    char *data;
    size_t length;
    struct Element *next;
} Element;

void read_string(FILE *file, Element **head);
void create_element(Element *new_element, char *data, size_t length);
void append_element(Element **head, char *data, size_t length);
void free_string(Element *head);
void remove_middle_character(Element *element);
void process_and_output(FILE *output_file, Element *head);

int main(void) {
    FILE *input_file = fopen("./words.in", "r");
    SOFT_ASSERT(input_file != NULL, "Failed to read file", EXIT_FAILURE);
    
    Element *string = NULL;
    read_string(input_file, &string);
    fclose(input_file);

    FILE* output_file = fopen("./words.out", "w");
    SOFT_ASSERT(output_file != NULL, "Failed to read file", EXIT_FAILURE);

    process_and_output(output_file, string);

    free_string(string);
    fclose(output_file);

    return EXIT_SUCCESS;
}

void create_element(Element *new_element, char *data, size_t length) {
    SOFT_ASSERT(new_element != NULL, "Invalid new_element pointer", (void)0);
    SOFT_ASSERT(data != NULL, "Invalid data", (void)0);

    new_element->data = calloc(length + 1, sizeof(char));
    strncpy(new_element->data, data, length);
    new_element->length = length;
    new_element->next = NULL;
}

void
list_add_node(Element *node, char *buffer, size_t len)
{
    node->next = calloc(1, sizeof(Element));

    node = node->next;

    node->data = strndup(buffer, len);
    node->length = len;
    node->next = NULL;
}

void read_string(FILE *file, Element **head) {
    SOFT_ASSERT(file != NULL, "Invalid file pointer", (void)0);
    SOFT_ASSERT(head != NULL, "Invalid head pointer", (void)0);

    char curr_ch = fgetc(file);

    Element *curr_element = *head;

    char buffer[MAX_STRING_BUFFER] = {0};
    size_t buffer_idx = 0;

    while (curr_ch != '.')
    {
        while (curr_ch != ' ' && curr_ch != '.')
        {
            buffer[buffer_idx++] = curr_ch;
            curr_ch = fgetc(file);
        }

        append_element(head, buffer, buffer_idx);

        curr_ch = (curr_ch == '.') ? curr_ch : fgetc(file);

        if (curr_ch == 0)
            append_element(head, ".", 1);

    }
 
    while (fscanf(file, "%s", buffer) == 1) {
        size_t length = strlen(buffer);
        
        if (buffer[length - 1] == '.') {
            append_element(head, buffer, length - 1);
            append_element(head, ".", 1);
            break;
        } else {
            append_element(head, buffer, length);
        }
    }
}


void free_string(Element *head) {
    Element *current = head;

    while (current != NULL) {
        Element *temp = current;
        current = current->next;
        free(temp->data);
        free(temp);
    }
}

void append_element(Element **head, char *data, size_t length) {
    SOFT_ASSERT(data != NULL, "Invalid data", (void)0);
    SOFT_ASSERT(head != NULL, "Invalid head pointer", (void)0);

    Element *new_element = calloc(1, sizeof(Element));
    SOFT_ASSERT(new_element != NULL, "Memory allocation failed", (void)0);

    create_element(new_element, data, length);

    if (*head == NULL) {
        *head = new_element;
        return;
    }
    
    Element *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_element;
}

void remove_middle_character(Element *element) {
    SOFT_ASSERT(element != NULL, "Invalid element pointer", (void)0);

    if (element == NULL || element->length % 2 == 0) return;
    
    int middle = element->length / 2;
    char *new_string = calloc(element->length, sizeof(char));
    
    int result_string_word_index = 0;
    for (int string_word_index = 0; string_word_index < element->length; string_word_index++) {
        if (string_word_index != middle) {
            new_string[result_string_word_index++] = element->data[string_word_index];
        }
    }

    new_string[result_string_word_index] = '\0';

    free(element->data);
    element->data = new_string;
    element->length--;
}

void process_and_output(FILE *output_file, Element *head) {
    SOFT_ASSERT(output_file != NULL, "Invalid file pointer", (void)0);
    SOFT_ASSERT(head != NULL, "Invalid head pointer", (void)0);

    Element *current = head;
    
    while (current != NULL) {
        if (current->length > 0) {
            if (current->length % 2 != 0 && current->data[0] != '.') {
                remove_middle_character(current);
            }
        }
        
        if (current->length > 0) {
            if (current != head && current->data[0] != '.') {
                fprintf(output_file, " ");
            }

            fprintf(output_file, "%s", current->data);
        }

        if (current->data[0] == '.') {
            break;
        }
        
        current = current->next;
    }
}
