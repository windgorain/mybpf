/*********************************************************
*   Copyright (C) LiXingang
*   Description: 
*
********************************************************/
#include "bs.h"
#include "utl/pci_utl.h"
#include "utl/pci_tag.h"

void PCIE_TAG_Init(OUT PCIE_TAG_S *tags)
{
    int i;

    memset(tags, 0, sizeof(PCIE_TAG_S));

    for (i=0; i<PCIE_TAG_MAX; i++) {
        tags->node[i].sem = SEM_CCreate("pcie_tag", 0);
    }

    MUTEX_Init(&tags->lock);
}

static int pcie_tag_alloc_tag(PCIE_TAG_S *tags)
{
    UCHAR tag;

    tag = tags->tag_begin;

    do {
        if (! tags->node[tag].used) {
            tags->node[tag].used = 1;
            tags->tag_begin = tag + 1;
            return tag;
        }
        tag ++;
    } while(tag != tags->tag_begin);

    return -1;
}

int PCIE_TAG_AllocTag(PCIE_TAG_S *tags)
{
    int ret;
    MUTEX_P(&tags->lock);
    ret = pcie_tag_alloc_tag(tags);
    MUTEX_V(&tags->lock);

    return ret;
}

static void pcie_tag_free_tag(PCIE_TAG_S *tags, int tag)
{
    tags->node[tag].used = 0;
}

void PCIE_TAG_FreeTag(PCIE_TAG_S *tags, int tag)
{
    MUTEX_P(&tags->lock);
    pcie_tag_free_tag(tags, tag);
    MUTEX_V(&tags->lock);
}

static int pcie_tag_wait(PCIE_TAG_S *tags, int tag, UINT timeout_ms)
{
    return SEM_P(tags->node[tag].sem, BS_WAIT, timeout_ms);
}

static void pcie_tag_wakeup(PCIE_TAG_S *tags, int tag)
{
    SEM_V(tags->node[tag].sem);
}

int PCIE_TAG_WriteTlp(PCIE_TAG_S *tags, void *tlp, int tlp_len)
{
    PCIE_TLP_COMPLETE_S *complete_tlp = tlp;
    int tag = complete_tlp->tag;
    int len = MIN(tlp_len, sizeof(PCIE_TLP_S));

    memcpy(&tags->node[tag].tlp, tlp, len);

    pcie_tag_wakeup(tags, tag);

    return 0;
}

PCIE_TLP_S * PCIE_TAG_WaitTlp(PCIE_TAG_S *tags, int tag, UINT timeout_ms)
{
    if (pcie_tag_wait(tags, tag, timeout_ms) < 0) {
        return NULL;
    }

    return &tags->node[tag].tlp;
}

