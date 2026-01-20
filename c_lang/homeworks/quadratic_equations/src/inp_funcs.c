#include <stdio.h>
#include <stdlib.h>

void
get_inp_data(double *first_ind, double *sec_ind, double *third_ind)
{
    printf("A program that can help you solve quadratic equation\n");
    printf("Enter indices for your equation. E.g.: 2x^2 - 5x + 1 = 0 equals to 2 -5 1\n");

    if (scanf("%le %le %le", first_ind, sec_ind, third_ind) != 3 || *first_ind == 0)
    {
        fprintf(stderr, "Please, enter a correct equation.\n");
        exit(1);
    }
}
