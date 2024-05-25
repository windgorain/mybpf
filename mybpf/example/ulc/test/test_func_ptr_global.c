/*********************************************************
*   Copyright (C) LiXingang
*
********************************************************/
#include "utl/ulc_user.h"

typedef int (*PF_test2)(void);

noinline int test2(void);

PF_test2 g_test2 = NULL;

noinline int test(void)
{
    return g_test2();
}

noinline int test2(void)
{
    printf("global func ptr called! \n");
    return 0;
}

SEC(".spf.cmd/main")
int main()
{
    g_test2 = test2;
    test();
    return 0;
}
