/* retcode所需要的宏 */
#define RETCODE_FILE_NUM RETCODE_FILE_NUM_AVL

#include "bs.h"

#include "utl/mem_utl.h"
#include "utl/avllib_utl.h"



#define AVL_MAX_HEIGHT 42 



void avlRebalance
(
    AVL_NODE ***    ancestors,  
    int         count       
)
{
    while (count > 0)
    {
    AVL_NODE ** nodepp; 
    AVL_NODE *  nodep;  
    AVL_NODE *  leftp;  
    int     lefth;  
    AVL_NODE *  rightp; 
    int     righth; 

    

    nodepp = ancestors[--count];
    nodep = *nodepp;
    leftp = nodep->left;
    lefth = (leftp != NULL) ? leftp->height : 0;
    rightp = nodep->right;
    righth = (rightp != NULL) ? rightp->height : 0;

    if (righth - lefth < -1)
    {
        

        AVL_NODE *  leftleftp;  
        AVL_NODE *  leftrightp; 
        int     leftrighth; 

        leftleftp = leftp->left;
        leftrightp = leftp->right;
        leftrighth = (leftrightp != NULL) ? leftrightp->height : 0;

        if ((leftleftp != NULL) && (leftleftp->height >= leftrighth))
        {
        

        nodep->left = leftrightp;   
        nodep->height = leftrighth + 1;
        leftp->right = nodep;       
        leftp->height = leftrighth + 2;
        *nodepp = leftp;        
        }
        else
        {
        

        leftp->right = leftrightp->left;    
        leftp->height = leftrighth;
        nodep->left = leftrightp->right;    
        nodep->height = leftrighth;
        leftrightp->left = leftp;       
        leftrightp->right = nodep;      
        leftrightp->height = leftrighth + 1;
        *nodepp = leftrightp;           
        }
        }
    else if (righth - lefth > 1)
    {
        

        AVL_NODE *  rightleftp; 
        int     rightlefth; 
        AVL_NODE *  rightrightp;    

        rightleftp = rightp->left;
        rightlefth = (rightleftp != NULL) ? rightleftp->height : 0;
        rightrightp = rightp->right;

        if ((rightrightp != NULL) && (rightrightp->height >= rightlefth))
        {
        

        nodep->right = rightleftp;  
        nodep->height = rightlefth + 1;
        rightp->left = nodep;       
        rightp->height = rightlefth + 2;
        *nodepp = rightp;       
        }
        else
        {
        

        nodep->right = rightleftp->left;    
        nodep->height = rightlefth;
        rightp->left = rightleftp->right;   
        rightp->height = rightlefth;
        rightleftp->left = nodep;       
        rightleftp->right = rightp;     
        rightleftp->height = rightlefth + 1;
        *nodepp = rightleftp;           
        }
        }
    else
    {
        

        int height;

        height = ((righth > lefth) ? righth : lefth) + 1;
        if (nodep->height == height)
        break;
        nodep->height = height;
        }
    }
}



void * avlSearch(AVL_TREE root, void *key, PF_AVL_CMP_FUNC cmp_func)
{
    AVL_NODE *  nodep;  

    nodep = root;
    while (1)
    {
        int delta;  

        if (nodep == NULL)
            return NULL;    

        delta = cmp_func(key, nodep);
        if (0 == delta)
            return nodep;   
        else if (delta < 0)
            nodep = nodep->left;
        else
            nodep = nodep->right;
    }
}



void * avlSearchUnsigned(AVL_TREE root, UINT key)
{
    AVL_UNSIGNED_NODE * nodep;  

    nodep = (AVL_UNSIGNED_NODE *) root;
    while (1)
    {
        if (nodep == NULL)
            return NULL;    

        if (key == nodep->key)
            return nodep;   
        else if (key < nodep->key)
            nodep = nodep->avl.left;
        else
            nodep = nodep->avl.right;
    }
}



