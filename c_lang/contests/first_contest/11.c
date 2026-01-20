#include <stdio.h>

enum BYTES_CONSTS
{
    ONE_BYTE_MOVE = 8,
};

void err_msg(const char* msg);
unsigned int get_mask(void);

int
main(void) 
{
    unsigned int input_num = 0;
    unsigned int mask = 0; 

    if (scanf("%u", &input_num) != 1)
    {
        err_msg("invalid input arguments");
        return 1;
    }
    
    mask = get_mask();

    input_num ^= mask;

    printf("%u\n", input_num);
    
    return 0;
}

unsigned int
get_mask(void)
{
    unsigned int mask = 1U;

    mask <<= ONE_BYTE_MOVE;
    --mask;
    mask <<= ONE_BYTE_MOVE * 3;

    return mask;
}

void
err_msg(const char* msg)
{
    fprintf(stderr, "An error occurred: %s\n", msg);
}
