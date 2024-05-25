/*********************************************************
*   Copyright (C) LiXingang
*
********************************************************/
#include "utl/ulc_user.h"

static U64 g_count1 = 0;
static int g_count2 = 1000;
static char *g_string = "global-test";

SEC(".spf.cmd/main")
int main()
{
    g_count1 ++;
    g_count2 ++;

    printf("%s: count1:%llu count2:%d \n", g_string, g_count1, g_count2);

    return 0;
}


