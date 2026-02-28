#include <stdio.h>
#include "foo.h"

int main(void)
{
    int a = 0;
    int b = 0;
    scanf("%d%d", &a, &b);
    printf("%d | %d = %d", a, b, foo(a, b));
    return 0;
}
