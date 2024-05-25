/*================================================================
*   Created by LiXingang
*   Description: 红黑树
*
================================================================*/
#include "bs.h"
#include "utl/rb_tree.h"

#define RB_PARENT(r)   ((r)->parent)
#define rb_color(r) ((r)->color)
#define rb_is_red(r)   ((r)->color==RB_TREE_RED)
#define rb_is_black(r)  ((r)->color==RB_TREE_BLACK)
#define rb_set_black(r)  do { (r)->color = RB_TREE_BLACK; } while (0)
#define rb_set_red(r)  do { (r)->color = RB_TREE_RED; } while (0)
#define rb_set_parent(r,p)  do { (r)->parent = (p); } while (0)
#define rb_set_color(r,c)  do { (r)->color = (c); } while (0)


static int _rbtree_preorder_walk(RB_TREE_NODE_S * tree, PF_RBTREE_WALK walk_func, void *ud)
{
    int ret;

    if (! tree) {
        return 0;
    }

    if ((ret= walk_func(tree, ud)) < 0) {
        return ret;
    }

    if ((ret = _rbtree_preorder_walk(tree->left, walk_func, ud)) < 0) {
        return ret;
    }

    if ((ret = _rbtree_preorder_walk(tree->right, walk_func, ud)) < 0) {
        return BS_STOP;
    }

    return 0;
}

static int _rbtree_inorder_walk(RB_TREE_NODE_S * tree, PF_RBTREE_WALK walk_func, void *ud)
{
    int ret;

    if (! tree) {
        return 0;
    }

    if ((ret = _rbtree_inorder_walk(tree->left, walk_func, ud)) < 0) {
        return ret;
    }

    if ((ret = walk_func(tree, ud)) < 0) {
        return ret;
    }

    if ((ret = _rbtree_inorder_walk(tree->right, walk_func, ud)) < 0) {
        return ret;
    }

    return 0;
}

static int _rbtree_postorder_walk(RB_TREE_NODE_S * tree, PF_RBTREE_WALK walk_func, void *ud)
{
    int ret;

    if (! tree) {
        return 0;
    }

    if ((ret = _rbtree_postorder_walk(tree->left, walk_func, ud)) < 0) {
        return ret;
    }

    if ((ret = _rbtree_postorder_walk(tree->right, walk_func, ud)) < 0) {
        return ret;
    }

    if ((ret = walk_func(tree, ud)) < 0) {
        return ret;
    }

    return 0;
}

