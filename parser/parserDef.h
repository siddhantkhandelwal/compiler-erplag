#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOOFRULES 95
#define MAXLENGTHTNT 50

typedef enum
{
    T,
    N
} typeOfSym;

typedef union {
    terminal T;
    nonTerminal N;
} Symbol;

struct rhsnode
{
    Symbol S;
    typeOfSym flag;
    struct rhsnode *next;
};

typedef struct rhsnode rhsNode;

typedef struct
{
    nonTerminal cell;
    rhsNode *head;
} Grammar[NOOFRULES];

Grammar *populateGrammar(FILE *fp);