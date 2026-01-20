#ifndef UTIL_FUNCS_H
#define UTIL_FUNCS_H

#include <sys/stat.h>
#include <stdbool.h>

#include "consts.h"

// Returns true on success, false otherwise
bool set_path_finfo(char *path_buffer, struct stat *file_info,
        const char *curr_file_name, const char *input_path);

// This func inits the corresponding structure
// It returns true on success and false otherwise
bool input_param_init(struct input_params *input_param, char *argv[]);

// This func parses input arguments and changes `input_param` fields
// 'input_param' is declared in consts.h
bool parse_input_args(
    int argc, char *argv[], struct input_params *input_param);

enum exit_codes dispatch(struct input_params *input_param);

#endif
