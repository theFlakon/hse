#ifndef STR_FUNCS_H
#define STR_FUNCS_H

#include <stdio.h>
#include <sys/types.h>

enum GET_LINE_CONSTS
{
    START_SIZE = 2,
    INCR_COEFF = 2,
    ERR_VALUE = -1,
};

char *str_chr(const char *s, int c);
size_t str_len(const char *s);
char *str_cpy(char *restrict dst, const char *restrict src);
char *str_n_cpy (char *restrict dst, const char *restrict src, 
        size_t dsize);
char *str_cat(char *restrict dst, const char *restrict src);
char *str_n_cat(char *restrict dst, const char *restrict src,
        size_t ssize);
ssize_t get_line(char **restrict lineptr, size_t *restrict n,
        FILE *restrict stream);
char *str_dup(const char *s);

#endif
