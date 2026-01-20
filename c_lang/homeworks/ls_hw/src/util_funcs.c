#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <dirent.h>

#include "util_funcs.h"
#include "consts.h"
#include "err_funcs.h"
#include "files_io_funcs.h"

enum exit_codes
dispatch(struct input_params *input_param)
{
    if (input_param == NULL)
    {
        err_msg("dispatch", "a NULL pointer was received");
        return EXIT_ERROR_CODE;
    }

    DIR *dir_ptr = NULL;

    dir_ptr = opendir(input_param->dir_name);

    if (dir_ptr == NULL)
    {
        fprintf(stderr, "%s: cannot access '%s': No such file or directory\n",
        input_param->prog_name, input_param->dir_name);
        return EXIT_ERROR_CODE;
    }

    print_file_lst(
    dir_ptr, input_param->dir_name, 
    input_param->print_hid_files, 
    input_param->print_long_list,
    input_param->print_recursive);

    return EXIT_SUCCESS_CODE;
}

bool
set_path_finfo(
    char *path_buffer, struct stat *file_info, 
    const char *curr_file_name, const char *input_path)
{

    int snprintf_ret_code = DEF_INT_VAL;
    int stat_ret_code = DEF_INT_VAL;

    snprintf_ret_code = snprintf(path_buffer, PATH_MAX, "%s/%s", input_path, curr_file_name);

    if (snprintf_ret_code == SNPRINTF_ERR_CODE)
    {
        err_msg("set_path_finfo",
        "writing data to the buffer failed");

        return false;
    }

    stat_ret_code = stat(path_buffer, file_info);

    if (stat_ret_code == STAT_ERR_CODE)
    {
        fprintf(stderr, 
        "Receiving information about '%s' failed\n", curr_file_name);

        return false;
    }

    return true;
}


bool
input_param_init(struct input_params *input_param, char *argv[])
{
    if (input_param != NULL && argv != NULL)
    {
        input_param->prog_name = argv[PROG_NAME_IND];
        input_param->dir_name = NULL;
        input_param->print_hid_files = false;
        input_param->print_recursive = false;
        input_param->print_long_list = false;
        return true;
    }

    else
    {
        err_msg("input_param_init", "a NULL ponter was received");
        return false;
    }
}

bool
parse_input_args(
    int argc, char *argv[], struct input_params *input_param)
{
    if (argc <= 0)
    {
        err_msg("parse_input_args", "argc value must be above zero");
        return false;
    }

    if (argv == NULL)
    {
        err_msg("parse_input_args",
        "a NULL pointer was received instead of pointer to 'char *'");
        return false;
    }

    if (input_param == NULL)
    {
        err_msg("parse_input_args",
    "a NULL pointer was received instead of pointer to 'struct input_params'");
        return false;
    }

    size_t arg_ind = 1; // Skip the prog. name
    size_t char_ind = DEF_SIZE_T_VAL;

    size_t curr_len = DEF_SIZE_T_VAL;

    for (; arg_ind < (size_t)argc; ++arg_ind)
    {
        if (argv[arg_ind] != NULL)
        {
            curr_len = strlen(argv[arg_ind]);

            if (argv[arg_ind][DASH_IND] == '-' && 
                    curr_len >= MIN_FLAGS_LEN)
            {
                char_ind = DASH_IND + 1; // Skip the '-'

                for(; char_ind < curr_len; ++char_ind)
                {
                    switch (argv[arg_ind][char_ind])
                    {
                        case 'a':
                            input_param->print_hid_files = true;
                            break;

                        case 'R':
                            input_param->print_recursive = true;
                            break;

                        case 'l':
                            input_param->print_long_list = true;
                            break;

                        default:
                            fprintf(stderr, "%s: invalid option '-%c'\n",
                            argv[PROG_NAME_IND], argv[arg_ind][char_ind]);

                            return false;

                    }
                }
            }

            else
            {
                input_param->dir_name = strdup(argv[arg_ind]);

                if (input_param->dir_name != NULL)
                    return true;

                else 
                {
                    err_msg("parse_input_args",
                    "memory allocation (strdup) failed");
                    return false;
                }
            }

        }

        else
        {
            err_msg("parse_input_args",
            "a NULL pointer received instead of pointer to 'char'");
            return false;
        }
    }

    if (input_param->dir_name == NULL)
    {
        input_param->dir_name = strdup(".");

        if (input_param->dir_name != NULL)
            return true;
        else
        {
            err_msg("parse_input_args",
            "memory allocation (strdup) failed");
            return false;
        }
    }

    return true;
}
