#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int
main(void)
{
    uint32_t input_num = 0;
    uint32_t mask = (1 << 8) - 1;

    uint32_t input_cpy;

    uint32_t nums[4];

    uint32_t fst_even;
    uint32_t sec_even;
    uint32_t fst_odd;
    uint32_t sec_odd;

    scanf("%u", &input_num);

    uint32_t result = 0;

    for (int i = 0; i < 4; ++i)
    {
        input_cpy = input_num >> (8 * i);
        input_cpy &= mask;
        nums[i] = input_cpy;
    }


    fst_even = (nums[0] > nums[2]) ? nums[0] : nums[2];
    sec_even = (nums[0] > nums[2]) ? nums[2] : nums[0];

    fst_odd = (nums[1] > nums[3]) ? nums[1] : nums[3];
    sec_odd = (nums[1] > nums[3]) ? nums[3] : nums[1];

    result |= (fst_odd);
    result |= (sec_odd << 8);
    result |= (fst_even << 16);
    result |= (sec_even << 24);

    printf("%u\n", result);

    return 0;
}
