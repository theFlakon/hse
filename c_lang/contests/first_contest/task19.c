#include <stdio.h>

enum CODES {
    FOUND = 1,
    ERROR = 0
};

int check_res(unsigned a11, unsigned a12, unsigned b1, unsigned a21, unsigned a22, unsigned b2, unsigned* res1, unsigned* res2) {
    for(int bit = 0; bit < 32; bit++) {
        int isFound = 0;

        unsigned mask = 1U << bit;

        for (int i = 0; i <= 1; i++) 
        {
            for (int j = 0; j <= 1; j++) 
            {
                if(((((a11 & mask) & i) ^ ((a12 & mask) & j)) == (b1 & mask)) && ((((a21 & mask) & i) ^ ((a22 & mask) & j)) == (b2 & mask))) 
                {
                    if(i) *res1 |= mask;
                    if(j) *res2 |= mask;
                    
                    isFound = 1;
                    break;
                }
            }
            if (isFound) break;
        }

        if (!isFound) {
            return ERROR;
        }
    }

    return FOUND;
}

int main(void) {
    unsigned a11 = 0, a12 = 0, a21 = 0, a22 = 0, b1 = 0, b2 = 0;
    
    scanf("%u %d", &a11, &a12);
    scanf("%d %d", &a21, &a22);
    scanf("%d %d", &b1, &b2);

    unsigned res1 = 0;
    unsigned res2 = 0;
    
    int isFound = check_res(a11, a12, b1, a21, a22, b2, &res1, &res2);

    if(isFound == FOUND) {
        printf("Yes\n%d %d\n", res1, res2);
    } else {
        printf("No\n");
    }

    return 0;
}
