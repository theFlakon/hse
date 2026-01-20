#include <stdio.h>

int
main(void)
{
    int arr1[10000];
    int arr2[10000];

    int N, M;

    scanf("%d", &N);

    for (int i = 0; i < N; ++i)
        scanf("%d", arr1 + i);

    scanf("%d", &M);

    for (int i = 0; i < M; ++i)
        scanf("%d", arr2 + i);

    int min = (M < N) ? M : N;
    int max = (M > N) ? M : N;

    for (int i = 0; i < min; ++i)
    {
        printf("%d %d ", arr1[i], arr2[i]);
    }

    if (min != max)
    {
        if (N > M)
            for (int i = min; i < N; ++i)
                printf("%d ", arr1[i]);

        else
            for (int i = min; i < M; ++i)
                printf("%d ", arr2[i]);
    }

    printf("\n");

}
