/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _VECTOR_UTL_H
#define _VECTOR_UTL_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct tagVECTOR_S {
    void *memcap;
	char *array;
	UINT count; 
	UINT size;  
	UINT ele_size; 
	UINT resize_factor;
	UINT sorted:1; 
}VECTOR_S;

typedef struct {
    UINT ele_size;
    UINT resize_factor;
    UINT init_size;
    void *memcap;
}VECTOR_PARAM_S;

int VECTOR_Init(VECTOR_S *vec, VECTOR_PARAM_S *p);
void VECTOR_Finit(VECTOR_S *vec);
VECTOR_S * VECTOR_Create(VECTOR_PARAM_S *p);
void VECTOR_Destroy(VECTOR_S *vec);

int VECTOR_AddBySize(VECTOR_S *vec, void *val, int val_size);
int VECTOR_Add(VECTOR_S *vec, void *val);
int VECTOR_AddN(VECTOR_S *vec, void *val, UINT num);
int VECTOR_Append(VECTOR_S *vec_dst, VECTOR_S *vec_src);
void VECTOR_Sort(VECTOR_S *vec, PF_CMP_FUNC cmp_func);
void VECTOR_Remove(VECTOR_S *vec, UINT pos);
void VECTOR_Delete(VECTOR_S *vec, void *value, PF_CMP_FUNC cmp_func);
int VECTOR_Find(VECTOR_S *vec, void *value, PF_CMP_FUNC cmp_func);
void * VECTOR_Get(VECTOR_S *vec, int idx);
int VECTOR_Copy(VECTOR_S *vec, int idx, OUT void *val);

static inline UINT VECTOR_Count(VECTOR_S *vec)
{
    if (! vec) {
        BS_DBGASSERT(0);
        return 0;
    }

    return vec->count;
}

#ifdef __cplusplus
}
#endif
#endif 
