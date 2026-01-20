#include <stdio.h>

#include "consts.h"

void
err_msg(const char *func_name, const char *msg)
{
    if (msg != NULL && func_name != NULL)
        fprintf(stderr,
        "An error occurred in %s func.: %s\n", func_name, msg);

    else
        fprintf(stderr,
        "Couldn't print error message: a NULL pointer received\n");
}

