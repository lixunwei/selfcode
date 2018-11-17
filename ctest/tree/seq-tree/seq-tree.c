#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <inttypes.h>
#include "seq-tree.h"

SEQNode *SEQ_NewNode(uint32_t val)
{
    SEQNode *root = malloc(sizeof(struct SEQ_Node));
    root->parents = T;
    root->left = T;
    root->right = T;
    root->value = val;
    root->color = RB_RED;
    root->size = 1;
}

SEQTree *SEQ_NewTree(uint32_t val)
{
    SEQNode *root = SEQ_NewNode(val);
    SEQTree *tree = (SEQTree *)malloc(sizeof(struct SEQ_Tree));

    root->color = RB_BLACK;
    root->size = 1;
    tree->root = root;
    return tree;
}

static void Left_Rotate(SEQTree *tree, SEQNode *pivot)
{
    SEQNode *right = pivot->right;

    right->size = pivot->size;
    pivot->right = right->left;
    if (right->left != T)
        right->left->parents = pivot;
    right->left = pivot;
    right->parents = pivot->parents;
    pivot->parents = right;
    if (tree->root == pivot)
        tree->root = right;
    else if (pivot == right->parents->left)
        right->parents->left = right;
    else if (pivot == right->parents->right)
        right->parents->right = right;
    pivot->size = pivot->left->size + pivot->right->size + 1;
}

static void Right_Rotate(SEQTree *tree, SEQNode *pivot)
{
    SEQNode *left = pivot->left;

    left->size = pivot->size;
    pivot->left = left->right;
    if (left->right != T)
        left->right->parents = pivot;
    left->right = pivot;
    left->parents = pivot->parents;
    pivot->parents = left;
    if (tree->root == pivot)
        tree->root = left;
    else if (pivot == left->parents->left)
        left->parents->left = left;
    else if (pivot == left->parents->right)
        left->parents->right = left;
    pivot->size = pivot->left->size + pivot->right->size + 1;
}

static void SEQ_InsertFix(SEQTree *tree, SEQNode *new)
{
    SEQNode *current = new;
    SEQNode *uncle;
    while(current->parents->color == RB_RED) {
        if (current->parents == current->parents->parents->left) {
            uncle = current->parents->parents->right;
            if (uncle->color == RB_RED) {
                uncle->color = RB_BLACK;
                current->parents->color = RB_BLACK;
                current = current->parents->parents;
                current->color = RB_RED;
                continue;
            }

            if (current == current->parents->right) {
                current = current->parents;
                Left_Rotate(tree, current);
                continue;
            }

            if (current == current->parents->left) {
                current->parents->parents->color = RB_RED;
                current->parents->color = RB_BLACK;
                Right_Rotate(tree, current->parents->parents);
                break;
            }
        }

        if (current->parents == current->parents->parents->right) {
            uncle = current->parents->parents->left;
            if (uncle->color == RB_RED) {
                uncle->color = RB_BLACK;
                current->parents->color = RB_BLACK;
                current = current->parents->parents;
                current->color = RB_RED;
                continue;
            }

            if (current == current->parents->left) {
                current = current->parents;
                Right_Rotate(tree, current);
                continue;
            }

            if (current == current->parents->right) {
                current->parents->parents->color = RB_RED;
                current->parents->color = RB_BLACK;
                Left_Rotate(tree, current->parents->parents);
                break;
            }
        }
    }
    tree->root->color = RB_BLACK;
}

void SEQ_InsertNode(SEQTree *tree, SEQNode *node)
{
    SEQNode *search = tree->root;
    SEQNode *current = T;

    while(search != T) {
        current = search;
        current->size++; //seq insert a new node
        if (node->value < search->value)
            search = search->left;
        else
            search = search->right;
    }
    if (current == T) {
        tree->root = node;
    }
    if (node->value < current->value)
        current->left = node;
    else
        current->right = node;
    node->parents = current;
    node->left  = T;
    node->right = T;
    node->size  = 1;
    node->color  = RB_RED;
    SEQ_InsertFix(tree, node);
}

static void SEQ_Transplat(SEQTree *tree, SEQNode *old, SEQNode *new)
{
    if (tree->root == old)
        tree->root = new;
    else if (old == old->parents->left)
        old->parents->left = new;
    else
        old->parents->right = new;

    new->parents = old->parents;
}

static SEQNode *MinNode(SEQTree *tree, SEQNode *node)
{
    while (node->left != T)
        node = node->left;
    return node;
}

