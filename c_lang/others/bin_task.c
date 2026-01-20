#include <stdio.h>

#define NUM_SIZE 8

size_t get_file_len(FILE *file)
{
    size_t current = ftell(file);
    fseek(file, 0L, SEEK_END);
    size_t size = ftell(file);
    fseek(file, current, SEEK_SET);
    return size;
}

int main()
{
    FILE *file = fopen("file.bin", "rb");

    size_t file_len = get_file_len(file);
    size_t total_numbers = file_len / NUM_SIZE;
    
    for (size_t i = 0; i < total_numbers; i += 2)
    {
        long long left_num = 0, right_num = 0;
        
        fread(&left_num, NUM_SIZE, 1, file);
        fread(&right_num, NUM_SIZE, 1, file);
        
        if ((left_num + right_num) % 3 == 0)
            printf("%lld ", right_num);
    }
    
    printf("\n");
    fclose(file);

    return 0;
}
