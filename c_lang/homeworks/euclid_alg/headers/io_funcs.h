#ifndef IO_FUNCS_H
#define IO_FUNCS_H

#include <stdbool.h>

bool get_inp_data(long long* first_nm, long long* sec_nm);
void err_msg(const char* msg);
void welcome_msg();
void clear_buffer();
void print_rs(long long result);

#endif
