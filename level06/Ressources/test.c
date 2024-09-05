#include <stdio.h>
#include <string.h>


int auth_test(char *s)
{
    int v4;
 
    v4 = (s[3] ^ 0x1337) + 6221293;
    for (int i = 0; i < strnlen(s, 32); ++i) {
        v4 += (v4 ^ (unsigned int)s[i]) % 0x539;
    }
    printf("v4: %d\n", v4);
}

int main(void) {
    auth_test("helllo");
    return 0;
}