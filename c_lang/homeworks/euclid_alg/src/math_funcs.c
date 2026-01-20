#include "consts.h"

long long
eucl_alg(long long first_nm, long long sec_nm)
{
    long long tmp = 0;

    if (first_nm == 0LL && sec_nm == 0LL)
        return RS_ERR_VAL;

    if (first_nm == 0LL || sec_nm == 0LL)
        return (first_nm != 0LL) ? first_nm : sec_nm;

    first_nm = (first_nm < 0LL) ? -first_nm : first_nm;
    sec_nm = (sec_nm < 0LL) ? -sec_nm : sec_nm;

    while (sec_nm != 0LL)
    {
        tmp = sec_nm;
        sec_nm = first_nm % sec_nm;
        first_nm = tmp;
    }

    return first_nm;
}

