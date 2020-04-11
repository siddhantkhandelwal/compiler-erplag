/* LAVANYA SONI - 2017A7PS0151P
    KUSHAGRA AGRAWAL - 2017A7PS0107P
    SIDDHANT KHANDELWAL - 2017A7PS0127P
    GROUP - 11
*/

#include <stdio.h>
#include <stdlib.h>
#include "parserDef.h"
#include "lexer.h"

typedef struct tNode tNode;
typedef struct symbol_entry se;
typedef struct scope scope;
typedef struct type_info type_info;

struct nlNode
{
    Symbol s;
    tNode *child;
    tNode *sibling;
    int is_operator;
    int line;
};

typedef struct nlNode nlNode;

struct lNode
{
    Symbol s;
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
    tNode *parent;
    scope *sc;
    se *entry;
    type_info *type;
};

tNode *createtNode(rhsNode *rh, tokenInfo *ti);
void inserttNode(tNode *ptn, tNode *tn);
extern tNode *head;