/*********************************************************
*   Copyright (C) LiXingang
*
********************************************************/
#include "utl/ulc_user.h"

SEC(".spf.cmd/")
int main(int argc, char **argv)
{
    int i;

    BPF_Print("argc=%d \n", argc);
    for (i=0; i<argc; i++) {
        BPF_Print("%d: %s \n", i, argv[i]);
    }

    return 0;
}


