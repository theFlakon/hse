#include <stdio.h>
#include <stdint.h>

void swap_bits(uint16_t *num, int pos1, int pos2);
uint16_t transpose_matrix_bits(uint16_t num);

int
main(void) 
{
    uint16_t input_num = 0;
    uint16_t result_num = 0;

    if (scanf("%hx", &input_num) != 1)
    {
        fprintf(stderr, "invalid input number\n");
        return 1;
    }

    result_num = transpose_matrix_bits(input_num);
    
    printf("%04hx\n", result_num);
    
    return 0;
}

void
swap_bits(uint16_t *num, int pos1, int pos2)
{
    int bit1 = 0;
    int bit2 = 0;

    if (num == NULL) return;
    
    bit1 = (*num >> pos1) & 1U;
    bit2 = (*num >> pos2) & 1U;
    
    if (bit1 != bit2)
        *num ^= (1U << pos1) | (1U << pos2);
}

uint16_t
transpose_matrix_bits(uint16_t num)
{
    uint16_t result = num;

    int pos_ij = 0;
    int pos_ji = 0;
    
    for (int row = 0; row < 4; row++) 
        for (int column = row + 1; column < 4; column++) 
        {
            pos_ij = row * 4 + column; 
            pos_ji = column * 4 + row;
            
            swap_bits(&result, pos_ij, pos_ji);
        }
    
    return result;
}
