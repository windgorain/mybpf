/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*
********************************************************/
#include <string.h>
#include "utl/ulc_user.h"

static long long fibonacci(long x)
{
    long long ret;
    long long prev = 0;
    long long next = 1;

    if (x < 2) {
        return x;
    }

    while (x >= 2) {
        x--;
        ret = prev + next;
        prev = next;
        next = ret;
    }

    return ret;
}

SEC(".spf.cmd/")
int main(int argc, char **argv)
{
    long count;

    if (argc < 2) {
        printf("Need params: number \r\n");
        return -1;
    }

    bpf_strtol(argv[1], 20, 10, &count);

    unsigned long long start = bpf_ktime_get_ns();
    unsigned long long ret = fibonacci(count);
    unsigned long long end = bpf_ktime_get_ns();

    printf("Bpf Use time %llu ms, result:%lld \n", (end - start)/1000000, ret);

    return 0;
}

