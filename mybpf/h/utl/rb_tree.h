/*================================================================
*   Created by LiXingang
*   Description:红黑树 
*
================================================================*/
#ifndef _RB_TREE_H
#define _RB_TREE_H
#ifdef __cplusplus
extern "C"
{
#endif

#define RB_TREE_RED      0    
#define RB_TREE_BLACK    1    


typedef struct RBTreeNode {
    unsigned char color;        
    struct RBTreeNode *left;    
    struct RBTreeNode *right;    
    struct RBTreeNode *parent;    
}RB_TREE_NODE_S;


typedef struct rb_root{
    RB_TREE_NODE_S *node;
}RB_TREE_CTRL_S;

typedef int (*PF_RBTREE_CMP)(void *key, RB_TREE_NODE_S *node);
typedef int (*PF_RBTREE_WALK)(RB_TREE_NODE_S *node, void *ud);
typedef void (*PF_RBTREE_FREE)(RB_TREE_NODE_S *node, void *ud);

void RBTree_Init(IN RB_TREE_CTRL_S *root);
void RBTree_Finit(RB_TREE_CTRL_S *root, PF_RBTREE_FREE free_func, void *ud);
int RBTree_Insert(RB_TREE_CTRL_S *root, RB_TREE_NODE_S *node, void *key, PF_RBTREE_CMP cmp_func);
RB_TREE_NODE_S * RBTree_Search(RB_TREE_CTRL_S *root, void *key, PF_RBTREE_CMP cmp_func);

RB_TREE_NODE_S * RBTree_SuccessorGet(RB_TREE_CTRL_S *root, void *key, PF_RBTREE_CMP cmp_func);

RB_TREE_NODE_S * RBTree_PredecessorGet(RB_TREE_CTRL_S *root, void *key, PF_RBTREE_CMP cmp_func);
RB_TREE_NODE_S * RBTree_Del(RB_TREE_CTRL_S *root, void *key, PF_RBTREE_CMP cmp_func);
void RBTree_DelNode(RB_TREE_CTRL_S *root, RB_TREE_NODE_S *node);
RB_TREE_NODE_S * RBTree_Min(RB_TREE_CTRL_S *root);
RB_TREE_NODE_S * RBTree_Max(RB_TREE_CTRL_S *root);

void RBTree_PreOrderWalk(RB_TREE_CTRL_S *root, PF_RBTREE_WALK walk_func, void *ud);

void RBTree_InOrderWalk(RB_TREE_CTRL_S *root, PF_RBTREE_WALK walk_func, void *ud);

void RBTree_PostOrderWalk(RB_TREE_CTRL_S *root, PF_RBTREE_WALK walk_func, void *ud);

RB_TREE_NODE_S* RBTree_Next(RB_TREE_NODE_S * x);

RB_TREE_NODE_S* RBTree_Pre(RB_TREE_NODE_S * x);

#ifdef __cplusplus
}
#endif
#endif 
