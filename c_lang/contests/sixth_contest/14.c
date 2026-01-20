#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>

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

status_code_t swap_num_endian(void *num, size_t bytes_cnt);
status_code_t parse_input_create_matrix(
    int32_t **matrix, uint16_t *matrix_size, FILE *input_file);
status_code_t parse_input_calc_trase(
    int32_t *matrix, int64_t *trace, uint16_t matrix_size, FILE *input_file);

int
main(void) 
{
    #define CHECK_AND_EXIT(condition, message,                                 \
        input_file, output_file, matrix, exit_code)                            \
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
            free(matrix);                                                      \
            return exit_code;                                                  \
        }                                                                      \
    } while(0);                                                                \

    FILE *input_file = NULL;
    FILE *output_file = NULL;

    int32_t *matrix = NULL;
    int64_t trace = 0;

    uint16_t matrix_size = 0;

    input_file = fopen("matrix.in", "rb");

    CHECK_AND_EXIT(input_file == NULL, "Failed to open an input file",
        input_file, output_file, matrix, EXIT_FAILURE);

    output_file = fopen("trace.out", "wb");

    CHECK_AND_EXIT(output_file == NULL, "Failed to open an output file",
        input_file, output_file, matrix, EXIT_FAILURE);

    status_code_t creating_matrix_code = parse_input_create_matrix(
        &matrix, &matrix_size, input_file);

    CHECK_AND_EXIT(creating_matrix_code != SUCCESS_CODE, NULL,
        input_file, output_file, matrix, EXIT_FAILURE);

    status_code_t calculating_trace_code = parse_input_calc_trase(
        matrix, &trace, matrix_size, input_file);

    CHECK_AND_EXIT(calculating_trace_code != SUCCESS_CODE, NULL,
        input_file, output_file, matrix, EXIT_FAILURE);

    size_t writing_code = fwrite(&trace, sizeof(trace), 1, output_file);

    CHECK_AND_EXIT(writing_code == 0, 
        "Failed to write the result to the output file",
        input_file, output_file, matrix, EXIT_FAILURE);

    CHECK_AND_EXIT(true, NULL,
        input_file, output_file, matrix, EXIT_SUCCESS);

    #undef CHECK_AND_EXIT
}

status_code_t
parse_input_calc_trase(
    int32_t *matrix, int64_t *trace, uint16_t matrix_size, FILE *input_file)
{
    SOFT_ASSERT(matrix == NULL,
        "matrix was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(trace == NULL,
        "trace was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(input_file == NULL,
        "input_file was received as a NULL pointer", NULL_PTR_ARG_CODE);

    *trace = 0; 

    size_t reading_code = fread(
        matrix, sizeof(int32_t), matrix_size * matrix_size, input_file);

    SOFT_ASSERT(reading_code != matrix_size * matrix_size,
        "Failed to read the data from the input_file", INVALID_INPUT_CODE);

    for (size_t row_idx = 0; row_idx < matrix_size; ++row_idx)
    {
        int32_t curr_num = matrix[row_idx * matrix_size + row_idx];

        swap_num_endian(&curr_num, sizeof(curr_num));

        *trace += curr_num;
    }

    swap_num_endian(trace, sizeof(*trace));

    return SUCCESS_CODE;
}

status_code_t
parse_input_create_matrix(
    int32_t **matrix, uint16_t *matrix_size, FILE *input_file)
{
    SOFT_ASSERT(matrix == NULL,
        "ptr to matrix was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(matrix_size == NULL,
        "matrix_size was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(input_file == NULL,
        "input_file was received as a NULL pointer", NULL_PTR_ARG_CODE);

    size_t reading_code = fread(
        matrix_size, sizeof(*matrix_size), 1, input_file);

    SOFT_ASSERT(reading_code == 0,
        "Failed to read the data from the input file", INVALID_INPUT_CODE);

    swap_num_endian(matrix_size, sizeof(*matrix_size));

    *matrix = calloc((*matrix_size) * (*matrix_size), sizeof(int32_t));

    SOFT_ASSERT(*matrix == NULL,
        "Failed to allocate memory for matrix", MEM_ALLOC_ERROR_CODE);

    return SUCCESS_CODE;
}

status_code_t
swap_num_endian(void *num, size_t bytes_cnt)
{
    SOFT_ASSERT(num == NULL,
        "num was received as a NULL pointer", NULL_PTR_ARG_CODE);

    for (size_t byte_idx = 0; byte_idx < bytes_cnt / 2; ++byte_idx)
    {
        char tmp = *((char *)num + byte_idx);

        *((char *)num + byte_idx) = *((char *)num + bytes_cnt - byte_idx - 1);

        *((char *)num + bytes_cnt - byte_idx - 1) = tmp;
    }

    return SUCCESS_CODE;
}
