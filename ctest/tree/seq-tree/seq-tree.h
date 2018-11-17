#ifndef __SEQ_TREE_H
#define __SEQ_TREE_H
#include <inttypes.h>

#define RB_RED      0
#define RB_BLACK    1

typedef struct SEQ_Node {
    struct SEQ_Node *left;
    struct SEQ_Node *right;
    struct SEQ_Node *parents;
    uint8_t  color;
    uint32_t value;
    uint32_t size;
}SEQNode;


typedef struct SEQ_Tree {
    struct SEQ_Node *root;
}SEQTree;

SEQNode Tnull = {
    .left = NULL,
    .right = NULL,
    .parents = NULL,
    .color = RB_BLACK,
    .size = 0,
};

SEQNode *T = &Tnull;

#endif
