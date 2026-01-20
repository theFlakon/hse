#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_CNT 13

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

status_code_t calc_weights_cnt(int64_t weight, ssize_t *cnt);
void print_answer(ssize_t cnt);

int
main(void) 
{
    int64_t weight = 0;  
    ssize_t cnt = 0; 

    SOFT_ASSERT(scanf("%ld", &weight) != 1,
        "Invalid input", EXIT_FAILURE);

    calc_weights_cnt(weight, &cnt);
    
    print_answer(cnt);
    
    return EXIT_SUCCESS;
}

void
print_answer(ssize_t cnt)
{
    if (cnt == -1)
        printf("%ld\n", cnt);
    else
        printf("%ld\n", cnt);
}

status_code_t
calc_weights_cnt(int64_t weight, ssize_t *cnt)
{
    SOFT_ASSERT(cnt == NULL,
        "cnt was received as a NULL pointer", NULL_PTR_ARG_CODE);

    size_t steps = 0;

    while (weight > 0) 
    {
        steps++;

        if (steps > 13) {
            *cnt = -1;
            return SUCCESS_CODE;
        }

        if (weight % 3 != 0)
            (*cnt)++;

        if (weight % 3 == 2) 
            weight += 1;
        
        weight /= 3;
    }

    return SUCCESS_CODE;
}
