#include <stdio.h>
#include <stdlib.h>

int main()
{
    int i = 0;
    for (;i < 10; i++)
        printf("%ld\n", (random()*100/0xffffffff));
}
