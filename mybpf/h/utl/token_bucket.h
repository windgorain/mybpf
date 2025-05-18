/*================================================================
*   Created by LiXingang
*   Version: 1.0  Date: 2007-10-3
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
    U64 speed;       
    U64 burst_limit; 
    U64 token_count; 
    U64 ts;      
}TOKEN_BUCKET_S;

void TokenBucket_Init(INOUT TOKEN_BUCKET_S *bucket);
void TokenBucket_Set(OUT TOKEN_BUCKET_S *bucket, U64 speed, U64 burst);
U64 TokenBucket_Get(TOKEN_BUCKET_S *bucket, U64 n);
U64 TokenBucket_ExertGet(TOKEN_BUCKET_S *bucket, U64 n);

#ifdef __cplusplus
}
#endif
#endif 
