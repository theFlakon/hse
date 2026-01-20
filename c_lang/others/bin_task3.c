#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main()
{
    FILE *bin = fopen("file.bin", "rb");

    int16_t arr[6] = {0};
    size_t arr_idx = 0;

    size_t rt_value = 0;

    int16_t K = 0;

    do
    {
        int16_t curr_nm = 0;

        rt_value = fread(&curr_nm, sizeof(curr_nm), 1, bin);
        
        if (rt_value)
        {
            arr[arr_idx++] = curr_nm;
        }

        K = curr_nm;


    } while(rt_value);

    for (int i = 0; i < arr_idx; ++i)
        printf("%d\n", arr[i]);

    printf("%d\n", K);
}
