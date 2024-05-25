/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*
*************************************************/
#ifndef _BITMATCH_UTL_H
#define _BITMATCH_UTL_H

#include "utl/array_bit.h"
#include "utl/bit_opt.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tagBITMATCH_S {
    U16 tab_number;
    U8 seg_bits; 
    U8 reserved;
    UINT max_rule_num; 
    UCHAR *rule_bits;
}BITMATCH_S;

int BITMATCH_Init(INOUT BITMATCH_S *ctrl);
BITMATCH_S * BITMATCH_Create(int tab_number, int seg_bits, int max_rule_num);
void BITMATCH_Destroy(BITMATCH_S *ctrl);
void BITMATCH_AddRule(BITMATCH_S *ctrl, UINT tbl_index, UCHAR range_min, UCHAR range_max, UINT rule_id);
void BITMATCH_DelRule(BITMATCH_S *ctrl, UINT rule_id);
void BITMATCH_GetRuleBits(BITMATCH_S *ctrl, OUT void *rule_bits);
int BITMATCH_GetRule(BITMATCH_S *ctrl, U32 rule_id, OUT void *min, OUT void *max);


static inline UCHAR * _bitmatch_get_bits(U32 max_rule, U8 *rules_bits, int seg_bits, int tbl_index, int pos)
{
    UCHAR *bits;

    
    bits = rules_bits + (((max_rule / 8) << seg_bits) * tbl_index);

    
    bits += ((max_rule/ 8) * pos);

    return bits;
}


static inline U64 _bitmatch_get_u64(U32 max_rule, void *rules_bits, int seg_bits, int tbl_index, int pos, int offset)
{
    U64 * d = (void*)_bitmatch_get_bits(max_rule, rules_bits, seg_bits, tbl_index, pos);
    return d[offset];
}


static inline INT64 _bitmatch_match_first2(U32 tab_num, U32 max_rule, int seg_bits, void *rule_bits, void *data)
{
    int i;
    int offset;
    UCHAR *d = data;
    U64 tmp;

    for (offset=0; offset<max_rule/64; offset++) {
        tmp = _bitmatch_get_u64(max_rule, rule_bits, seg_bits, 0, d[0] & 0x3, offset);
        tmp &= _bitmatch_get_u64(max_rule, rule_bits, seg_bits, 1, (d[0] >> 2) & 0x3, offset);
        tmp &= _bitmatch_get_u64(max_rule, rule_bits, seg_bits, 2, (d[0] >> 4) & 0x3, offset);
        tmp &= _bitmatch_get_u64(max_rule, rule_bits, seg_bits, 3, (d[0] >> 6) & 0x3, offset);
        for (i=1; i<(tab_num/4); i++) {
            tmp &= _bitmatch_get_u64(max_rule, rule_bits, seg_bits, 4*i, d[i] & 0x3, offset);
            tmp &= _bitmatch_get_u64(max_rule, rule_bits, seg_bits, (4*i)+1, (d[i] >> 2) & 0x3, offset);
            tmp &= _bitmatch_get_u64(max_rule, rule_bits, seg_bits, (4*i)+2, (d[i] >> 4) & 0x3, offset);
            tmp &= _bitmatch_get_u64(max_rule, rule_bits, seg_bits, (4*i)+3, (d[i] >> 6) & 0x3, offset);
            if (! tmp) {
                break;
            }
        }
        if (tmp) {
            return (offset * 64) + BIT_GetLowIndex64(tmp);
        }
    }

    return -1;
}


static inline INT64 _bitmatch_match_first4(U32 tab_num, U32 max_rule, int seg_bits, void *rule_bits, void *data)
{
    int i;
    int offset;
    UCHAR *d = data;
    U64 tmp;

    for (offset=0; offset<max_rule/64; offset++) {
        tmp = _bitmatch_get_u64(max_rule, rule_bits, seg_bits, 0, d[0] & 0xf, offset);
        tmp &= _bitmatch_get_u64(max_rule, rule_bits, seg_bits, 1, (d[0] >> 4) & 0xf, offset);
        for (i=1; i<(tab_num/2); i++) {
            tmp &= _bitmatch_get_u64(max_rule, rule_bits, seg_bits, 2*i, d[i] & 0xf, offset);
            tmp &= _bitmatch_get_u64(max_rule, rule_bits, seg_bits, (2*i)+1, (d[i] >> 4) & 0xf, offset);
            if (! tmp) {
                break;
            }
        }
        if (tmp) {
            return (offset * 64) + BIT_GetLowIndex64(tmp);
        }
    }

    return -1;
}


static inline INT64 _bitmatch_match_first8(U32 tab_num, U32 max_rule, int seg_bits, void *rule_bits, void *data)
{
    int i;
    int offset;
    UCHAR *d = data;
    U64 tmp;

    for (offset=0; offset<max_rule/64; offset++) {
        tmp = _bitmatch_get_u64(max_rule, rule_bits, seg_bits, 0, d[0], offset);
        for (i=1; i<tab_num; i++) {
            tmp &= _bitmatch_get_u64(max_rule, rule_bits, seg_bits, i, d[i], offset);
            if (! tmp) {
                break;
            }
        }
        if (tmp) {
            return (offset * 64) + BIT_GetLowIndex64(tmp);
        }
    }

    return -1;
}

