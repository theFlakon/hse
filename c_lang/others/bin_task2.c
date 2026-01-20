#include <stdio.h>

size_t get_file_len(FILE *file)
{
    fseek(file, 0, SEEK_END);
    size_t len = ftell(file);
    fseek(file, 0, SEEK_SET);

    return len;
}

int main()
{
    FILE *bin_file = fopen("file.bin", "rb");

    size_t file_len = get_file_len(bin_file);

    size_t near_start_num = 0, near_end_num = 0;

    size_t nums_cnt = file_len / sizeof(size_t);
    
    for (size_t idx = 0; idx < nums_cnt / 2; ++idx)
    {
        fread(&near_start_num, sizeof(near_start_num), 1, bin_file);

        size_t curr_pos = ftell(bin_file);

        fseek(bin_file, -(idx + 1) * sizeof(size_t), SEEK_END);

        fseek(bin_file, curr_pos, SEEK_SET);

        printf("%zu %zu\n", near_start_num, near_end_num);
    }

    fclose(bin_file);

    return 0;
}
