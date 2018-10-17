#ifndef __RB_TREE_H
#define __RB_TREE_H

#define RB_RED    0
#define RB_BLACK  1

typedef struct RB_Node {
    struct RB_Node *left;
    struct RB_Node *right;
    struct RB_Node *parents;
    unsigned int color;
    unsigned int value;
}RBNode;


typedef struct RB_Tree {
    struct RB_Node *root;
}RBTree;

RBNode Tnull = {
    .left = NULL,
    .right = NULL,
    .parents = NULL,
    .color = RB_BLACK,
};

RBNode *T = &Tnull;


extern RBNode *NewNode(int val);
extern RBTree *NewRBTree(int val);
extern void RB_DeleteNode(RBTree *tree, RBNode *delete);
extern void RB_InsertNode(RBTree *tree, RBNode *new);

#endif
