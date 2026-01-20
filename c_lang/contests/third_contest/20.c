#include <stdio.h>
#include <limits.h>

unsigned g(unsigned num)
{
    if (num == 0)
        return 0;

    for (unsigned k = 1; k <= UINT_MAX; k++) {
        unsigned x = num / k;

        if (x * k != num)
            continue;

        if (k * k <= x && x < (k + 1) * (k + 1))
            return x;
    }

    return 0; 
}

unsigned f(unsigned x)
{
    long long s = 0;
    unsigned i;
    for (i = 0; s <= x; i++) {
        s += 2 * i + 1;
    }
    return x * (i - 1);
}

int main(void) 
{
    unsigned input_num = 0;

    scanf("%u", &input_num);
    
    unsigned result = g(input_num);
    printf("%u\n", result);
    
    return 0;
}
