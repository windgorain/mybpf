/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*
********************************************************/
#include "bs.h"
#include "utl/mem_utl.h"
#include "utl/qsort_utl.h"

#define CUTOFF 8



#define STKSIZ (8*sizeof(void*) - 2) 


static void _qsort_shortsort(char *lo, char *hi, size_t width, PF_CMP_FUNC cmp_func)
{
    char *p, *max;

    while (hi > lo) {
        max = lo;

        
        for (p = lo+width; p <= hi; p += width) {
            if (cmp_func(p, max) > 0) {
                max = p;
            }
        }

        
        MEM_Swap(max, hi, width);

        hi -= width;
    }
}

void QSORT_Do(void *base, int num, int width, PF_CMP_FUNC cmp_func)
{
    char *lo, *hi;              
    char *mid;                  
    char *loguy, *higuy;        
    int size;                   
    int stkptr;                 
    char *lostk[STKSIZ], *histk[STKSIZ];

    
    if (num < 2 || width == 0) {
        return;
    }

    stkptr = 0;

    lo = base;
    hi = (char *)base + width * (num-1);

    
recurse:

    
    size = ((U32)(hi - lo) / (U32)width) + 1;

    
    if (size <= CUTOFF) {
        _qsort_shortsort(lo, hi, width, cmp_func);
    }
    else {
        
        

        mid = lo + (size / 2) * width;

        
        if (cmp_func(lo, mid) > 0) {
            MEM_Swap(lo, mid, width);
        }
        if (cmp_func(lo, hi) > 0) {
            MEM_Swap(lo, hi, width);
        }
        if (cmp_func(mid, hi) > 0) {
            MEM_Swap(mid, hi, width);
        }

        
        

        
        loguy = lo; 
        higuy = hi;

        for (;;) {
            
            if (mid > loguy) {
                do  {
                    loguy += width;
                } while (loguy < mid && cmp_func(loguy, mid) <= 0);
            }

            
            if (mid <= loguy) {
                do  {
                    loguy += width;
                } while (loguy <= hi && cmp_func(loguy, mid) <= 0);
            }
 
           
           
            
            do  {
                higuy -= width;
            } while (higuy > mid && cmp_func(higuy, mid) > 0);
 
           
            if (higuy < loguy)
                break;
 
            
           
            MEM_Swap(loguy, higuy, width);
 
            

            if (mid == higuy)
                mid = loguy;
        }

        
        higuy += width;
        if (mid < higuy) {
            do  {
                higuy -= width;
            } while (higuy > mid && cmp_func(higuy, mid) == 0);
        }
        if (mid >= higuy) {
            do  {
                higuy -= width;
            } while (higuy > lo && cmp_func(higuy, mid) == 0);
        }

        
        if ( higuy - lo >= hi - loguy ) {
            if (lo < higuy) {
                lostk[stkptr] = lo;
                histk[stkptr] = higuy;
                ++stkptr;
            }

            if (loguy < hi) {
                lo = loguy;
                goto recurse;           
            }
        } else {
            if (loguy < hi) {
                lostk[stkptr] = loguy;
                histk[stkptr] = hi;
                ++stkptr;               
            }

            if (lo < higuy) {
                hi = higuy;
                goto recurse;           
            }
        }
    }

    

    --stkptr;
    if (stkptr >= 0) {
        lo = lostk[stkptr];
        hi = histk[stkptr];
        goto recurse;           
    } else {
        return;                 
    }
}

