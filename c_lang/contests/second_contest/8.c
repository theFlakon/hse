#include <stdio.h>
#include <stdbool.h>

#define MAX_SIZE 4000

#define soft_assert(condition, error_message, return_value) \
    do { \
        if (condition) { \
            fprintf(stderr, "Soft assertion failed: %s\n", error_message); \
            fprintf(stderr, "Condition: %s\n", #condition); \
            fprintf(stderr, "File: %s, Line: %d\n", __FILE__, __LINE__); \
            return return_value; \
        } \
    } while(0)

enum status_codes
{
    EXIT_SUCCESS_CODE = 0,
    EXIT_ERROR_CODE = 1,
};

typedef struct input_nums 
{
    size_t nums_amount;
    int nums_arr[MAX_SIZE];
} input_nums;

bool parse_input(input_nums *input);
bool get_triples_cnt(int *triples_cnt, input_nums *input);
bool is_triple_middle(int fst_num, int sec_num, int third_num);

int
main(void)
{
    input_nums input;
    input.nums_amount = 0;

    int triples_cnt = 0;

    soft_assert(!parse_input(&input),
        "Invalid input", EXIT_ERROR_CODE);


    get_triples_cnt(&triples_cnt, &input);

    printf("%d\n", triples_cnt);

    return EXIT_SUCCESS_CODE;
}

bool
parse_input(input_nums *input)
{
    soft_assert(input == NULL,
        "A NULL pointer was received\n", false);

    int curr_num = 0;

    soft_assert(scanf("%zu", &input->nums_amount) != 1,
        "Invalid input", false);

    for (int num_ind = 0; num_ind < input->nums_amount; ++num_ind)
    {
        soft_assert(scanf("%d", &curr_num) != 1,
            "Invalid input", false);

        input->nums_arr[num_ind] = curr_num;
    }

    return true;
}

bool
get_triples_cnt(int *triples_cnt, input_nums *input)
{
    soft_assert(triples_cnt == NULL,
        "triples_cnt was received as a NULL pointer", false);

    soft_assert(input == NULL,
        "input was received as a NULL pointer", false);

    int i_ind = 0;
    int k_ind = 0;
    int j_ind = 0;
    int sec_range = 0;

    for (k_ind = 1; k_ind < input->nums_amount; ++k_ind)
    {
        if ((k_ind < input->nums_amount / 2))
        {
            sec_range = k_ind;
        }

        else
        {
            sec_range = input->nums_amount - (k_ind + 1);
        }

        for (int add_ind = 1; add_ind <= sec_range; ++add_ind)
        {
            j_ind = k_ind + add_ind;
            i_ind = k_ind - add_ind;

            bool is_middle = is_triple_middle(
                input->nums_arr[i_ind], 
                input->nums_arr[k_ind],
                input->nums_arr[j_ind]
                    );

            if (is_middle)
            {
                ++(*triples_cnt);
            }
        }
    }

    return true;
}

bool 
is_triple_middle(int fst_num, int sec_num, int third_num)
{
    bool fst_condition = (fst_num + third_num) % 2 == 0;
    bool sec_condition = sec_num == (fst_num + third_num) / 2;

    return fst_condition && sec_condition;
}
