/* LAVANYA SONI - 2017A7PS0151P
    KUSHAGRA AGRAWAL - 2017A7PS0107P
    SIDDHANT KHANDELWAL - 2017A7PS0127P
    GROUP - 11
*/

#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

struct stacknode
{
    tNode *tn;
    struct stacknode *next;
};

typedef struct stacknode stackNode;

struct stack
{
    stackNode *top;
    int size;
};

typedef struct stack stack;

int isEmpty();
stackNode *createStackNode(tNode *tn);
int initializeStack(tNode *tn);
void push(tNode *tn);
stackNode *pop();
void print_stack();