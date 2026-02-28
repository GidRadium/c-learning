#include "foo.h"
#include <assert.h>

int main(void)
{
    assert(foo(1, 2) == 3);
    assert(foo(0, 0) == 0);
    return 0;
}
