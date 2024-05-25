/*********************************************************
*   Copyright (C) LiXingang
*
********************************************************/
#include "utl/ulc_user.h"

typedef int (*PF_test2)(void);

noinline int test(PF_test2 func)
{
    return func();
}

noinline int test2(void)
{
    printf("func ptr called! \n");
    return 0;
}

SEC(".spf.cmd/main")
int main()
{
    test(test2);
    return 0;
}


