/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _TOKEN_BUCKET_H
#define _TOKEN_BUCKET_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    uint64_t burst_limit; 
    uint64_t speed;       
    uint64_t token_count; 
    uint64_t ts;      
}TOKEN_BUCKET_S;

void TokenBucket_Init(INOUT TOKEN_BUCKET_S *bucket, uint64_t burst, uint64_t speed);
uint64_t TokenBucket_Acquire(TOKEN_BUCKET_S *bucket, uint64_t n);

#ifdef __cplusplus
}
#endif
#endif 
