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

enum doubles_comp_consts
{
    EQUAL = 0,
    NOT_EQUAL = 1,
};

typedef struct
{
    double *arr;
    size_t rows_cnt;
    size_t cols_cnt;
} matrix_t;

status_code_t matrix_constr(matrix_t **matrix, size_t rows_cnt, size_t cols_cnt);
status_code_t matrix_destr(matrix_t **matrix);

status_code_t parse_input(matrix_t **matrix);

status_code_t get_matrix_element(matrix_t *matrix, size_t row_idx, size_t col_idx,
    double *rt_value);
status_code_t get_matrix_element_ptr(matrix_t *matrix, size_t row_idx, size_t col_idx,
    double **rt_value);
status_code_t set_matrix_element(matrix_t *matrix, size_t row_idx, size_t col_idx,
    double element);

status_code_t add_row_to_another(matrix_t *matrix, 
    size_t fst_row_idx, size_t sec_row_idx, double coeff);
status_code_t swap_matrix_rows(matrix_t *matrix,
    size_t fst_row_idx, size_t sec_row_idx);

status_code_t check_oth_ld_elements_eq_zero(
    matrix_t *matrix, size_t diag_el_idx,
    bool *is_non_zero_found);
status_code_t make_oth_ld_elements_eq_zero(
    matrix_t *matrix, size_t diag_el_idx);

status_code_t transf_matrix_to_step(matrix_t *matrix);

status_code_t find_solutions(matrix_t *matrix, double **result);
status_code_t print_result(double *result_arr, size_t arr_size);

status_code_t swap_doubles(double *fst_var, double *sec_var);
int compare_doubles(double fst_num, double sec_num);

int 
main(void)
{
    #define FREE_MEMORY_AND_EXIT(result, matrix, exit_code)                    \
    do                                                                         \
    {                                                                          \
        free(result);                                                          \
        matrix_destr(&matrix);                                                 \
        return exit_code;                                                      \
    } while(0);                                                                \

    double *result = NULL;
    matrix_t *matrix = NULL;

    if (parse_input(&matrix) != SUCCESS_CODE)
        FREE_MEMORY_AND_EXIT(result, matrix, EXIT_FAILURE);

    if (transf_matrix_to_step(matrix) != SUCCESS_CODE)
        FREE_MEMORY_AND_EXIT(result, matrix, EXIT_FAILURE);

    if (find_solutions(matrix, &result) != SUCCESS_CODE)
        FREE_MEMORY_AND_EXIT(result, matrix, EXIT_FAILURE);

    if (print_result(result, matrix->rows_cnt) != SUCCESS_CODE)
        FREE_MEMORY_AND_EXIT(result, matrix, EXIT_FAILURE);

    FREE_MEMORY_AND_EXIT(result, matrix, EXIT_SUCCESS);

    #undef FREE_MEMORY_AND_EXIT
}

status_code_t
find_solutions(matrix_t *matrix, double **result)
{
    SOFT_ASSERT(matrix == NULL,
        "matrix was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(result == NULL,
        "ptr to result was received as a NULL pointer", NULL_PTR_ARG_CODE);

    size_t result_idx = 0;
    double curr_element = 0.0;

    *result = calloc(matrix->rows_cnt, sizeof(**result));
    
    SOFT_ASSERT(*result == NULL,
        "Memory allocation for result failed", MEM_ALLOC_ERROR_CODE);

    for (ssize_t row_idx = matrix->rows_cnt - 1; row_idx >= 0; --row_idx)
    {
        double free_var = 0.0;

        get_matrix_element(matrix, row_idx, matrix->cols_cnt - 1, &free_var);

        for (ssize_t col_idx = matrix->cols_cnt - 2; col_idx >= row_idx;
             --col_idx)
        {
            get_matrix_element(matrix, row_idx, col_idx, &curr_element);

            if (col_idx == row_idx)
            {
                (*result)[result_idx++] = free_var / curr_element;
                continue;
            }

            free_var -= (*result)[matrix->cols_cnt - col_idx - 2] * curr_element;
        }
    }

    return SUCCESS_CODE;
}