static inline RB_TREE_NODE_S * _rbtree_search(RB_TREE_NODE_S * x, void *key, PF_RBTREE_CMP cmp_func)
{
    int cmp_ret;

    while (x) {
        cmp_ret = cmp_func(key, x);
        if (cmp_ret == 0) {
            break;
        } else if (cmp_ret < 0) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    return x;
}


static inline RB_TREE_NODE_S * _rbtree_minimum(RB_TREE_NODE_S * tree)
{
    while(tree->left != NULL) {
        tree = tree->left;
    }

    return tree;
}


static inline RB_TREE_NODE_S * _rbtree_maximum(RB_TREE_NODE_S * tree)
{
    while(tree->right != NULL)
        tree = tree->right;
    return tree;
}


static void _rbtree_left_rotate(RB_TREE_CTRL_S *root, RB_TREE_NODE_S *x)
{
    
    RB_TREE_NODE_S *y = x->right;

    
    
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;

    
    y->parent = x->parent;

    if (x->parent == NULL)
    {
        
        root->node = y;            
    }
    else
    {
        if (x->parent->left == x)
            x->parent->left = y;    
        else
            x->parent->right = y;    
    }

    
    y->left = x;
    
    x->parent = y;
}


static void _rbtree_right_rotate(RB_TREE_CTRL_S *root, RB_TREE_NODE_S *y)
{
    
    RB_TREE_NODE_S *x = y->left;

    
    
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;

    
    x->parent = y->parent;

    if (y->parent == NULL)
    {
        
        root->node = x;            
    }
    else
    {
        if (y == y->parent->right)
            y->parent->right = x;    
        else
            y->parent->left = x;    
    }

    
    x->right = y;

    
    y->parent = x;
}


static void _rbtree_insert_fixup(RB_TREE_CTRL_S *root, RB_TREE_NODE_S *node)
{
    RB_TREE_NODE_S *parent, *gparent;

    
    while ((parent = node->parent) && rb_is_red(parent)) {
        gparent = parent->parent;

        
        if (parent == gparent->left) {

            
            RB_TREE_NODE_S *uncle = gparent->right;
            if (uncle && rb_is_red(uncle)) {
                rb_set_black(uncle);
                rb_set_black(parent);
                rb_set_red(gparent);
                node = gparent;
                continue;
            }

            
            if (parent->right == node) {
                RB_TREE_NODE_S *tmp;
                _rbtree_left_rotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            
            rb_set_black(parent);
            rb_set_red(gparent);
            _rbtree_right_rotate(root, gparent);
        } else { 

            
            RB_TREE_NODE_S *uncle = gparent->left;
            if (uncle && rb_is_red(uncle))
            {
                rb_set_black(uncle);
                rb_set_black(parent);
                rb_set_red(gparent);
                node = gparent;
                continue;
            }

            
            if (parent->left == node)
            {
                RB_TREE_NODE_S *tmp;
                _rbtree_right_rotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            
            rb_set_black(parent);
            rb_set_red(gparent);
            _rbtree_left_rotate(root, gparent);
        }
    }

    
    rb_set_black(root->node);
}


static int _rbtree_insert(RB_TREE_CTRL_S *root, RB_TREE_NODE_S *node, void *key, PF_RBTREE_CMP cmp_func)
{
    RB_TREE_NODE_S *y = NULL;
    RB_TREE_NODE_S *x = root->node;
    int cmp_ret = 0, cmp_ret_y = 0;

    
    while (x) {
        y = x;
        cmp_ret_y = cmp_ret;
        cmp_ret = cmp_func(key, x);
        if (cmp_ret < 0) {
            x = x->left;
        } else if (cmp_ret > 0) {
            x = x->right;
        } else {
            RETURN(BS_CONFLICT);
        }
    }

    node->parent = y;
    node->left = NULL;
    node->right = NULL;
    node->color = RB_TREE_RED;

    if (y != NULL) {
        if (cmp_ret_y < 0) {
            y->left = node; 
        } else {
            y->right = node; 
        }
    } else {
        root->node = node; 
    }

    
    _rbtree_insert_fixup(root, node);

    return 0;
}


static void _rbtree_delete_fixup(RB_TREE_CTRL_S *root, RB_TREE_NODE_S *node, RB_TREE_NODE_S *parent)
{
    RB_TREE_NODE_S *other;

    while ((!node || rb_is_black(node)) && node != root->node)
    {
        if (parent->left == node)
        {
            other = parent->right;
            if (rb_is_red(other))
            {
                
                rb_set_black(other);
                rb_set_red(parent);
                _rbtree_left_rotate(root, parent);
                other = parent->right;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right)))
            {
                
                rb_set_red(other);
                node = parent;
                parent = RB_PARENT(node);
            }
            else
            {
                if (!other->right || rb_is_black(other->right))
                {
                    
                    rb_set_black(other->left);
                    rb_set_red(other);
                    _rbtree_right_rotate(root, other);
                    other = parent->right;
                }
                
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->right);
                _rbtree_left_rotate(root, parent);
                node = root->node;
                break;
            }
        }
        else
        {
            other = parent->left;
            if (rb_is_red(other))
            {
                
                rb_set_black(other);
                rb_set_red(parent);
                _rbtree_right_rotate(root, parent);
                other = parent->left;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right)))
            {
                
                rb_set_red(other);
                node = parent;
                parent = RB_PARENT(node);
            }
            else
            {
                if (!other->left || rb_is_black(other->left))
                {
                    
                    rb_set_black(other->right);
                    rb_set_red(other);
                    _rbtree_left_rotate(root, other);
                    other = parent->left;
                }
                
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->left);
                _rbtree_right_rotate(root, parent);
                node = root->node;
                break;
            }
        }
    }
    if (node)
        rb_set_black(node);
}


static void _rbtree_destroy(RB_TREE_NODE_S * tree, PF_RBTREE_FREE free_func, void *ud)
{
    if (tree->left) {
        _rbtree_destroy(tree->left, free_func, ud);
    }

    if (tree->right) {
        _rbtree_destroy(tree->right, free_func, ud);
    }

    free_func(tree, ud);
}


void RBTree_PreOrderWalk(RB_TREE_CTRL_S *root, PF_RBTREE_WALK walk_func, void *ud)
{
    BS_DBGASSERT(NULL != root);
    _rbtree_preorder_walk(root->node, walk_func, ud);
}


void RBTree_InOrderWalk(RB_TREE_CTRL_S *root, PF_RBTREE_WALK walk_func, void *ud)
{
    BS_DBGASSERT(NULL != root);
    _rbtree_inorder_walk(root->node, walk_func, ud);
}


void RBTree_PostOrderWalk(RB_TREE_CTRL_S *root, PF_RBTREE_WALK walk_func, void *ud)
{
    BS_DBGASSERT(NULL != root);
    _rbtree_postorder_walk(root->node, walk_func, ud);
}

RB_TREE_NODE_S * RBTree_Search(RB_TREE_CTRL_S *root, void *key, PF_RBTREE_CMP cmp_func)
{
    BS_DBGASSERT(NULL != root);

    return _rbtree_search(root->node, key, cmp_func);
}


