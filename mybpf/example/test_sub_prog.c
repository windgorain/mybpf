/*********************************************************
*   Copyright (C) LiXingang
*
********************************************************/
#include "utl/ulc_user.h"

noinline int test2(int a)
{
    printf("Hello test2! \n");
    a ++;
    return a;
}

noinline int test(int a)
{
    printf("Hello test! \n");
    a ++;
    return test2(a);
}

SEC(".spf.cmd/main")
int main()
{
    printf("start \n");
    printf("a=%d \n", test(0));
    return 0;
}


