#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pwd.h>

#include "err_funcs.h"
#include "files_io_funcs.h"
#include "consts.h"
#include "util_funcs.h"

void 
print_file_lst(
    const char *input_path,
    bool print_all_files, bool print_long_list, bool print_recursive
)
{
    struct dirent *dir_entry;
    DIR *dir_ptr = opendir(input_path);

    struct stat file_info;

    char path_buffer[PATH_MAX] = {DEF_CHAR_VAL};
    char *curr_file_name = NULL;

    bool file_print_cond = DEF_BOOL_VAL;
    bool set_pf_ret_code = DEF_BOOL_VAL;

    if (dir_ptr == NULL)
    {
        fprintf(stderr, "ls: cannot access '%s': No such file or directory\n",
        input_path);
        return;
    }


    if (print_recursive)
        printf("%s:\n", input_path);

    while ((dir_entry = readdir(dir_ptr)) != NULL)
    {
        file_print_cond = print_all_files || 
            (strcmp(".", curr_file_name) != STRS_SAME_VAL &&
            strcmp("..", curr_file_name) != STRS_SAME_VAL &&
            curr_file_name[0] != '.'));

        if (file_print_cond)
        {
            set_pf_ret_code = set_path_finfo(
            path_buffer, &file_info, curr_file_name, input_path);

            if (set_pf_ret_code == SET_PF_ERR_CODE)
                continue;

            if (print_long_list)
                print_finfo_ll();

            if (print_recursive && S_ISDIR(file_info.st_mode))
                print_all_files(    
                path_buffer, print_all_files, print_long_list, print_recursive);
        }
    }


}

void
print_file_lst(
    DIR *dir_ptr, const char *input_path, 
    bool print_all_files, bool print_long_list, bool print_recursive)
{
    struct dirent *entry;
    struct stat file_info;

    bool set_pf_ret_code = DEF_BOOL_VAL;

    char path_buffer[PATH_MAX];
    char *curr_file_name = NULL;

    if (print_recursive)
        printf("%s\n", input_path);

    while ((entry = readdir(dir_ptr)) != NULL)
    {
        curr_file_name = entry->d_name;

        if (print_all_files || 
                (strcmp(".", curr_file_name) != STRS_SAME_VAL &&
                strcmp("..", curr_file_name) != STRS_SAME_VAL &&
                curr_file_name[0] != '.'))
        {

            if (print_long_list)
            {
                set_pf_ret_code = set_path_finfo(
                path_buffer, &file_info, curr_file_name, input_path 
                        );

                if (set_pf_ret_code == SET_PF_ERR_CODE)
                    continue;

                print_ft_perm_section(&file_info);
                printf("%lu ", (unsigned long)file_info.st_nlink);
                print_owner_grp_uname(&file_info);
                printf("%lu ", (unsigned long)file_info.st_size);
                print_last_mod_date(&file_info);

                printf("%s\n", curr_file_name);
            }

            else
            {
                printf("%s  ", curr_file_name);
            }
        }
    }

    if (!print_long_list)
        putc('\n', stdout);

}

void
print_owner_grp_uname(const struct stat *file_info)
{
    if (file_info == NULL)
    {
        err_msg("print_owner_grp_uname", "a NULL pointer was received");
        return;
    }

    uid_t owner_id = file_info->st_uid;
    uid_t group_id = file_info->st_gid;

    struct passwd *owner_info = getpwuid(owner_id);
    struct passwd *group_info = getpwuid(group_id);

    if (owner_info != NULL && group_info != NULL)
    {
        printf("%s %s  ", owner_info->pw_name, group_info->pw_name);
        return;
    }

    if (owner_info == NULL)
        err_msg("print_owner_grp_uname", 
        "file owner's info getting was failed");

    if (group_info == NULL) 
        err_msg("print_owner_grp_uname", 
        "group info getting was failed");

}

void
print_last_mod_date(const struct stat *file_info)
{
    if (file_info == NULL)
    {
        err_msg("print_last_mod_date", "a NULL pointer was received");
        return;
    }

    time_t lst_mod_time = file_info->st_mtime;
    size_t written_chs_cnt = DEF_SIZE_T_VAL;

    const char *print_format = "%b  %d %H:%M ";
    char buffer[DEF_BUFFER_SIZE] = {DEF_CHAR_VAL};

    const struct tm *tm_info = localtime(&lst_mod_time);

    if (tm_info != NULL)
    {
        written_chs_cnt = strftime(
        buffer, DEF_BUFFER_SIZE, print_format, tm_info);

        if (written_chs_cnt != STRFTIME_ERR_CODE)
            printf("%s", buffer);

        else
            err_msg("print_last_mod_date",
            "writing info when the file was last modified failed");
    }

    else
        err_msg("print_last_mod_date",
        "getting info when the file was last modified failed");
}

void
print_ft_perm_section(const struct stat *file_info)
{

    if (file_info == NULL)
    {
        err_msg("print_ft_perm_section", "a NULL pointer received");
        return;
    }

    mode_t mode = file_info->st_mode;
    
    // File type section

    if (mode & S_IFDIR) putc('d', stdout);
    else putc('-', stdout);

    // File owner permissions section

    if (mode & S_IRUSR) putc('r', stdout);
    else putc('-', stdout);

    if (mode & S_IWUSR) putc('w', stdout);
    else putc('-', stdout);

    if (mode & S_IXUSR) putc('x', stdout);
    else putc('-', stdout);

    // Permission to a user group that owns a file

    if (mode & S_IRGRP) putc('r', stdout);
    else putc('-', stdout);

    if (mode & S_IWGRP) putc('w', stdout);
    else putc('-', stdout);

    if (mode & S_IXGRP) putc('x', stdout);
    else putc('-', stdout);

    // Other users permissions section

    if (mode & S_IROTH) putc('r', stdout);
    else putc('-', stdout);

    if (mode & S_IWOTH) putc('w', stdout);
    else putc('-', stdout);

    if (mode & S_IXOTH) putc('x', stdout);
    else putc('-', stdout);

    putc(' ', stdout);
}