int avlInsert (AVL_TREE *root, void *newNode, void *key, PF_AVL_CMP_FUNC cmp_func)
{
    AVL_NODE ** nodepp;             
    AVL_NODE ** ancestor[AVL_MAX_HEIGHT];   
    int ancestorCount;          

    nodepp = root;
    ancestorCount = 0;

    while (1) {
        AVL_NODE *  nodep;  
        int     delta;  

        nodep = *nodepp;
        if (nodep == NULL)
            break;  

        ancestor[ancestorCount++] = nodepp;

        delta = cmp_func(key, nodep);
        if (0 == delta)
            RETURN(BS_ALREADY_EXIST);
        else if (delta < 0)
            nodepp = (AVL_NODE **)&(nodep->left);
        else
            nodepp = (AVL_NODE **)&(nodep->right);
    }

    ((AVL_NODE *)newNode)->left = NULL;
    ((AVL_NODE *)newNode)->right = NULL;
    ((AVL_NODE *)newNode)->height = 1;
    *nodepp = newNode;

    avlRebalance (ancestor, ancestorCount);

    return BS_OK;
}



int avlInsertUnsigned(AVL_TREE *root, void *newNode)
{
    AVL_UNSIGNED_NODE **nodepp; 
    AVL_UNSIGNED_NODE **ancestor[AVL_MAX_HEIGHT];
            
    int ancestorCount;      
    UINT key;

    key = ((AVL_UNSIGNED_NODE *)newNode)->key;
    nodepp = (AVL_UNSIGNED_NODE **) root;
    ancestorCount = 0;

    while (1) {
        AVL_UNSIGNED_NODE * nodep;  

        nodep = *nodepp;
        if (nodep == NULL)
            break;  

        ancestor[ancestorCount++] = nodepp;

        if (key == nodep->key)
            RETURN(BS_ERR);
        else if (key < nodep->key)
            nodepp = (AVL_UNSIGNED_NODE **)&(nodep->avl.left);
        else
            nodepp = (AVL_UNSIGNED_NODE **)&(nodep->avl.right);
    }

    ((AVL_NODE *)newNode)->left = NULL;
    ((AVL_NODE *)newNode)->right = NULL;
    ((AVL_NODE *)newNode)->height = 1;
    *nodepp = newNode;

    avlRebalance ((AVL_NODE ***)ancestor, ancestorCount);

    return BS_OK;
}



void * avlDelete(AVL_TREE *root, void *key, PF_AVL_CMP_FUNC cmp_func)
{
    AVL_NODE ** nodepp;             
    AVL_NODE *  nodep;              
    AVL_NODE ** ancestor[AVL_MAX_HEIGHT];   
    int ancestorCount;              
    AVL_NODE *  deletep;            

    nodepp = root;
    ancestorCount = 0;
    while (1)
    {
        int delta;      

        nodep = *nodepp;
        if (nodep == NULL)
            return NULL;    

        ancestor[ancestorCount++] = nodepp;

        delta = cmp_func(key, nodep);
        if (0 == delta)
            break;      
        else if (delta < 0)
            nodepp = (AVL_NODE **)&(nodep->left);
        else
            nodepp = (AVL_NODE **)&(nodep->right);
    }

    deletep = nodep;

    if (nodep->left == NULL) {
        

        *nodepp = nodep->right;

        

        ancestorCount--;    
    } else {
        

        AVL_NODE ** deletepp;       
        int     deleteAncestorCount;    

        deleteAncestorCount = ancestorCount;
        deletepp = nodepp;
        deletep = nodep;

        

        nodepp = (AVL_NODE **)&(nodep->left);
        while (1)
        {
            nodep = *nodepp;
            if (nodep->right == NULL)
                break;
            ancestor[ancestorCount++] = nodepp;
            nodepp = (AVL_NODE **)&(nodep->right);
        }

        

        *nodepp = nodep->left;

        

        nodep->left = deletep->left;
        nodep->right = deletep->right;
        nodep->height = deletep->height;
        *deletepp = nodep;

        

        ancestor[deleteAncestorCount] = (AVL_NODE **)&(nodep->left);
    }

    avlRebalance (ancestor, ancestorCount);

    return deletep;
}