RB_TREE_NODE_S * RBTree_SuccessorGet(RB_TREE_CTRL_S *root, void *key, PF_RBTREE_CMP cmp_func)
{
    RB_TREE_NODE_S *node = root->node;
    RB_TREE_NODE_S *superiorp = NULL;

    while (1) {
        int delta;  

        if (node == NULL)
            return superiorp;

        delta = cmp_func(key, node);
        if (delta < 0) {
            superiorp = node; 
            node = node->left;
        } else {
            node = node->right;
        }
    }
}


RB_TREE_NODE_S * RBTree_PredecessorGet(RB_TREE_CTRL_S *root, void *key, PF_RBTREE_CMP cmp_func)
{
    RB_TREE_NODE_S *node = root->node;
    RB_TREE_NODE_S *inferiorp = NULL;

    while (1) {
        int delta;  

        if (node == NULL)
            return inferiorp;

        delta = cmp_func(key, node);
        if (delta > 0) {
            inferiorp = node;
            node = node->right;
        } else {
            node = node->left;
        }
    }
}

RB_TREE_NODE_S * RBTree_Min(RB_TREE_CTRL_S *root)
{
    BS_DBGASSERT(root != NULL);

    if (! root->node) {
        return NULL;
    }

    return _rbtree_minimum(root->node);
}

RB_TREE_NODE_S * RBTree_Max(RB_TREE_CTRL_S *root)
{
    BS_DBGASSERT(NULL != root);

    if (! root->node) {
        return NULL;
    }

    return _rbtree_maximum(root->node);
}


RB_TREE_NODE_S* RBTree_Next(RB_TREE_NODE_S * x)
{
    
    if (x->right != NULL) {
        return _rbtree_minimum(x->right);
    }

    
    
    
    RB_TREE_NODE_S* y = x->parent;
    while ((y!=NULL) && (x==y->right)) {
        x = y;
        y = y->parent;
    }

    return y;
}


RB_TREE_NODE_S* RBTree_Pre(RB_TREE_NODE_S * x)
{
    
    if (x->left != NULL) {
        return _rbtree_maximum(x->left);
    }

    
    
    
    RB_TREE_NODE_S* y = x->parent;
    while ((y!=NULL) && (x==y->left)) {
        x = y;
        y = y->parent;
    }

    return y;
}

int RBTree_Insert(RB_TREE_CTRL_S *root, RB_TREE_NODE_S *node, void *key, PF_RBTREE_CMP cmp_func)
{
    return _rbtree_insert(root, node, key, cmp_func);
}

void RBTree_DelNode(RB_TREE_CTRL_S *root, RB_TREE_NODE_S *node)
{
    RB_TREE_NODE_S *child, *parent;
    int color;

    
    if ( (node->left!=NULL) && (node->right!=NULL) ) {
        
        
        RB_TREE_NODE_S *replace = node;

        
        replace = replace->right;
        while (replace->left != NULL) {
            replace = replace->left;
        }

        
        if (node->parent) {
            if (node->parent->left == node)
                node->parent->left = replace;
            else
                node->parent->right = replace;
        } else {
            
            root->node = replace;
        }

        
        
        child = replace->right;
        parent = replace->parent;
        
        color = rb_color(replace);

        
        if (parent == node) {
            parent = replace;
        } else {
            
            if (child) {
                rb_set_parent(child, parent);
            }
            parent->left = child;

            replace->right = node->right;
            rb_set_parent(node->right, replace);
        }

        replace->parent = node->parent;
        replace->color = node->color;
        replace->left = node->left;
        node->left->parent = replace;

        if (color == RB_TREE_BLACK) {
            _rbtree_delete_fixup(root, child, parent);
        }

        return;
    }

    if (node->left !=NULL) {
        child = node->left;
    } else {
        child = node->right;
    }

    parent = node->parent;
    
    color = node->color;

    if (child) {
        child->parent = parent;
    }

    
    if (parent) {
        if (parent->left == node) {
            parent->left = child;
        } else {
            parent->right = child;
        }
    }
    else
        root->node = child;

    if (color == RB_TREE_BLACK) {
        _rbtree_delete_fixup(root, child, parent);
    }
}

RB_TREE_NODE_S * RBTree_Del(RB_TREE_CTRL_S *root, void *key, PF_RBTREE_CMP cmp_func)
{
    RB_TREE_NODE_S *z;

    z = _rbtree_search(root->node, key, cmp_func);

    if (z) {
        RBTree_DelNode(root, z);
    }

    return z;
}


void RBTree_Init(IN RB_TREE_CTRL_S *root)
{
    memset(root, 0, sizeof(RB_TREE_CTRL_S));
}

void RBTree_Finit(RB_TREE_CTRL_S *root, PF_RBTREE_FREE free_func, void *ud)
{
    if (! root) 
        return;

    if (root->node) {
        _rbtree_destroy(root->node, free_func, ud);
    }

    memset(root, 0, sizeof(RB_TREE_CTRL_S));
}


