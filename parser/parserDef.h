#ifndef parserHeader
#define parserHeader

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lexer/lexerDef.h"

#define NOOFRULES 95
#define MAXLENGTHTNT 100
#define TERMINALS 59
#define NTERMINALS 57

enum NTERM
{

    PROGRAM_NT,
    MODULEDECLARATIONS,
    OTHERMODULES,
    DRIVERMODULE,
    MODULEDECLARATION,
    MODULE_NT,
    MODULEDEF,
    INPUTPLIST,
    RET,
    INPUTPLISTNEW,
    DATATYPE,
    OUTPUTPLIST,
    TYPE,
    OUTPUTPLISTNEW,
    STATEMENTS,
    STATEMENT,
    IOSTMT,
    SIMPLESTMT,
    DECLARESTMT,
    CONDITIONALSTMT,
    ITERATIVESTMT,
    VAR,
    VARIDNUM,
    BOOLCONSTT,
    WHICHID,
    INDEX,
    ASSIGNMENTSTMT,
    MODULEREUSESTMT,
    WHICHSTMT,
    LVALUEIDSTMT,
    LVALUEARRSTMT,
    EXPRESSION,
    OPTIONAL,
    IDLIST,
    IDLISTNEW,
    ARITHMETICORBOOLEANEXPRESSION,
    U,
    UNARYOP,
    NEWNT,
    ARITHMETICEXPR,
    ANYTERM,
    N7,
    LOGICALOP,
    N8,
    RELATIONALOP,
    OP1,
    TERM,
    N4,
    OP2,
    FACTOR,
    N5,
    CASESTMTS,
    DEFAULT_NT,
    VALUE,
    N9,
    RANGE,
    RANGEARRAYS

};

typedef enum TERM Terminal;
typedef enum NTERM nonTerminal;

/*typedef enum
{
    T,
    N
} typeOfSym;*/

typedef union {
    Terminal T;
    nonTerminal N;
} Symbol;

struct rhsnode
{
    Symbol S;
    int tag;
    struct rhsnode *next;
};

typedef struct rhsnode rhsNode;

typedef struct rule_header rule_header;

typedef rule_header *Grammar;

Grammar g[NOOFRULES];

void populateGrammar(FILE *fp);

typedef struct
{

    Symbol s;
    char str[30];
    int tag;

} map_node;

typedef struct hash_node hash_node;

struct hash_node
{

    map_node *m;
    hash_node *next;
};

rule_header *parseTable[NTERMINALS][TERMINALS];

struct rule_header
{
    rule_header *next_rule;
    rhsNode *curr_rule;
};

long long unsigned First[NTERMINALS];

long long unsigned Follow[NTERMINALS];

#endif