void * avlDeleteUnsigned(AVL_TREE *root, UINT key)
{
    AVL_UNSIGNED_NODE **    nodepp;     
    AVL_UNSIGNED_NODE *     nodep;      
    AVL_UNSIGNED_NODE **    ancestor[AVL_MAX_HEIGHT];
    
    int             ancestorCount;  
    AVL_UNSIGNED_NODE *     deletep;    

    nodepp = (AVL_UNSIGNED_NODE **)root;
    ancestorCount = 0;
    while (1) {
        nodep = *nodepp;
        if (nodep == NULL)
            return NULL;    

        ancestor[ancestorCount++] = nodepp;

        if (key == nodep->key)
            break;      
        else if (key < nodep->key)
            nodepp = (AVL_UNSIGNED_NODE **)&(nodep->avl.left);
        else
            nodepp = (AVL_UNSIGNED_NODE **)&(nodep->avl.right);
    }

    deletep = nodep;

    if (nodep->avl.left == NULL) {
        

        *nodepp = nodep->avl.right;

        

        ancestorCount--;    
    } else {
        

        AVL_UNSIGNED_NODE **    deletepp;   
        int deleteAncestorCount;    

        deleteAncestorCount = ancestorCount;
        deletepp = nodepp;
        deletep = nodep;

        

        nodepp = (AVL_UNSIGNED_NODE **)&(nodep->avl.left);
        while (1) {
            nodep = *nodepp;
            if (nodep->avl.right == NULL)
                break;
            ancestor[ancestorCount++] = nodepp;
            nodepp = (AVL_UNSIGNED_NODE **)&(nodep->avl.right);
        }

        

        *nodepp = nodep->avl.left;

        

        nodep->avl.left = deletep->avl.left;
        nodep->avl.right = deletep->avl.right;
        nodep->avl.height = deletep->avl.height;
        *deletepp = nodep;

        

        ancestor[deleteAncestorCount] = (AVL_UNSIGNED_NODE **)&(nodep->avl.left);
    }

    avlRebalance ((AVL_NODE ***)ancestor, ancestorCount);

    return deletep;
}



void * avlSuccessorGet(AVL_TREE root, IN void *key, PF_AVL_CMP_FUNC cmp_func)
{
    AVL_NODE *  nodep;  
    AVL_NODE *  superiorp;  

    nodep = root;
    superiorp = NULL;
    while (1) {
        int delta;  

        if (nodep == NULL)
            return superiorp;

        delta = cmp_func(key, nodep);
        if (delta < 0) {
            superiorp = nodep; 
            nodep = nodep->left;
        } else {
            nodep = nodep->right;
        }
    }
}



VOID * avlPredecessorGet (AVL_TREE root, IN void *key, PF_AVL_CMP_FUNC cmp_func)
{
    AVL_NODE *  nodep;  
    AVL_NODE *  inferiorp;  

    nodep = root;
    inferiorp = NULL;

    while (1) {
        int delta;  

        if (nodep == NULL)
            return inferiorp;

        delta = cmp_func(key, nodep);
        if (delta > 0) {
            inferiorp = nodep; 
            nodep = nodep->right;
        } else {
            nodep = nodep->left;
        }
    }
}



void * avlMinimumGet(AVL_TREE root)
{
    if  (NULL == root)
        return NULL;

    while (root->left != NULL) {
        root = root->left;
    }

    return root;
}



void * avlMaximumGet (AVL_TREE root)
{
    if  (NULL == root)
        return NULL;

    while (root->right != NULL) {
        root = root->right;
    }

    return root;
}



int avlInsertInform (AVL_TREE *pRoot, void *pNewNode, void *key, void **ppKeyHolder, PF_AVL_CMP_FUNC cmp_func)
{
    AVL_NODE ** nodepp;             
    AVL_NODE ** ancestor[AVL_MAX_HEIGHT];   
    int     ancestorCount;          

    if  (NULL == ppKeyHolder) {
        RETURN(BS_ERR);
    }

    nodepp = pRoot;
    ancestorCount = 0;

    while (1) {
        AVL_NODE *  nodep;  
        int     delta;  

        nodep = *nodepp;
        if (nodep == NULL)
            break;  

        ancestor[ancestorCount++] = nodepp;

        delta = cmp_func(key, nodep);
        if  (0 == delta) {
            
            *ppKeyHolder = nodep;
            RETURN(BS_ERR);
        } else if (delta < 0) {
            nodepp = (AVL_NODE **)&(nodep->left);
        } else {
            nodepp = (AVL_NODE **)&(nodep->right);
        }
    }

    ((AVL_NODE *)pNewNode)->left = NULL;
    ((AVL_NODE *)pNewNode)->right = NULL;
    ((AVL_NODE *)pNewNode)->height = 1;
    *nodepp = pNewNode;

    *ppKeyHolder = pNewNode;

    avlRebalance (ancestor, ancestorCount);

    return BS_OK;
}



