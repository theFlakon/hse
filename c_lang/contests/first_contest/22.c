#include <stdio.h>
#include <stdlib.h>

int min(int a, int b);
int max(int a, int b);
int calculate_min_jumps(int x, int y);

int main(void) 
{
    int result = 0;
    int x = 0;
    int y = 0;
    
    if (scanf("%d %d", &x, &y) != 2) 
    {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    result = calculate_min_jumps(x, y);
    printf("%d\n", result);

    
    return 0;
}

int
calculate_min_jumps(int x, int y) 
{
    int abs_x = abs(x);
    int abs_y = abs(y);
    int diff = 0;
    
    int min_coord = min(abs_x, abs_y);
    int max_coord = max(abs_x, abs_y);
    
    if (max_coord == 0) 
        return 0; 
    
    
    if (max_coord == min_coord) 
        return 2 * max_coord;
    else if (max_coord == min_coord + 1) 
        return 2 * max_coord - 1;
    else 
    {
        diff = max_coord - min_coord;
        if (diff % 2 == 0) 
            return 2 * max_coord;
        else 
            return 2 * max_coord - 1;
        
    }
}

int
min(int a, int b) 
{
    return (a < b) ? a : b;
}

int 
max(int a, int b) 
{
    return (a > b) ? a : b;
}