static void SEQ_DeleteFix(SEQTree *tree, SEQNode *node)
{
    SEQNode *brother;
    SEQNode *current = node;

    while(current != tree->root && current->color == RB_BLACK) {
        if (current == current->parents->left) {
            brother = current->parents->right;
            if (brother->color == RB_RED) {
                brother->color = RB_BLACK;
                current->parents->color = RB_RED;
                Left_Rotate(tree, current->parents);
                continue;
            }
            if (brother->left->color == RB_BLACK && brother->right->color == RB_BLACK) {
                brother->color = RB_RED;
                current = current->parents;
                continue;
            }
            if (brother->right->color == RB_BLACK) {
                brother->left->color = RB_BLACK;
                brother->color = RB_RED;
                Right_Rotate(tree, brother);
            } else {
                brother->color = current->parents->color;
                current->parents->color = RB_BLACK;
                brother->right->color = RB_BLACK;
                Left_Rotate(tree, current->parents);
                current = tree->root;
            }
        } else {
            brother = current->parents->left;
            if (brother->color == RB_RED) {
                brother->color = RB_BLACK;
                current->parents->color = RB_RED;
                Right_Rotate(tree, current->parents);
                continue;
            }

            if (brother->left->color == RB_BLACK && brother->right->color == RB_BLACK) {
                brother->color = RB_RED;
                current = current->parents;
                continue;
            }
            if (brother->left->color == RB_BLACK) {
                brother->color = RB_RED;
                brother->right->color = RB_BLACK;
                Left_Rotate(tree, brother);
            } else {
                brother->color = current->parents->color;
                current->parents->color = RB_BLACK;
                brother->left->color = RB_BLACK;
                Right_Rotate(tree, current->parents);
                current = tree->root;
            }
        }
    }
    current->color = RB_BLACK;
}

void SEQ_DeleteNode(SEQTree *tree, SEQNode *node)
{
    SEQNode *current;
    SEQNode *search;
    uint32_t color = node->color;

    if (node->left == T) {
        current = node->right;
        SEQ_Transplat(tree, node, current);
        search = current->parents;
        while(search != T) {
            search->size--;
            search = search->parents;
        }
    } else if (node->right == T) {
        current = node->left;
        SEQ_Transplat(tree, node, current);
        search = current->parents;
        while(search != T) {
            search->size--;
            search = search->parents;
        }
    } else {
        SEQNode *min = MinNode(tree, node->right);
        color = min->color;
        current = min->right;
        min->color = node->color;
        if(min->parents == node) {
            current->parents = min;
            min->left = node->left;
            node->left->parents = min;
            SEQ_Transplat(tree, node ,min);
            min->size = min->left->size + min->right->size + 1;
            search = min->parents;
            while(search != T) {
                search->size--;
                search = search->parents;
            }
        }else {
            SEQ_Transplat(tree, min, min->right);
            min->left = node->left;
            node->left->parents = min;
            min->right = node->right;
            node->right->parents = min;
            min->size = min->left->size + min->right->size + 1;
            SEQ_Transplat(tree, node, min);
            search = current->parents;
            while(search != T) {
                search->size--;
                search = search->parents;
            }
        }
    }
    if (color == RB_BLACK)
        SEQ_DeleteFix(tree, current);
    T->parents = NULL;
}

uint32_t SEQ_SelfTest(SEQNode *root)
{
    uint32_t ret = 0;
    uint32_t lret;
    uint32_t rret;
    if (root != T) {
        lret = SEQ_SelfTest(root->left);
        ret = lret;
        if (root->color == RB_BLACK)
            ret += 1;
        else if (root->left->color != RB_BLACK || root->right->color != RB_BLACK) {
            printf("color error val=%u\n", root->value);
            exit(1);
        }

        if (root->size != (root->left->size + root->right->size + 1)) {
            printf("size is error %u\n", root->value);
            exit(2);
        }
        rret = SEQ_SelfTest(root->right);
        if (lret != rret) {
            printf("error count of black node is not match %u\n", root->value);
            exit(1);
        }
    }
    return ret;
}

void SEQ_walkTreeRec(SEQNode *root)
{
    if ( root != T ) {
        SEQ_walkTreeRec(root->left);
        printf("%u\n", root->value);
        SEQ_walkTreeRec(root->right);
    }
}

int main(int argc, char *argv[])
{
    SEQTree *tree = SEQ_NewTree(500);
    int i = 0;
    SEQNode *arr[10000];
    uint32_t val;
    for (; i < 1000; i++) {
        val = random();
        arr[i] = SEQ_NewNode(val%1000);
        SEQ_InsertNode(tree, arr[i]);
    }
    i = 0;
    for (; i < 900; i++) {
        SEQ_DeleteNode(tree, arr[i]);
    }

    SEQ_SelfTest(tree->root);
    //SEQ_walkTreeRec(tree->root);
}
