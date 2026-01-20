#include <stdio.h>
#include <stdint.h>

int 
main()
{
    FILE* bin_file = fopen("file.bin", "wb");
    uint16_t nums[] = {1, 2, 3, 4, 5, 6};
    uint8_t k = 2;

    fwrite(nums, sizeof(nums[0]), sizeof(nums) / sizeof(nums[0]), bin_file);
    fwrite(&k, sizeof(k), 1, bin_file);

    fclose(bin_file);
}
