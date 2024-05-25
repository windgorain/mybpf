/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _TRACE_PIPE_H
#define _TRACE_PIPE_H

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define DEBUGFS "/sys/kernel/debug/tracing/"
static inline void my_read_trace_pipe(void)
{
	int trace_fd;

	trace_fd = open(DEBUGFS "trace_pipe", O_RDONLY, 0);
	if (trace_fd < 0)
		return;

	while (1) {
		static char buf[4096];
		ssize_t sz;

		sz = read(trace_fd, buf, sizeof(buf));
		if (sz > 0) {
			buf[sz] = 0;
			printf("%s", buf);
		}
	}
}



#ifdef __cplusplus
}
#endif
#endif
