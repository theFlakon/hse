#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#ifdef LOG_ALL
    #define SOFT_ASSERT(condition, error_message, return_value) \
        do { \
            if (condition) { \
                fprintf(stderr, "Soft assertion failed: %s\n", error_message); \
                fprintf(stderr, "Condition: %s\n", #condition); \
                fprintf(stderr, "File: %s, Line: %d\n", __FILE__, __LINE__); \
                return return_value; \
            } \
        } while(0)
#else
#define SOFT_ASSERT(condition, error_message, return_value) \
    do { \
        if (condition) { \
            fprintf(stderr, "An Error occurred: %s\n", error_message); \
            return return_value; \
        } \
    } while(0)
#endif

typedef struct
{
    unsigned fst_num;
    unsigned sec_num;
    unsigned third_num;
    uint64_t mod;
} input_data_t;

typedef enum status_codes 
{
    INVALID_INPUT_CODE = -1,
    MEM_ALLOC_ERROR_CODE = -2,
    NULL_PTR_ARG_CODE = -3,
    SUCCESS_CODE = 1,
} status_code_t; 

enum input_consts
{
    INPUT_NUMS_CNT = 4,
    TOTAL_PERMS_CNT = 6,
};

status_code_t init_input_data(input_data_t *inp_data);
status_code_t parse_input(input_data_t *inp_data);
status_code_t init_input(input_data_t *inp_data);
status_code_t cnt_max_value(input_data_t *inp_data, uint64_t *ret_max_value);

uint64_t mod_pow(unsigned base, unsigned exp, uint64_t mod); 

int 
main(void) 
{
    input_data_t inp_data;
    uint64_t max_val = 0;

    init_input_data(&inp_data);

    if (parse_input(&inp_data) != SUCCESS_CODE)
        return EXIT_FAILURE;

    cnt_max_value(&inp_data, &max_val);

    printf("%lu\n", max_val);

    return EXIT_SUCCESS;
}

status_code_t
cnt_max_value(input_data_t *inp_data, uint64_t *ret_max_value)
{
    SOFT_ASSERT(inp_data == NULL,
        "inp_data was received as a NULL pointer", NULL_PTR_ARG_CODE);

    SOFT_ASSERT(ret_max_value == NULL,
        "ret_value was received as a NULL pointer", NULL_PTR_ARG_CODE);

    unsigned input_nums[INPUT_NUMS_CNT - 1] = {
        inp_data->fst_num, inp_data->sec_num, inp_data->third_num};

    const unsigned perm[TOTAL_PERMS_CNT][INPUT_NUMS_CNT - 1] = 
    {
        {0, 1, 2}, {0, 2, 1},
        {1, 0, 2}, {1, 2, 0},
        {2, 0, 1}, {2, 1, 0}
    };

    for (int idx = 0; idx < TOTAL_PERMS_CNT; ++idx) 
    {
        unsigned fst_loop_num = input_nums[perm[idx][0]];
        unsigned sec_loop_num = input_nums[perm[idx][1]];
        unsigned third_loop_num = input_nums[perm[idx][2]];

        uint64_t higher_pow = (uint64_t)pow((double)sec_loop_num, (double)third_loop_num);
        uint64_t val = mod_pow(fst_loop_num, higher_pow, inp_data->mod); 

        if (val > *ret_max_value)
        {
            *ret_max_value = val;
        }
    }

    return SUCCESS_CODE;
}

status_code_t
parse_input(input_data_t *inp_data)
{
    SOFT_ASSERT(inp_data == NULL,
        "data was received as a NULL pointer", NULL_PTR_ARG_CODE);

    int read_args_cnt = scanf(
        "%u %u %u %lu", 
        &inp_data->fst_num, &inp_data->sec_num, 
        &inp_data->third_num, &inp_data->mod);

    SOFT_ASSERT(read_args_cnt != INPUT_NUMS_CNT,
        "Invalid input", INVALID_INPUT_CODE);

    return SUCCESS_CODE;
}

status_code_t
init_input_data(input_data_t *inp_data)
{
    SOFT_ASSERT(inp_data == NULL,
        "data was received as a NULL pointer", NULL_PTR_ARG_CODE);

    inp_data->fst_num = 0;
    inp_data->sec_num = 0;
    inp_data->third_num = 0;
    inp_data->mod = 0;

    return SUCCESS_CODE;
}

uint64_t
mod_pow(unsigned base, unsigned exp, uint64_t mod) 
{
    uint64_t result = 1 % mod;
    base %= mod;

    while (exp > 0) 
    {
        if (exp & 1)
        {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }

    return result;
}