static inline int _bitmatch_match2(U32 tab_num, U32 max_rule, void *rule_bits, int seg_bits, void *data, OUT void *matched_bits)
{
    int i;
    UCHAR *d = data;
    void *bits;
    int size = max_rule / 8;

    bits = _bitmatch_get_bits(max_rule, rule_bits, seg_bits, 0, d[0] & 0x3);
    memcpy(matched_bits, bits, size);
    bits = _bitmatch_get_bits(max_rule, rule_bits, seg_bits, 0, (d[0] >> 2) & 0x3);
    ArrayBit_And64(matched_bits, bits, max_rule / 64, matched_bits);
    bits = _bitmatch_get_bits(max_rule, rule_bits, seg_bits, 0, (d[0] >> 4) & 0x3);
    ArrayBit_And64(matched_bits, bits, max_rule / 64, matched_bits);
    bits = _bitmatch_get_bits(max_rule, rule_bits, seg_bits, 0, (d[0] >> 6) & 0x3);
    ArrayBit_And64(matched_bits, bits, max_rule / 64, matched_bits);

    for (i=0; i<tab_num; i++) {
        bits = _bitmatch_get_bits(max_rule, rule_bits, seg_bits, i, d[i] & 0x3);
        ArrayBit_And64(matched_bits, bits, max_rule / 64, matched_bits);
        bits = _bitmatch_get_bits(max_rule, rule_bits, seg_bits, i, (d[i] >> 2) & 0x3);
        ArrayBit_And64(matched_bits, bits, max_rule / 64, matched_bits);
        bits = _bitmatch_get_bits(max_rule, rule_bits, seg_bits, i, (d[i] >> 4) & 0x3);
        ArrayBit_And64(matched_bits, bits, max_rule / 64, matched_bits);
        bits = _bitmatch_get_bits(max_rule, rule_bits, seg_bits, i, (d[i] >> 6) & 0x3);
        ArrayBit_And64(matched_bits, bits, max_rule / 64, matched_bits);
    }

    return 0;
}

static inline int _bitmatch_match4(U32 tab_num, U32 max_rule, void *rule_bits, int seg_bits, void *data, OUT void *matched_bits)
{
    int i;
    UCHAR *d = data;
    void *bits;
    int size = max_rule / 8;

    bits = _bitmatch_get_bits(max_rule, rule_bits, seg_bits, 0, d[0] & 0xf);
    memcpy(matched_bits, bits, size);
    bits = _bitmatch_get_bits(max_rule, rule_bits, seg_bits, 0, (d[0] >> 4) & 0xf);
    ArrayBit_And64(matched_bits, bits, max_rule / 64, matched_bits);

    for (i=0; i<tab_num; i++) {
        bits = _bitmatch_get_bits(max_rule, rule_bits, seg_bits, i, d[i] & 0xf);
        ArrayBit_And64(matched_bits, bits, max_rule / 64, matched_bits);
        bits = _bitmatch_get_bits(max_rule, rule_bits, seg_bits, i, (d[i] >> 4) & 0xf);
        ArrayBit_And64(matched_bits, bits, max_rule / 64, matched_bits);
    }

    return 0;
}

static inline int _bitmatch_match8(U32 tab_num, U32 max_rule, void *rule_bits, int seg_bits, void *data, OUT void *matched_bits)
{
    int i;
    UCHAR *d = data;
    void *bits;
    int size = max_rule / 8;

    bits = _bitmatch_get_bits(max_rule, rule_bits, seg_bits, 0, d[0]);

    memcpy(matched_bits, bits, size);

    for (i=0; i<tab_num; i++) {
        bits = _bitmatch_get_bits(max_rule, rule_bits, seg_bits, i, d[i]);
        ArrayBit_And64(matched_bits, bits, max_rule / 64, matched_bits);
    }

    return 0;
}


static inline INT64 BITMATCH_MatchFirst(U32 tab_num, U32 max_rule, void *rule_bits, int seg_bits, void *data)
{
    if (seg_bits == 2) {
        return _bitmatch_match_first2(tab_num, max_rule, seg_bits, rule_bits, data);
    } else if (seg_bits == 4) {
        return _bitmatch_match_first4(tab_num, max_rule, seg_bits, rule_bits, data);
    } if (seg_bits == 8) {
        return _bitmatch_match_first8(tab_num, max_rule, seg_bits, rule_bits, data);
    } else {
        return -1;
    }
}

static inline int BITMATCH_Match(U32 tab_num, U32 max_rule, void *rule_bits, int seg_bits, void *data, OUT void *matched_bits)
{
    if (seg_bits == 2) {
        return _bitmatch_match2(tab_num, max_rule, rule_bits, seg_bits, data, matched_bits);
    } else if (seg_bits == 4) {
        return _bitmatch_match4(tab_num, max_rule, rule_bits, seg_bits, data, matched_bits);
    } else if (seg_bits == 8) {
        return _bitmatch_match8(tab_num, max_rule, rule_bits, seg_bits, data, matched_bits);
    } else {
        return -1;
    }
}


static inline int BITMATCH_DoMatch(BITMATCH_S *ctrl, void *data, OUT void *matched_bits)
{
    return BITMATCH_Match(ctrl->tab_number, ctrl->max_rule_num, ctrl->rule_bits, ctrl->seg_bits, data, matched_bits);
}

#ifdef __cplusplus
}
#endif
#endif 
