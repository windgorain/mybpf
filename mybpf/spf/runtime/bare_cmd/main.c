/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#include "bs.h"
#include "utl/mybpf_bare.h"


int main(int argc, char **argv)
{
    MYBPF_PARAM_S p = {0};

    if (argc < 2) {
        printf("Usage: %s filename\n", argv[0]);
        return -1;
    }

    MYBPF_RunBareFile(argv[1], NULL, &p);

    return 0;
}

