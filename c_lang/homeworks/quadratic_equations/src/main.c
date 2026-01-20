#include <stdio.h>

#include "calc_funcs.h"
#include "inp_funcs.h"
#include "constants.h"

int
main()
{
    struct roots rt;

    double first_ind, sec_ind, third_ind;
    first_ind = sec_ind = third_ind = 0;


    get_inp_data(&first_ind, &sec_ind, &third_ind);

    switch (calc_sols(first_ind, sec_ind, third_ind, &rt))
    {
        case 0:
            printf("This equation doesn't have solutions\n");
            break;

        case 1:
            printf("This equation has one solution: %.2f\n", rt.f_root);
            break;

        case 2:
            printf("This equation has two solutions: %.2f and %.2f\n", rt.f_root, rt.s_root);
            break;
    }
}
