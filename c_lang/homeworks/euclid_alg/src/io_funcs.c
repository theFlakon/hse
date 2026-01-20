#include <stdio.h>

#include "consts.h"

void
clear_buffer()
{
    char c = '\0';
    
    while ((c = getchar()) != '\n' && c != EOF && c != '\0');
}

void 
welcome_msg()
{
    printf("This program can help you to find the greatest common divisor \
of two numbers using Euclidean algorithm.\n");

    printf("If you want to stop press Ctrl + C\n");
}

bool
get_inp_data(long long* first_nm, long long* sec_nm)
{

    if (scanf("%lld %lld", first_nm, sec_nm) != 2)
    {
        return FALSE;
    }

    clear_buffer();

    return TRUE;
}

void
err_msg(const char* msg)
{
    fprintf(stderr, "Error occured: %s\n", msg);
}

void 
print_rs(long long result)
{
    printf("The greatest common divisor equals to %lld\n", result);
}
