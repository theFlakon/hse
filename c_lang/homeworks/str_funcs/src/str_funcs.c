#include <stdlib.h>
#include <errno.h>

#include "str_funcs.h"

size_t 
str_len(const char *s)
{
    size_t s_dst_ind = 0;

    for (; s[s_dst_ind] != '\0'; ++s_dst_ind)
        ;

    return s_dst_ind;
}

char* 
str_chr(const char *s, int c)
{
    char *ptr = (char*)s;

    do
    {
        if ((int)*ptr == c)
            return ptr;
    } while (*ptr++ != '\0');

    return NULL;
}

char*
str_cpy(char *restrict dst, const char *restrict src)
{
    size_t dst_ind = 0;

    while (*src != '\0')
        dst[dst_ind++] = *src++;

    dst[dst_ind] = '\0';

    return dst;
}

char*
str_n_cpy(char *restrict dst, const char *restrict src,
        size_t dsize)
{
    size_t dst_ind = 0;
    size_t src_ind = 0;

    for (; dst_ind < dsize; ++dst_ind)
    {
        if (src[src_ind] != '\0')
        {
            dst[dst_ind] = src[src_ind];
            ++src_ind;
        }

        else
        {
            dst[dst_ind] = '\0';
        }
    }

    return dst;
}

char*
str_cat(char *restrict dst, const char *restrict src)
{
    size_t dst_start_len = str_len(dst);
    size_t src_ind = 0;

    for (; src[src_ind] != '\0'; ++src_ind)
        dst[dst_start_len + src_ind] = src[src_ind];

    return dst;
}

char*
str_n_cat(char *restrict dst, const char *restrict src,
        size_t ssize)
{
    size_t dst_start_len = str_len(dst);
    size_t src_ind = 0;

    for(; src_ind < ssize; ++src_ind)
    {
        dst[dst_start_len + src_ind] = src[src_ind];

        if (src[src_ind] == '\0')
            break;
    }

    dst[dst_start_len + src_ind] = '\0';

    return dst;
}

char*
str_dup(const char *s)
{
    size_t ind = 0;
    size_t s_len = str_len(s);

    char* new_str = (char *)malloc(sizeof(char) * s_len);

    if (new_str == NULL)
        errno = ENOMEM;

    else
        for(; ind < s_len; ++ind)
            new_str[ind] = s[ind];

    return new_str;
}

ssize_t
get_line(char **restrict lineptr, size_t *restrict n, 
        FILE *restrict stream)
{
    int ch = 0;
    ssize_t added_chars_cnt = 0;
    char* realloc_ptr = NULL;

    if (lineptr == NULL || n == NULL || stream == NULL)
    {
        errno = EINVAL;
        return ERR_VALUE;
    }

    if (*lineptr == NULL)
    {
        *n = START_SIZE;
        *lineptr = (char *)malloc(sizeof(char) * START_SIZE);

        if (*lineptr == NULL)
        {
            errno = ENOMEM;
            return ERR_VALUE;
        }
    }

    if (*n == 0)
        *n = START_SIZE;

    while ((ch = fgetc(stream)) != EOF)
    {
        if ((size_t)added_chars_cnt == *n - 1)
        {
            *n *= INCR_COEFF;

            realloc_ptr = (char *)realloc(*lineptr, sizeof(char) * *n);

            if (realloc_ptr == NULL)
            {
                errno = ENOMEM;
                return ERR_VALUE;
            }

            *lineptr = realloc_ptr;
        }

        (*lineptr)[added_chars_cnt++] = (char)ch;

        if ((char)ch == '\n')
            break;
    }

    if (added_chars_cnt == 0)
        return ERR_VALUE;

    (*lineptr)[added_chars_cnt] = '\0';

    return added_chars_cnt;
}