status_code_t
transf_matrix_to_step(matrix_t *matrix)
{
    SOFT_ASSERT(matrix == NULL,
        "matrix was received as a NULL pointer", NULL_PTR_ARG_CODE);

    double diag_element = 0.0;
    size_t diag_el_idx = 0;

    status_code_t checking_rt_code = SUCCESS_CODE;
    status_code_t making_rt_code = SUCCESS_CODE;

    while (diag_el_idx < matrix->rows_cnt)
    {
        bool is_non_zero_found = false;
        get_matrix_element(matrix, diag_el_idx, diag_el_idx, &diag_element);

        if (compare_doubles(diag_element, 0.0) == EQUAL)
        {
            checking_rt_code = check_oth_ld_elements_eq_zero(matrix, 
                diag_el_idx, &is_non_zero_found);

            if (checking_rt_code != SUCCESS_CODE)
                return checking_rt_code;
        }

        else
        {
            making_rt_code = make_oth_ld_elements_eq_zero(matrix, 
                diag_el_idx);

            if (making_rt_code != SUCCESS_CODE)
                return making_rt_code;
        }

        if (!is_non_zero_found)
            diag_el_idx++;
    }

    return SUCCESS_CODE;
}

status_code_t check_oth_ld_elements_eq_zero(
    matrix_t *matrix, size_t diag_el_idx,
    bool *is_non_zero_found)
{
    SOFT_ASSERT(matrix == NULL,
        "matrix was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(is_non_zero_found == NULL,
        "is_non_zero_found was received as a NULL pointer", NULL_PTR_ARG_CODE);

    double curr_element = 0.0;

    for (size_t row_idx = diag_el_idx + 1; 
         row_idx < matrix->rows_cnt; ++row_idx)
    {
        get_matrix_element(matrix, row_idx, diag_el_idx, &curr_element);

        if (compare_doubles(curr_element, 0.0) == NOT_EQUAL)
        {
            swap_matrix_rows(matrix, diag_el_idx, row_idx);
            *is_non_zero_found = true;
            break;
        }
    }

    return SUCCESS_CODE;
}

status_code_t make_oth_ld_elements_eq_zero(
    matrix_t *matrix, size_t diag_el_idx)
{
    SOFT_ASSERT(matrix == NULL,
        "matrix was received as a NULL pointer", NULL_PTR_ARG_CODE);

    double curr_element = 0.0, diag_element = 0.0, coeff = 0.0;

    get_matrix_element(matrix, diag_el_idx, diag_el_idx, &diag_element);

    for (size_t row_idx = diag_el_idx + 1; row_idx < matrix->rows_cnt; ++row_idx)
    {
        get_matrix_element(matrix, row_idx, diag_el_idx, &curr_element);

        if (compare_doubles(curr_element, 0.0) == EQUAL)
            continue;

        coeff = (-curr_element) / diag_element;

        add_row_to_another(matrix, diag_el_idx, row_idx, coeff);
    }

    return SUCCESS_CODE;
}

status_code_t
swap_matrix_rows(matrix_t *matrix, size_t fst_row_idx, size_t sec_row_idx)
{
    SOFT_ASSERT(matrix == NULL,
        "matrix was received as a NULL pointer", NULL_PTR_ARG_CODE);

    double *fst_row_curr_el_ptr = NULL;
    double *sec_row_curr_el_ptr = NULL;

    for (size_t col_idx = 0; col_idx < matrix->cols_cnt; ++col_idx)
    {
        get_matrix_element_ptr(matrix, 
            fst_row_idx, col_idx, &fst_row_curr_el_ptr);

        get_matrix_element_ptr(matrix, 
            sec_row_idx, col_idx, &sec_row_curr_el_ptr);

        swap_doubles(fst_row_curr_el_ptr, sec_row_curr_el_ptr);
    }

    return SUCCESS_CODE;
}

status_code_t
add_row_to_another(matrix_t *matrix, 
    size_t fst_row_idx, size_t sec_row_idx, double coeff)
{
    SOFT_ASSERT(matrix == NULL,
        "matrix was received as a NULL pointer", NULL_PTR_ARG_CODE);

    double *fst_row_curr_el_ptr = NULL;
    double *sec_row_curr_el_ptr = NULL;

    for (size_t col_idx = 0; col_idx < matrix->cols_cnt; ++col_idx)
    {
        get_matrix_element_ptr(matrix,
            fst_row_idx, col_idx, &fst_row_curr_el_ptr);

        get_matrix_element_ptr(matrix,
            sec_row_idx, col_idx, &sec_row_curr_el_ptr);

        *sec_row_curr_el_ptr += coeff * (*fst_row_curr_el_ptr);
    }

    return SUCCESS_CODE;
}

status_code_t
get_matrix_element(matrix_t *matrix, size_t row_idx, size_t col_idx,
    double *rt_value)
{
    SOFT_ASSERT(rt_value == NULL,
        "rt_value was received as a NULL pointer", NULL_PTR_ARG_CODE);

    *rt_value = matrix->arr[row_idx * matrix->cols_cnt + col_idx];

    return SUCCESS_CODE;
}

