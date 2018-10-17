#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#define DOWN  0
#define RUP   1
#define LUP   2

typedef struct Node {
    int val;
    struct Node *parents;
    struct Node *left;
    struct Node *right;
}BNode;

BNode * NewNode(int val)
{
    BNode *root = malloc(sizeof(struct Node));
    root->val = val;
    root->left = NULL;
    root->right = NULL;
    root->parents = NULL;

    return root;
}

void InsertNode(BNode **tree, BNode *node)
{
    BNode *p = *tree;
    if (*tree == NULL)
        *tree = node;
    while (1)
    {
        if (node->val < p->val)
        {
            if (p->left == NULL)
            {
                p->left = node;
                node->parents = p;
                break;
            }
            p = p->left;
            continue;
        }

        if (p->right == NULL)
        {
            p->right = node;
            node->parents = p;
            break;
        }
        p = p->right;
    }
}

BNode *MinNode(BNode *node)
{
    while(node->left != NULL)
        node = node->left;
    return node;
}

BNode *MinNodeRec(BNode *node)
{
    BNode *curr = node;
    if (node->left == NULL)
        return node;
    return MinNode(curr->left);
}

void Transplat(BNode **tree, BNode *old, BNode *new)
{
    if(old->parents == NULL)
    {
        *tree = new;
        return;
    }

    if (old->parents->left == old)
        old->parents->left = new;
    else
        old->parents->right = new;
    if (new != NULL)
        new->parents = old->parents;
}

void DeleteNode(BNode **tree, BNode *old)
{
    if (old->left == NULL)
        Transplat(tree, old, old->right);
    else if (old->right == NULL)
        Transplat(tree, old, old->left);
    else {
        BNode *Min = MinNode(old->right);
        if (Min == old->right) {
            Min->left = old->left;
            Transplat(tree, old, Min);
            return;
        }
        Transplat(tree, Min, Min->right);
        Min->left = old->left;
        Min->right = old->right;
        old->left->parents = Min;
        old->right->parents = Min;
        Transplat(tree, old, Min);
    }
}

void walkTree(BNode *tree)
{
    BNode *curr = tree;
    int state = DOWN;
    while(!(curr == tree && ((curr->right == NULL && state & LUP) || (state & RUP))))
    {
        if (curr->left != NULL && state == DOWN) {
            curr = curr->left;
            state = DOWN;
            continue;
        }
        if (state == DOWN)
            printf("%d\n",curr->val);

        if (curr->right != NULL && state != RUP) {
            curr = curr->right;
            state = DOWN;
            continue;
        }

        if (curr == tree)
            break;
        state = (curr == curr->parents->left)?(LUP):(RUP);
        curr = curr->parents;

        if (state & LUP)
            printf("%d\n",curr->val);
    }

}

void walkTreeRec(BNode *tree)
{
    BNode *curr = tree;
    if (curr != NULL) {
        walkTreeRec(tree->left);
        printf("%d\n", curr->val);
        walkTreeRec(tree->right);
    }
}

BNode * SearchNode(BNode *tree, int val)
{
    BNode *curr = tree;
    while(curr != NULL)
    {
        if (val > curr->val)
            curr = curr->right;
        else if (val < curr->val)
            curr = curr->left;
        else
            return curr;
    }
    printf("Not Found\n");
}

int main(int argc, char *argv[])
{
    int i = 0;
    BNode *tree = NewNode(50);
    BNode *min;
    //for (i; i < 50000000; i++)
   // {
    //  InsertNode(&tree, NewNode(random()));
   // }
       BNode *b1 = NewNode(18);
       BNode *b2 = NewNode(20);
       BNode *b3 = NewNode(6);
       BNode *b4 = NewNode(1);
       BNode *b5 = NewNode(4);
       BNode *b6 = NewNode(44);
       BNode *b7 = NewNode(133);
       BNode *b8 = NewNode(12);
       InsertNode(&tree, b1);
       InsertNode(&tree, b2);
       InsertNode(&tree, b3);
       InsertNode(&tree, b4);
       InsertNode(&tree, b5);
       InsertNode(&tree, b6);
       InsertNode(&tree, b7);
       InsertNode(&tree, b8);
       InsertNode(&tree, NewNode(19));
       InsertNode(&tree, NewNode(100));
       walkTree(tree);
       //printf("==========================\n");
       //SearchNode(tree, atoi(argv[1]));
       min = MinNode(b2);
       printf("min:%d\n", min->val);
       min = MinNodeRec(b2);
       printf("min:%d\n", min->val);
}
