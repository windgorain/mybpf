/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      LiXingang  Version: 1.0  Date: 2009-4-3
* Description: 
* History:     
******************************************************************************/

#ifndef __VLDBMP_UTL_H_
#define __VLDBMP_UTL_H_

#include "utl/endian_utl.h"

#ifdef __cplusplus
    extern "C" {
#endif 

#pragma pack(1)

typedef struct
{
    USHORT usType;      
    UINT  uiSize;      
    USHORT usReserved1;
    USHORT usReserved2;
    UINT  uiOffBits;   
}VLDBMP_HEADER_S;

typedef struct
{
    UINT  uiSize;         
    UINT  uiWidth;        
    UINT  uiHeight;       
    USHORT usPlanes;       
    USHORT usBitCount;     
    UINT  uiCompression;  
    UINT  uiSizeImage;    
    UINT uiXPelsPerMeter; 
    UINT uiYPelsPerMeter; 
    UINT uiClrUsed;       
    UINT uiClrImportant;
}VLDBMP_INFO_S;

typedef struct
{
    UCHAR ucBule;
    UCHAR ucGreen;
    UCHAR ucRed;
 }VLDBMP_DATA_S;

typedef struct
{
    VLDBMP_HEADER_S stBmpHead;
    VLDBMP_INFO_S   stInfo;
    VLDBMP_DATA_S   pstData[0];
}VLDBMP_S;

#pragma pack()

#define VLDBMP_FLAG_YWAP        0x1 
#define VLDBMP_FLAG_DRIFT       0x2 
#define VLDBMP_FLAG_RAND_COLOR  0x4 


typedef struct
{
    UINT uiLineRadii; 
    UINT uiFlag;
    UCHAR ucXCompressMin; 
    UCHAR ucXCompressMax; 
    UCHAR ucYCompressMin; 
    UCHAR ucYCompressMax; 
    UCHAR ucMaxRotate;    
}VLDBMP_OPT_S;

#define VLDBMP_SET_RADII(_pstOpt,_uiRadii) ((_pstOpt)->uiLineRadii = (_uiRadii))
#define VLDBMP_SET_MAX_ROTATE(_pstOpt,_ucRotate) ((_pstOpt)->ucMaxRotate = (_ucRotate))
#define VLDBMP_YAWP_ON(_pstOpt) ((_pstOpt)->uiFlag |= VLDBMP_FLAG_YWAP)
#define VLDBMP_YAWP_OFF(_pstOpt) ((_pstOpt)->uiFlag &= (~VLDBMP_FLAG_YWAP))
#define VLDBMP_DRIFT_ON(_pstOpt) ((_pstOpt)->uiFlag |= VLDBMP_FLAG_DRIFT)
#define VLDBMP_DRIFT_OFF(_pstOpt) ((_pstOpt)->uiFlag &= (~VLDBMP_FLAG_DRIFT))
#define VLDBMP_RAND_COLOR_ON(_pstOpt) ((_pstOpt)->uiFlag |= VLDBMP_FLAG_RAND_COLOR)
#define VLDBMP_RAND_COLOR_OFF(_pstOpt) ((_pstOpt)->uiFlag &= (~VLDBMP_FLAG_RAND_COLOR))


static inline VOID VLDBMP_SetXCompress(IN VLDBMP_OPT_S *pstOpt, IN UCHAR ucMin, IN UCHAR ucMax)
{
    pstOpt->ucXCompressMin = ucMin;
    pstOpt->ucXCompressMax = ucMax;
}

static inline VOID VLDBMP_SetYCompress(IN VLDBMP_OPT_S *pstOpt, IN UCHAR ucMin, IN UCHAR ucMax)
{
    pstOpt->ucYCompressMin = ucMin;
    pstOpt->ucYCompressMax = ucMax;
}

#define VLDBMP_SIZE(pstBmp) (Litter2Host32(pstBmp->stBmpHead.uiSize))

VOID VLDBMP_GenCode(IN UINT ulCount, OUT CHAR *pszCode);
VLDBMP_S * VLDBMP_CreateBmp
(
    IN const CHAR *pszVldCode,
    IN UINT uiWidth,
    IN UINT uiHeight,
    IN const VLDBMP_OPT_S *pstOpt 
);

VLDBMP_S * VLDBMP_CreateListBmp
(
    IN UINT uiWidth,
    IN UINT uiHeight,
    IN VLDBMP_OPT_S *pstOpt  
);
VOID VLDBMP_Destory(IN const VLDBMP_S *pstBmp);
BS_STATUS VLDBMP_SaveToFile(IN VLDBMP_S *pstBmp, IN CHAR *pszPath);

#ifdef __cplusplus
    }
#endif 

#endif 


