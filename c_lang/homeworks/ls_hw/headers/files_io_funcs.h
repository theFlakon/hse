#ifndef IO_FUNCS_H
#define IO_FUNCS_H

#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>

// Main function that prints the list of files
// Its work depends on mode parameter (will be implemented later)
void print_file_lst(
    DIR *dir_ptr, const char *input_path, 
    bool print_all_files, bool print_long_list, bool print_recursive);

// It prints the first section in ls -l
// E. g.: drwxr-xr-x 
void print_ft_perm_section(const struct stat *st);

// It prints uname of the owner and group 
// The 3`rd and 4`th section in ls -l respectively
void print_owner_grp_uname(const struct stat *st);

// It prints last modification date
// E.g.: Oct  06 20:09
void print_last_mod_date(const struct stat *st);

#endif
