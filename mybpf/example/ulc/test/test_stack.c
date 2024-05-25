/*********************************************************
*   Copyright (C) LiXingang
*
********************************************************/
#include "utl/ulc_user.h"

static noinline void _test(char *c)
{
    int i;

    for (i=0; i<256; i++) {
        c[i] = 1;
    }
}

static noinline void _output(void)
{
    printf("test OK \n");
}

SEC(".spf.cmd/main")
int main()
{
    char c[256];

    _test(c);
    _output();

    return 0;
}


