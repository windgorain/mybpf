/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#ifndef _HOLE_DATA_H
#define _HOLE_DATA_H
#include "utl/bitmap_utl.h"
#include "utl/mem_cap.h"
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    void *data;
    int max_size; 
    int filled_size;  
    BITMAP_S bitmap;
}HOLE_DATA_S;


int HoleData_Init(HOLE_DATA_S *hole_data, void *data, int max_size, void *bits);
void HoleData_Final(HOLE_DATA_S *hole_data);
HOLE_DATA_S * HoleData_Create(int max_size, MEM_CAP_S *mem_cap);
void HoleData_Destroy(HOLE_DATA_S *hole_data);
void HoleData_Reset(HOLE_DATA_S *hole_data);
int HoleData_Input(HOLE_DATA_S *hole_data, void *data, int offset, int len);
BOOL_T HoleData_IsFinished(HOLE_DATA_S *hole_data);

int HoleData_GetContineLen(HOLE_DATA_S *hole_data);
int HoleData_GetFilledSize(HOLE_DATA_S *hole_data);

#ifdef __cplusplus
}
#endif
#endif 
