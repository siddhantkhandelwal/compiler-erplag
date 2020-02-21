#include <stdio.h>
#include <stdlib.h>
#include "parserDef.h"
#include "../lexer/lexer.h"

typedef struct tNode tNode;

struct nlNode
{
    Symbol s;
    tNode *child;
    tNode *sibling;
};

typedef struct nlNode nlNode;

struct lNode
{
    tokenInfo *ti;
    tNode *sibling;
};

typedef struct lNode lNode;

union node {
    nlNode *n;
    lNode *l;
};

typedef union node treeNode;

struct tNode
{
    treeNode node;
    int leafTag;
};

tNode *createtNode(rhsNode *rh, tokenInfo *ti);