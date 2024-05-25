/*================================================================
*   Created by LiXingang: 2018.11.17
*   Description: 
*
================================================================*/
#ifdef IN_UNIXLIKE
#include <execinfo.h>
#endif

#include "bs.h"

#ifdef IN_LINUX
#include <sys/prctl.h>
#endif

#define MAX_DEPTH 128

typedef int (*PF_BACKTRACE_PRINT)(char *info, void *ud);

static void _backtrace_call(PF_BACKTRACE_PRINT func, void *ud)
{
#ifdef USE_BACKTRACE
    void *buffer[MAX_DEPTH];
    int nptrs = backtrace(buffer, MAX_DEPTH);
    char **stack = backtrace_symbols(buffer, nptrs);
    int i;
    char info[256];
    
    if (stack) {
        for (i = 0; i < nptrs; ++i) {
            scnprintf(info, sizeof(info), "%s \r\n", stack[i]);
            func(info, ud);
        }

        free(stack);
    } else {
        for (i = 0; i < nptrs; ++i) {
            scnprintf(info, sizeof(info), "%p \r\n", buffer[i]);
            func(info, ud);
        }
    }
#endif
}

static int _backtrace_print(char *info, void *ud)
{
    printf("%s", info);
    return 0;
}

static int _backtrace_write_fp(char *info, void *ud)
{
    FILE *fp = ud;
    fprintf(fp, "%s", info);
    return 0;
}

static int _backtrace_write_fd(char *info, void *ud)
{
    int fd = HANDLE_UINT(ud);
    return write(fd, info, strlen(info));
}

static int _backtrace_write_buf(char *info, void *ud)
{
    LSTR_S *lstr = ud;
    
    if (lstr->uiLen == 0) {
        return -1;
    }

    int n = snprintf(lstr->pcData, lstr->uiLen, "%s", info);

    if (n >= lstr->uiLen) {
        lstr->uiLen = 0;
    } else {
        lstr->pcData += n;
        lstr->uiLen -= n;
    }

    return 0;
}

void BackTrace_Print(void)
{
    _backtrace_call(_backtrace_print, NULL);
}

void BackTrace_WriteToFile(char *file)
{
    FILE *fp = fopen(file, "w+");
    if (!fp) {
        return;
    }

    _backtrace_call(_backtrace_write_fp, fp);

    fclose(fp);
}

void BackTrace_WriteToFp(FILE *fp)
{
    _backtrace_call(_backtrace_write_fp, fp);
}

void BackTrace_WriteToFd(int fd)
{
    _backtrace_call(_backtrace_write_fd, UINT_HANDLE(fd));
}

void BackTrace_WriteToBuf(OUT char *buf, int buf_size)
{
    LSTR_S tmp;

    tmp.pcData = (void*)buf;
    tmp.uiLen = buf_size;

    _backtrace_call(_backtrace_write_buf, &tmp);
}

void CoreDump_Enable(void)
{
#ifdef IN_LINUX
    prctl(PR_SET_DUMPABLE, 1);
#endif
    return;
}

