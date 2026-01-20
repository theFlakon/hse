#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "calc_funcs.h"
#include "constants.h"

int 
calc_sols(double first_ind, double sec_ind, double third_ind, struct roots *rt)
{
    int roots_cnt;
    double D = sec_ind * sec_ind - 4 * first_ind * third_ind;

    if (D < 0)
    {
        roots_cnt = 0;
    }
    else if (D == 0)
    {
        rt->f_root = -sec_ind / (2 * first_ind);
        roots_cnt = 1;
    }

    else
    {
        rt->f_root = (-sec_ind + sqrt(D)) / (2 * first_ind);
        rt->s_root = (-sec_ind - sqrt(D)) / (2 * first_ind);
        roots_cnt = 2;
    }

    return roots_cnt;

}
