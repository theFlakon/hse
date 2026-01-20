#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>

#define user_assert(condition, error_message, return_value) \
    do { \
        if (condition) { \
            fprintf(stderr, "An error occurred: %s\n", error_message); \
            return return_value; \
        } \
    } while(0)

// Codes for return in funcs != main()
typedef enum status_codes 
{
    INVALID_INPUT_CODE = -1,
    MEM_ALLOC_ERROR_CODE = -2,
    NULL_PTR_ARG_CODE = -3,
    SUCCESS_CODE = 1,
} status_code_t; 

// Codes for return in main()
enum exit_status_codes 
{
    EXIT_SUCCESS_CODE = 0,
    EXIT_ERROR_CODE = 1,
}; 

typedef unsigned usgn;

bool isprime(usgn num);
usgn calc_nearest_prime(usgn num);

int 
main(void)
{
    usgn input_num = 0, result = 0;

    user_assert(scanf("%d", &input_num) != 1,
        "Invalid input", EXIT_ERROR_CODE);

    result = calc_nearest_prime(input_num);

    printf("%d\n", result);

    return EXIT_SUCCESS_CODE;
}

usgn
calc_nearest_prime(usgn num)
{
    if ((num != 2 && num % 2 == 0 && num) || num == 1)
        ++num;

    while (num < UINT_MAX - 1)
    {
        if (isprime(num))
            return num;

        num += 2;
    }

    return INT_MAX;

}

bool
isprime(usgn num)
{
    if (num == 1 || num == 0)
        return false;

    usgn iter_max = (usgn)sqrt((double)num) + 1;

    for (int divisor = 2; divisor < iter_max; ++divisor)
    {
        if (num % divisor == 0)
            return false;
    }

    return true;
}
