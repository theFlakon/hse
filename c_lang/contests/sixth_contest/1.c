#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define COMMA ,
#define MAX_LEN 20000

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
    size_t words_cnt;
    size_t sentences_cnt;
    size_t paragraphs_cnt;
} file_stat_t;

status_code_t file_stat_constr(file_stat_t **file_stat);
status_code_t file_stat_destr(file_stat_t **file_stat);

status_code_t parse_input_file(FILE *input_file, 
    file_stat_t *file_stat);
status_code_t write_answ_to_file(file_stat_t *file_stat, FILE *output_file);
bool is_ch_special(int ch);

int
main(void)
{
    #define FREE_MEMORY_AND_EXIT(file_stat, fst_file, sec_file, exit_code)     \
    do                                                                         \
    {                                                                          \
        file_stat_destr(&file_stat);                                           \
        if (fst_file != NULL)                                                  \
            fclose(fst_file);                                                  \
        if (sec_file != NULL)                                                  \
            fclose(sec_file);                                                  \
        return exit_code;                                                      \
    } while(0);                                                                \

    file_stat_t *file_stat = NULL;

    FILE *input_file = fopen("input.txt", "r");

    SOFT_ASSERT(input_file == NULL,
        "Failed to open an input file", EXIT_FAILURE);

    FILE *output_file = fopen("output.txt", "w+");

    SOFT_ASSERT(output_file == NULL,
        "Failed to open an output file", 
        fclose(input_file) COMMA EXIT_FAILURE);

    if (file_stat_constr(&file_stat) != SUCCESS_CODE)
        FREE_MEMORY_AND_EXIT(file_stat, input_file, output_file, EXIT_FAILURE);

    if (parse_input_file(input_file, file_stat) != SUCCESS_CODE)
        FREE_MEMORY_AND_EXIT(file_stat, input_file, output_file, EXIT_FAILURE);

    if (write_answ_to_file(file_stat, output_file) != SUCCESS_CODE)
        FREE_MEMORY_AND_EXIT(file_stat, input_file, output_file, EXIT_FAILURE);

    FREE_MEMORY_AND_EXIT(file_stat, input_file, output_file, EXIT_SUCCESS);

    #undef FREE_MEMORY_AND_EXIT
}

status_code_t 
parse_line(char *line, file_stat_t *file_stat, bool *is_empty_line)
{
    SOFT_ASSERT(line == NULL,
        "line was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(file_stat == NULL,
        "file_stat was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(is_empty_line == NULL,
        "is_empty_line was received as a NULL pointer", NULL_PTR_ARG_CODE);

    int curr_ch = 0;
    bool in_word = false;
    bool is_special = false;
    bool add_word = false;

    *is_empty_line = true;

    for (size_t line_idx = 0; line[line_idx]; ++line_idx)
    {
        curr_ch = line[line_idx];

        is_special = is_ch_special(curr_ch);

        if (is_special)
        {
            if (curr_ch == ' ' && in_word)
                add_word = true;

            else if (curr_ch == '.')
            {
                file_stat->sentences_cnt++;

                if (in_word)
                    add_word = true;
            }

            else if (curr_ch == '\n' && in_word && line[line_idx - 1] != '-')
                add_word = true;

            if (add_word)
            {
                file_stat->words_cnt++;
                in_word = false;
                add_word = false;
            }
        }

        else
        {
            *is_empty_line = false;
            in_word = true;
        }
    }

    return SUCCESS_CODE;
}

status_code_t
parse_input_file(FILE *input_file, file_stat_t *file_stat)
{
    SOFT_ASSERT(input_file == NULL,
        "input_file was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(file_stat == NULL,
        "file_stat was received as a NULL pointer", NULL_PTR_ARG_CODE);

    char curr_line_buffer[MAX_LEN + 1] = {0};

    bool is_empty_line = false;
    bool was_lst_line_empty = false;
    bool par_added = false;

    while(fgets(curr_line_buffer, MAX_LEN + 1, input_file) != NULL)
    {
        parse_line(curr_line_buffer, file_stat, &is_empty_line);

        if (is_empty_line && !par_added && file_stat->words_cnt)
        {
            file_stat->paragraphs_cnt++;
            par_added = true;
        }

        else if (!is_empty_line)
            par_added = false;

        if (is_empty_line)
            was_lst_line_empty = true;

        else 
            was_lst_line_empty = false;
    }

    if (file_stat->words_cnt && !was_lst_line_empty)
        file_stat->paragraphs_cnt++;

    return SUCCESS_CODE;
}

bool
is_ch_special(int ch)
{
    return (ch == ' ' || ch == '\n' || ch == '-' || ch == '.');
}

status_code_t
write_answ_to_file(file_stat_t *file_stat, FILE *output_file)
{
    SOFT_ASSERT(file_stat == NULL,
        "file_stat was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(output_file == NULL,
        "output_file was received as a NULL pointer", NULL_PTR_ARG_CODE);

    fprintf(output_file, "%zu %zu %zu",
        file_stat->words_cnt, file_stat->sentences_cnt, 
        file_stat->paragraphs_cnt);

    return SUCCESS_CODE;
}

status_code_t 
file_stat_constr(file_stat_t **file_stat)
{
    SOFT_ASSERT(file_stat == NULL,
        "ptr to file_stat was received as a NULL pointer", NULL_PTR_ARG_CODE);

    *file_stat = calloc(1, sizeof(**file_stat));

    SOFT_ASSERT(*file_stat == NULL,
        "Memory allocation for file_stat failed", MEM_ALLOC_ERROR_CODE);

    (*file_stat)->words_cnt = 0;
    (*file_stat)->sentences_cnt = 0;
    (*file_stat)->paragraphs_cnt = 0;

    return SUCCESS_CODE;
}

status_code_t
file_stat_destr(file_stat_t **file_stat)
{
    SOFT_ASSERT(file_stat == NULL,
        "ptr to file_stat was received as a NULL pointer", NULL_PTR_ARG_CODE);

    if (*file_stat == NULL)
        return SUCCESS_CODE;

    (*file_stat)->words_cnt = 0;
    (*file_stat)->sentences_cnt = 0;
    (*file_stat)->paragraphs_cnt = 0;

    free(*file_stat);
    *file_stat = NULL;

    return SUCCESS_CODE;
}
