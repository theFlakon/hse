#ifndef CONSTS_H
#define CONSTS_H

#include <stdbool.h>

struct input_params
{
    char *prog_name;
    char *dir_name;
    
    bool print_hid_files;
    bool print_recursive;
    bool print_long_list;
};

// Default values for some data types
// It's necessary to prevent init. vars with trash by comp.

// For ptr def value is NULL

enum def_data_types_values
{
    DEF_INT_VAL = 0,
    DEF_SIZE_T_VAL = 0,
    DEF_CHAR_VAL = '\0',
    DEF_BOOL_VAL = 0
};

enum funcs_codes
{
    STRFTIME_ERR_CODE = 0, // For strftime() function
    SNPRINTF_ERR_CODE = -1, // For snprintf() function
    STAT_ERR_CODE = -1, // For stat() function
    STRS_SAME_VAL = 0, // For strcmp() func when the strs are the same
    SET_PF_ERR_CODE = 0, // For set_path_finfo() func..
                         // It's declared in util_funcs.h
    PARSE_INP_ERR_CODE = 0, // For parse_input_args() function
                            // It's declared in util_funcs.h
};

// Exit codes  main()
enum exit_codes
{
    EXIT_SUCCESS_CODE = 0,
    EXIT_ERROR_CODE = 1,
};

enum other_consts
{
    DEF_BUFFER_SIZE = 30, // We use it if we need just a small buffer
    DASH_IND = 0, // For str like -Rla, here ind of '-' equals to 0
    PROG_NAME_IND = 0, // Prog index in argv
    MIN_FLAGS_LEN = 2, // Flag section len starts with this, 
                       // e.g.: '-la, -l' - flags, '-' - not a flag

};

#endif