void * avlRemoveInsert (AVL_TREE * pRoot, void *pNewNode, void *key, PF_AVL_CMP_FUNC cmp_func)
{
    AVL_NODE ** nodepp;             
    AVL_NODE ** ancestor[AVL_MAX_HEIGHT];   
    int     ancestorCount;          

    nodepp = pRoot;
    ancestorCount = 0;

    while (1) {
        AVL_NODE *  nodep;  
        int     delta;  

        nodep = *nodepp;
        if (nodep == NULL)
            break;  

        ancestor[ancestorCount++] = nodepp;

        delta = cmp_func(key, nodep);
        if  (0 == delta) {
            
            ((AVL_NODE *)pNewNode)->left = nodep->left;
            ((AVL_NODE *)pNewNode)->right = nodep->right;
            ((AVL_NODE *)pNewNode)->height = nodep->height;

            
            *nodepp = pNewNode;

            
            nodep->left = NULL;
            nodep->right = NULL;
            nodep->height = 1;

            return nodep;
        }
        else if (delta < 0)
            nodepp = (AVL_NODE **)&(nodep->left);
        else
            nodepp = (AVL_NODE **)&(nodep->right);
    }

    ((AVL_NODE *)pNewNode)->left = NULL;
    ((AVL_NODE *)pNewNode)->right = NULL;
    ((AVL_NODE *)pNewNode)->height = 1;
    *nodepp = pNewNode;

    avlRebalance (ancestor, ancestorCount);

    return NULL;
}




int avlTreeWalk(AVL_TREE * pRoot, PF_AVL_WALK_FUNC walk_func, void *ud)
{
    int ret;

    if  ((NULL == pRoot) || (NULL == *pRoot)) {
        return 0;
    }

    if  (!(NULL == (*pRoot)->left)) {
        ret = avlTreeWalk((AVL_TREE *)(&((*pRoot)->left)), walk_func, ud);
        if (ret < 0) {
            return ret;
        }
    }

    ret = walk_func(*pRoot, ud);
    if (ret < 0) {
        return ret;
    }

    if  (NULL == (*pRoot)->right) {
        return 0;
    }

    return avlTreeWalk((AVL_TREE *)(&((*pRoot)->right)), walk_func, ud);
}



BS_STATUS avlTreePrint(AVL_TREE * pRoot, PF_AVL_PRINT_FUNC print_func)
{
    if  ((NULL == pRoot) || (NULL == *pRoot))
    {
        return BS_OK;
    }

    print_func(*pRoot);

    if  (!(NULL == (*pRoot)->left))
    {
        avlTreePrint((AVL_TREE *)(&((*pRoot)->left)), print_func);
    }

    if  (!(NULL == (*pRoot)->right))
    {
        avlTreePrint((AVL_TREE *)(&((*pRoot)->right)), print_func);
    }

    return BS_OK;
}



BS_STATUS avlTreeErase(AVL_TREE * pRoot, IN PF_AVL_FREE_FUNC pfFree, IN void *pUserHandle)
{
    if  ((NULL == pRoot) || (NULL == *pRoot)) {
        return BS_OK;
    }

    if  (!(NULL == (*pRoot)->left)) {
        avlTreeErase((AVL_TREE *)(&((*pRoot)->left)), pfFree, pUserHandle);
    }

    if  (!(NULL == (*pRoot)->right)) {
        avlTreeErase((AVL_TREE *)(&((*pRoot)->right)), pfFree, pUserHandle);
    }

    pfFree(*pRoot, pUserHandle);

    *pRoot = NULL;

    return BS_OK;
}



BS_STATUS avlTreePrintErase(AVL_TREE * pRoot, PF_AVL_PRINT_FUNC print_func)
{
    if  ((NULL == pRoot) || (NULL == *pRoot))
    {
        return BS_OK;
    }

    print_func(*pRoot);

    if  (!(NULL == (*pRoot)->left))
    {
        avlTreePrintErase((AVL_TREE *)(&((*pRoot)->left)), print_func);
    }

    if  (!(NULL == (*pRoot)->right))
    {
        avlTreePrintErase((AVL_TREE *)(&((*pRoot)->right)), print_func);
    }

    MEM_Free(*pRoot);
    *pRoot = NULL;

    return BS_OK;
}

