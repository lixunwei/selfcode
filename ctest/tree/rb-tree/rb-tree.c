#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include "rb-tree.h"

RBNode *NewNode(int val)
{
    RBNode *root = malloc(sizeof(struct RB_Node));
    root->parents = T;
    root->left = T;
    root->right = T;
    root->value = val;
    root->color = RB_RED;
}

RBTree *NewRBTree(int val)
{
    RBNode *root = NewNode(val);
    RBTree *tree = malloc(sizeof(struct RB_Tree));

    root->color = RB_BLACK;
    tree->root = root;
    return tree;
}

void Left_Rotate(RBTree *tree, RBNode *pivot)
{
    RBNode *right = pivot->right;

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
}

void Right_Rotate(RBTree *tree, RBNode *pivot)
{
    RBNode *left = pivot->left;

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
}

void RB_InsertFix(RBTree *tree, RBNode *new)
{
    RBNode *current = new;
    RBNode *uncle;
    while (current->parents->color == RB_RED)
    {
        if (current->parents == current->parents->parents->left) {//insert node is in left child tree
            uncle = current->parents->parents->right;
            if (uncle->color == RB_RED) //case 1: uncle is red
            {
                uncle->color = RB_BLACK; //fix: uncle set to black
                current->parents->color = RB_BLACK; //fix: parents set to black
                current = current->parents->parents; //fix: new node rise up to grandfather node
                current->color = RB_RED;//fix: grandfater set to RED
                continue;
            }
            if (current == current->parents->right) //case 2: uncle is black and current in right child tree
            {
                current = current->parents;//case 2 to case 3
                Left_Rotate(tree, current);//fix: left rotate
                continue;
            }
            current->parents->parents->color = RB_RED; //case 3: uncle is black and current in left child tree
            current->parents->color = RB_BLACK;
            Right_Rotate(tree, current->parents->parents);
        }
        if (current->parents == current->parents->parents->right) {
            uncle = current->parents->parents->left;
            if (uncle->color == RB_RED)
            {
                uncle->color = RB_BLACK;
                current->parents->color = RB_BLACK;
                current = current->parents->parents;
                current->color = RB_RED;
                continue;
            }
            if (current = current->parents->left)
            {
                current = current->parents;
                Right_Rotate(tree, current);
                continue;
            }
            current->parents->parents->color = RB_RED;
            current->parents->color = RB_BLACK;
            Left_Rotate(tree, current->parents->parents);
        }
    }
    tree->root->color = RB_BLACK;
}

void RB_InsertNode(RBTree *tree, RBNode *new)
{
    RBNode *search = tree->root;
    RBNode *current = T;
    while(search != T)
    {
        current = search;
        if (new->value < search->value)
            search = search->left;
        else
            search = search->right;
    }
    if (current == T)
        tree->root = new;
    if (new->value < current->value)
        current->left = new;
    else
        current->right = new;
    new->parents = current;
    new->left = T;
    new->right = T;
    new->color = RB_RED;
    RB_InsertFix(tree, new);
}

RBNode *MinNode(RBTree *tree, RBNode *node)
{
    while(node->left != T)
        node = node->left;
    return node;
}

void RB_Transplat(RBTree *tree, RBNode *old, RBNode *new)
{
    if (tree->root == old)
        tree->root = new;
    else if (old == old->parents->left)
        old->parents->left = new;
    else
        old->parents->right = new;
    // if (new != T) T->parents = old->parents;
        new->parents = old->parents;
}

void RB_FixDelete(RBTree *tree, RBNode *current)
{
    while (current != tree->root && current->color == RB_BLACK) {
        if (current == current->parents->left) {
            if (current->parents->right->color == RB_RED) { //case 1 : brother node is red
                Left_Rotate(tree, current->parents);
                current->parents->right->color = RB_BLACK;
                current->parents->color = RB_RED;
                continue;
            }
            if (current->parents->right->right->color == RB_RED) {
                current->parents->right->right->color = RB_BLACK;
                current->parents->right->color = current->parents->color;
                current->parents->color = RB_BLACK;
                Left_Rotate(tree, current->parents);
                current = tree->root;
                break;
            }
            if (current->parents->right->left->color == RB_RED) {
                current->parents->right->left->color = RB_BLACK;
                current->parents->right->color = RB_RED;
                Right_Rotate(tree, current->parents->right);
                continue;
            }
            current->parents->right->color = RB_RED;
            current = current->parents;
        }

        if (current == current->parents->right) {
            if (current->parents->left->color == RB_RED) {
                Right_Rotate(tree, current->parents);
                current->parents->left->color = RB_BLACK;
                current->parents->color = RB_RED;
                continue;
            }
            if (current->parents->left->left->color == RB_RED) {
                current->parents->left->left->color = RB_BLACK;
                current->parents->left->color = current->parents->color;
                current->parents->color = RB_BLACK;
                Right_Rotate(tree, current->parents);
                current = tree->root;
                break;
            }
            if (current->parents->left->right->color == RB_RED) {
                current->parents->left->right->color = RB_BLACK;
                current->parents->left->color = RB_RED;
                Left_Rotate(tree, current->parents->left);
                continue;
            }
            current->parents->left->color = RB_RED;
            current = current->parents;
        }
    }
    current->color = RB_BLACK;
}

void RB_DeleteNode(RBTree *tree, RBNode *delete)
{
    RBNode *current;
    unsigned int color = delete->color;

    if (delete->left == T) {
        current = delete->right;
        RB_Transplat(tree, delete, current);
    } else if (delete->right == T) {
        current = delete->left;
        RB_Transplat(tree, delete, current);
    } else {
        RBNode *min = MinNode(tree, delete->right);
        color = min->color;
        current = min->right;
        if (min->parents == delete) {
            min->left = delete->left;
            delete->left->parents = min;
            RB_Transplat(tree, delete, min);
        }else {
            RB_Transplat(tree, min, min->right);
            min->left = delete->left;
            delete->left->parents = min;
            min->right = delete->right;
            delete->right->parents = min;
            RB_Transplat(tree, delete, min);
        }
    }

    if (color == RB_BLACK)
        RB_FixDelete(tree, current);
}

int main(int argc, char *argv[])
{
    int i = 0;
    RBTree *tree = NewRBTree(50);

    RBNode *b1 = NewNode(18);
    RBNode *b2 = NewNode(20);
    RBNode *b3 = NewNode(6);
    RBNode *b4 = NewNode(1);
    RBNode *b5 = NewNode(4);
    RBNode *b6 = NewNode(44);
    RBNode *b7 = NewNode(133);
    RBNode *b8 = NewNode(12);
    RBNode *b9 = NewNode(30);
    RBNode *b10 = NewNode(41);

    RB_InsertNode(tree, b1);
    RB_InsertNode(tree, b2);
    RB_InsertNode(tree, b3);
    RB_InsertNode(tree, b4);
    RB_InsertNode(tree, b5);
    RB_InsertNode(tree, b6);
    RB_InsertNode(tree, b7);
    RB_InsertNode(tree, b8);
    RB_InsertNode(tree, b9);
    RB_InsertNode(tree, b10);
}
