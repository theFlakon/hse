#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

enum
{
    MAX_INPUT_AMOUNT = 1500000,
    DEF_VAL = -1,
};

unsigned hash(unsigned num);

ssize_t get_el_ind(ssize_t *arr, size_t arr_size, unsigned num);
void init_arr(ssize_t arr[], size_t arr_len);
void swap(unsigned *fst_var, unsigned *sec_var);

void print_arr(ssize_t *arr, size_t size);

int
main(void)
{
    unsigned nums_amount = 0;
    unsigned curr_num = 0;

    unsigned greatest_num = 0;
    unsigned middle_num = 0;
    unsigned lowest_num = 0;

    ssize_t ind_arr[] = {-1, -1, -1};

    scanf("%u", &nums_amount);

    bool *nums_hash = calloc(2500000000, sizeof(bool));

    if (nums_hash == NULL)
        printf("HI\n");


    for (int i = 0; i < nums_amount; ++i)
    {
        scanf("%d", &curr_num);

        if (!nums_hash[curr_num])
            nums_hash[curr_num] = true;

        else
        {
            nums_hash = false;
        }
    }

    for (int i = 0; i < 3; ++i)
    {
        if (ind_arr[i] != DEF_VAL)
        {
            if ((unsigned)ind_arr[i] > greatest_num)
            {
                lowest_num = middle_num;
                middle_num = greatest_num;
                greatest_num = (unsigned)ind_arr[i];
            }

            else if ((unsigned)ind_arr[i] > middle_num)
            {
                lowest_num = middle_num;
                middle_num = (unsigned)ind_arr[i];
            }

            else
                lowest_num = (unsigned)ind_arr[i];
        }
    }
    
    free(nums_hash);

    printf("%d %d %d\n", lowest_num, middle_num, greatest_num);

    
}

ssize_t
get_el_ind(ssize_t *arr, size_t arr_size, unsigned num)
{
    for (ssize_t i = 0; i < arr_size; ++i)
        if (arr[i] == num)
            return i;

    return -1;
}

void
init_arr(ssize_t arr[], size_t arr_len)
{
    for (size_t i = 0; i < arr_len; ++i)
        arr[i] = DEF_VAL;
}

void
swap(unsigned *fst_var, unsigned *sec_var)
{
    unsigned temp_var = *fst_var;
    *fst_var = *sec_var;
    *sec_var = temp_var;
}

void
print_arr(ssize_t *arr, size_t size)
{
    printf("[ ");

    for (int i = 0; i < size; ++i)
        printf("%zd, ", arr[i]);

    printf("]\n");
}
