/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0  Date: 2007-8-18
* Description: 
* History:     
******************************************************************************/

#ifndef __AVLLIB_UTL_H_
#define __AVLLIB_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 


typedef struct {
    VOID *  left;   
    VOID *  right;  
    INT     height; 
} AVL_NODE;


typedef struct {
    AVL_NODE    avl;
    UINT        key;
}AVL_UNSIGNED_NODE;

typedef AVL_NODE * AVL_TREE;    

typedef VOID (*PF_AVL_FREE_FUNC)(IN VOID *pNode, IN VOID *pUserHandle);
typedef int (*PF_AVL_CMP_FUNC)(IN void *key, IN void *node);
typedef int (*PF_AVL_WALK_FUNC)(void *node, void *ud);
typedef void (*PF_AVL_PRINT_FUNC)(void *nodep);



void avlRebalance (AVL_NODE *** ancestors, INT count);

void * avlSearch (AVL_TREE root, IN void *key, PF_AVL_CMP_FUNC cmp_func);

void * avlSuccessorGet (AVL_TREE root, IN void *key, PF_AVL_CMP_FUNC cmp_func);

void * avlPredecessorGet (AVL_TREE root, IN void *key, PF_AVL_CMP_FUNC cmp_func);

void * avlMinimumGet (AVL_TREE root);

void * avlMaximumGet (AVL_TREE root);

int avlInsert (AVL_TREE *root, void *newNode, void *key, PF_AVL_CMP_FUNC cmp_func);

int avlInsertInform (AVL_TREE *pRoot, void *pNewNode, void *key, void **ppKeyHolder, PF_AVL_CMP_FUNC cmp_func);

void * avlRemoveInsert (AVL_TREE * pRoot, void *pNewNode, void *key, PF_AVL_CMP_FUNC cmp_func);

void * avlDelete(AVL_TREE *root, void *key, PF_AVL_CMP_FUNC cmp_func);

int avlTreeWalk(AVL_TREE *pRoot, PF_AVL_WALK_FUNC walk_func, void *ud);

BS_STATUS avlTreePrint(AVL_TREE * pRoot, PF_AVL_PRINT_FUNC print_func);

BS_STATUS avlTreeErase(AVL_TREE * pRoot, PF_AVL_FREE_FUNC pfFree, void *ud);

BS_STATUS avlTreePrintErase(AVL_TREE * pRoot, PF_AVL_PRINT_FUNC print_func);



VOID * avlSearchUnsigned (AVL_TREE root, UINT key);

BS_STATUS avlInsertUnsigned (AVL_TREE * root, VOID * newNode);

VOID * avlDeleteUnsigned (AVL_TREE * root, UINT key);


#ifdef __cplusplus
    }
#endif 

#endif 

