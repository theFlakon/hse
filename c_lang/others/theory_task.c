#include <stdio.h>

#define K 10
#define M 4
#define L 256

unsigned long long a[K];

char b[L];

int main(void)
{
    a[sizeof(b[0] - 10)] = sizeof(b);
    printf("%llu %zu\n", a[M], sizeof a);
}
