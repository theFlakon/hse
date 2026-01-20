#include <stdio.h>

#include "consts.h"
#include "io_funcs.h"
#include "math_funcs.h"

int 
main()
{ 
    long long first_nm = 0, sec_nm = 0, result = 0;

    welcome_msg();

    while (TRUE)
    {
        printf("\nEnter two nums:\n");

        if (!get_inp_data(&first_nm, &sec_nm))
        {
            err_msg("ivalid input data, try again.");
        }

        else
        {
            result = eucl_alg(first_nm, sec_nm);
            if (result == RS_ERR_VAL)
                err_msg("the greatest common divisor for these numbers isn't defined, try again");
            else
                print_rs(result);
        }
    }

    return 0;
}
