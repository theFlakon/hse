#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>

void print(const char *str, ...);
void put_str(const char *str);
void put_int(int num);
void put_float(float num);
void put_error(const char *err_msg);

int
main()
{
}

void
print(const char *str, ...)
{
    va_list arg_ptr;
    va_start(arg_ptr, str);

    for (int i = 0; str[i] != '\0'; ++i)
    {
        if (str[i] == '%')
        {
            switch (str[i + 1])
            {
                case 's':
                    put_str(va_arg(arg_ptr, char *));
                    break;

                case 'd':
                    put_int(va_arg(arg_ptr, int));
                    break;

                case 'c':
                    putc(va_arg(arg_ptr, int), stdout);
                    break;

                default:
                    put_error("Error: unknown format after %\n");
                    break;
            }

            ++i;
        }

        else
        {
            putc((int)str[i], stdout);
        }
    }

    va_end(arg_ptr);
}

void
put_str(const char *str)
{
    for (int i = 0; str[i] != '\0'; ++i)
        putc((int)str[i], stdout);

}

void
put_int(int num)
{
    int nm_len = (num == 0) ? 1 : 0;
    int digit = 0;

    if (num < 0)
    {
        putc('-', stdout);
        num = abs(num);
    }

    while (num / (int)pow(10, nm_len) > 0)
        ++nm_len;

    for (int i = 1; i <= nm_len; ++i)
    {
        digit = num / (int)(pow(10, nm_len - i));
        digit %= 10;
        putc((int)((int)'0' + digit), stdout);
    }
}

void
put_float(float num)
{
    int integer_part = (int)num;
    float fract_part = (num - (float)integer_part) * 10;

    put_int(integer_part);
    putc('.', stdout);

    while ((int)fract_part % 10 != 0)
    {
        fract_part *= 10.0f;
    }

    fract_part /= 10.0f;

    put_int((int)fract_part);
}

void 
put_error(const char *err_msg)
{
    for (int i = 0; err_msg[i] != '\0'; ++i)
        putc((int)err_msg[i], stderr);
}