status_code_t
get_matrix_element_ptr(matrix_t *matrix, size_t row_idx, size_t col_idx,
    double **rt_value)
{
    SOFT_ASSERT(matrix == NULL,
        "matrix was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(rt_value == NULL,
        "ptr to rt_value was received as a NULL pointer", NULL_PTR_ARG_CODE);

    *rt_value = &matrix->arr[row_idx * matrix->cols_cnt + col_idx];

    return SUCCESS_CODE;
}

status_code_t
set_matrix_element(matrix_t *matrix, size_t row_idx, size_t col_idx,
    double element)
{
    SOFT_ASSERT(matrix == NULL,
        "matrix was received as a NULL pointer", NULL_PTR_ARG_CODE);

    matrix->arr[row_idx * matrix->cols_cnt + col_idx] = element;

    return SUCCESS_CODE;
}

status_code_t
swap_doubles(double *fst_var, double *sec_var)
{
    SOFT_ASSERT(fst_var == NULL,
        "fst_var was received as a NULL pointer", NULL_PTR_ARG_CODE);
    SOFT_ASSERT(sec_var == NULL,
        "sec_var was received as a NULL pointer", NULL_PTR_ARG_CODE);

    double tmp_var = *fst_var;

    *fst_var = *sec_var;
    *sec_var = tmp_var;

    return SUCCESS_CODE;
}

int
compare_doubles(double fst_num, double sec_num) 
{
    const double EPS = 0.000001;

    if (fabs(fst_num - sec_num) < EPS)
        return EQUAL;

    return NOT_EQUAL;
}

status_code_t
parse_input(matrix_t **matrix)
{
    SOFT_ASSERT(matrix == NULL,
        "ptr to matrix was received as a NULL pointer", NULL_PTR_ARG_CODE);

    size_t matrix_size = 0;
    double curr_num = 0.0;
    status_code_t constr_rt_code = SUCCESS_CODE;

    SOFT_ASSERT(scanf("%zu", &matrix_size) != 1,
        "Invalid input", NULL_PTR_ARG_CODE);

    constr_rt_code = matrix_constr(matrix, matrix_size, matrix_size + 1);

    if (constr_rt_code != SUCCESS_CODE)
        return constr_rt_code;

    for (size_t row_idx = 0; row_idx < (*matrix)->rows_cnt;
        ++row_idx)
    {
        for (size_t col_idx = 0; col_idx < (*matrix)->cols_cnt;
            ++col_idx)
        {
            SOFT_ASSERT(scanf("%lf", &curr_num) != 1,
                "Invalid input", 
                matrix_destr(matrix) COMMA NULL_PTR_ARG_CODE);

            set_matrix_element(*matrix, row_idx, col_idx, curr_num);
        }
    }

    return SUCCESS_CODE;
}

status_code_t
print_result(double *result_arr, size_t arr_size)
{
    SOFT_ASSERT(result_arr == NULL,
        "result_arr was received as a NULL pointer", NULL_PTR_ARG_CODE);

    for (ssize_t idx = arr_size - 1; idx >= 0; --idx)
        printf("%g\n", round(result_arr[idx]));

    return SUCCESS_CODE;
}

status_code_t
matrix_constr(matrix_t **matrix, size_t rows_cnt, size_t cols_cnt)
{
    SOFT_ASSERT(matrix == NULL,
        "ptr to matrix was received as a NULL pointer", NULL_PTR_ARG_CODE);

    *matrix = calloc(1, sizeof(**matrix));

    SOFT_ASSERT(*matrix == NULL,
        "Memory allocation for matrix failed", MEM_ALLOC_ERROR_CODE);

    (*matrix)->arr = calloc(rows_cnt * cols_cnt, sizeof(double));

    SOFT_ASSERT((*matrix)->arr == NULL,
        "Memory allocation for matrix arr failed", 
        matrix_destr(matrix) COMMA MEM_ALLOC_ERROR_CODE);

    (*matrix)->rows_cnt = rows_cnt;
    (*matrix)->cols_cnt = cols_cnt;

    return SUCCESS_CODE;
}

status_code_t 
matrix_destr(matrix_t **matrix)
{
    SOFT_ASSERT(matrix == NULL,
        "ptr to matrix was received as a NULL pointer", NULL_PTR_ARG_CODE);

    if (*matrix == NULL)
        return SUCCESS_CODE;

    free((*matrix)->arr);

    (*matrix)->arr = NULL;
    (*matrix)->rows_cnt = 0;
    (*matrix)->cols_cnt = 0;

    free(*matrix);
    *matrix = NULL;

    return SUCCESS_CODE;
}
