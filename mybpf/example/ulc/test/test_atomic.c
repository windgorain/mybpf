/*********************************************************
*   Copyright (C) LiXingang
*
********************************************************/
#include "utl/ulc_user.h"
#include "utl/print_color.h"

static unsigned long long g_test_count = 1;

SEC(".spf.cmd/hello_test")
int main()
{
    unsigned long long ret;

    ret = __sync_fetch_and_add(&g_test_count, 1);
    if ((ret != 1) || (g_test_count != 2)) {
        PRINTFLM_ERR("test failed ");
        PRINTLN_RED("ret=0x%llx, count=0x%llx", ret, g_test_count);
        return -1;
    }

    ret = __sync_fetch_and_add(&g_test_count, -1);
    if ((ret != 2) && (g_test_count != 1)) {
        PRINTFLM_ERR("test failed ");
        PRINTLN_RED("ret=0x%llx, count=0x%llx", ret, g_test_count);
        return -1;
    }

    ret = __sync_fetch_and_or(&g_test_count, 0x10);
    if ((ret != 1) && (g_test_count != 0x11)){
        PRINTFLM_ERR("test failed ");
        PRINTLN_RED("ret=0x%llx, count=0x%llx", ret, g_test_count);
        return -1;
    }

    ret = __sync_fetch_and_and(&g_test_count, 0x10);
    if ((ret != 0x11) && (g_test_count != 0x10)){
        PRINTFLM_ERR("test failed ");
        PRINTLN_RED("ret=0x%llx, count=0x%llx", ret, g_test_count);
        return -1;
    }

    ret = __sync_fetch_and_xor(&g_test_count, 0x11);
    if ((ret != 0x10) && (g_test_count != 0x1)){
        PRINTFLM_ERR("test failed ");
        PRINTLN_RED("ret=0x%llx, count=0x%llx", ret, g_test_count);
        return -1;
    }

    ret = __sync_val_compare_and_swap(&g_test_count, 0x1, 0x10);
    if ((ret != 0x1) && (g_test_count != 0x10)){
        PRINTFLM_ERR("test failed ");
        PRINTLN_RED("ret=0x%llx, count=0x%llx", ret, g_test_count);
        return -1;
    }

    int bret = __sync_bool_compare_and_swap(&g_test_count, 0x10, 0x1);
    if ((! bret) && (g_test_count != 0x1)){
        PRINTFLM_ERR("test failed ");
        PRINTLN_RED("ret=0x%llx, count=0x%llx", ret, g_test_count);
        return -1;
    }

    printf("Test OK \n");

    return 0;
}


