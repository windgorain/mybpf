/*********************************************************
*   Copyright (C) LiXingang
*   Description: 用于查找多个表,将这些表的结果进行bit与操作得到结果
*                比如用于ACL的匹配
*
*********************************************************/
#include "bs.h"
#include "utl/types.h"
#include "utl/array_bit.h"
#include "utl/err_code.h"
#include "utl/bitmatch_utl.h"

int BITMATCH_Init(INOUT BITMATCH_S *ctrl)
{
    int size = (ctrl->tab_number * ctrl->max_rule_num / 8) << ctrl->seg_bits;

    if (ctrl->max_rule_num % 64 == 0) {
        BS_DBGASSERT(0);
        RETURN(BS_BAD_PARA);
    }

    if ((ctrl->seg_bits != 2) && (ctrl->seg_bits != 4) && (ctrl->seg_bits != 8)) {
        BS_DBGASSERT(0);
        RETURN(BS_BAD_PARA);
    }

    if (size <= 0) {
        BS_DBGASSERT(0);
        RETURN(BS_ERR);
    }

    memset(ctrl->rule_bits, 0, size);

    return 0;
}

BITMATCH_S * BITMATCH_Create(int tab_number, int seg_bits, int max_rule_num)
{
    int size = (tab_number * max_rule_num / 8) << seg_bits;

    if (max_rule_num % 64 == 0) {
        BS_DBGASSERT(0);
        return NULL;
    }

    if ((seg_bits != 2) && (seg_bits != 4) && (seg_bits != 8)) {
        BS_DBGASSERT(0);
        return NULL;
    }

    BITMATCH_S *ctrl = MEM_ZMalloc(sizeof(BITMATCH_S));
    if (! ctrl) {
        return NULL;
    }

    ctrl->rule_bits = MEM_ZMalloc(size);
    if (! ctrl->rule_bits) {
        MEM_Free(ctrl);
        return NULL;
    }

    ctrl->tab_number = tab_number;
    ctrl->seg_bits = seg_bits;
    ctrl->max_rule_num = max_rule_num;

    return ctrl;
}

void BITMATCH_Destroy(BITMATCH_S *ctrl)
{
    if (ctrl) {
        return;
    }

    if (ctrl->rule_bits) {
        MEM_Free(ctrl->rule_bits);
    }

    MEM_Free(ctrl);
}

void BITMATCH_AddRule(BITMATCH_S *ctrl, U32 tbl_index, U8 range_min, U8 range_max, U32 rule_id)
{
    int i;
    void *bits;

    BS_DBGASSERT(tbl_index < ctrl->tab_number);
    BS_DBGASSERT(rule_id < ctrl->max_rule_num);
    BS_DBGASSERT(range_max >= range_min);
    BS_DBGASSERT(range_max < (1 << ctrl->seg_bits));
    BS_DBGASSERT((2 == ctrl->seg_bits) || (4 == ctrl->seg_bits) || (8 == ctrl->seg_bits));

    for (i=range_min; i<=range_max; i++) {
        bits = _bitmatch_get_bits(ctrl->max_rule_num, ctrl->rule_bits, ctrl->seg_bits, tbl_index, i);
        ArrayBit_Set(bits, rule_id);
    }
}

void BITMATCH_DelRule(BITMATCH_S *ctrl, UINT rule_id)
{
    int i, j;
    void *bits;
    int line_count = (1 << ctrl->seg_bits); 

    BS_DBGASSERT(rule_id < ctrl->max_rule_num);

    for (i=0; i<ctrl->tab_number; i++) {
        for (j=0; j<line_count; j++) {
            bits = _bitmatch_get_bits(ctrl->max_rule_num, ctrl->rule_bits, ctrl->seg_bits, i, j);
            ArrayBit_Clr(bits, rule_id);
        }
    }
}

void BITMATCH_GetRuleBits(BITMATCH_S *ctrl, OUT void *rule_bits)
{
    int i, j;
    void *bits;
    int size = ctrl->max_rule_num / 8;
    int line_count = (1 << ctrl->seg_bits); 

    memset(rule_bits, 0, size);

    for (i=0; i<ctrl->tab_number; i++) {
        for (j=0; j<line_count; j++) {
            bits = _bitmatch_get_bits(ctrl->max_rule_num, ctrl->rule_bits, ctrl->seg_bits, i, j);
            ArrayBit_Or64(rule_bits, bits, ctrl->max_rule_num / 64, rule_bits);
        }
    }
}

int BITMATCH_GetRule(BITMATCH_S *ctrl, U32 rule_id, OUT void *min, OUT void *max)
{
    int tbl_index, i;
    void *bits;
    U8 *d_min, *d_max;
    int ret = -1;
    int line_count = (1 << ctrl->seg_bits); 
    int shift = 0;

    d_min = min;
    d_max = max;

    memset(d_min, 0, ctrl->tab_number*ctrl->seg_bits/8);
    memset(d_max, 0, ctrl->tab_number*ctrl->seg_bits/8);

    for (tbl_index = 0; tbl_index < ctrl->tab_number; tbl_index++) {
        int found = -1;

        
        for (i=0; i<line_count; i++) {
            shift = (tbl_index & ((8 /ctrl->seg_bits) - 1)) * ctrl->seg_bits;
            bits = _bitmatch_get_bits(ctrl->max_rule_num, ctrl->rule_bits, ctrl->seg_bits, tbl_index, i);
            if (ArrayBit_Test(bits, rule_id)) {
                d_min[tbl_index*ctrl->seg_bits/8] |= (i << shift);
                found = i;
                ret = 0;
                break;
            }
        }

        if (found < 0) {
            continue;
        }

        
        for (i=(line_count - 1); i>=found; i--) {
            shift = (tbl_index & ((8 /ctrl->seg_bits) - 1)) * ctrl->seg_bits;
            bits = _bitmatch_get_bits(ctrl->max_rule_num, ctrl->rule_bits, ctrl->seg_bits, tbl_index, i);
            if (ArrayBit_Test(bits, rule_id)) {
                d_max[tbl_index*ctrl->seg_bits/8] |= (i << shift);
                break;
            }
        }
    }

    return ret;
}


