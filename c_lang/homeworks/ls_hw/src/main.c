#include <stddef.h>
#include <stdlib.h>

#include "files_io_funcs.h"
#include "err_funcs.h"
#include "consts.h"
#include "util_funcs.h"

int
main(int argc, char *argv[])
{
    struct input_params input_param;
    bool parse_input_rt_val = DEF_BOOL_VAL;

    input_param_init(&input_param, argv);
    parse_input_rt_val = parse_input_args(argc, argv, &input_param);

    if (parse_input_rt_val == PARSE_INP_ERR_CODE)
        return EXIT_ERROR_CODE;

    int exit_code = dispatch(&input_param);

    free(input_param.dir_name);
   
    return exit_code;
}
