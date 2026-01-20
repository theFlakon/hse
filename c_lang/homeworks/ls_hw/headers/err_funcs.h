#ifndef ERR_FUNCS_H
#define ERR_FUNCS_H

// Printing error messages
void err_msg(const char *func_name, const char *msg);

// Print error msg if user entered an invalid directory name
int no_such_dir_err(const char *prog_name, const char *dir_name);

#endif
