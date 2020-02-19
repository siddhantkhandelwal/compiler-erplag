#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOOFRULES 95
#define MAXLENGTHTNT 50

char *terminalDict[] = {"INTEGER", "REAL", "BOOLEAN", "OF", "ARRAY", "START",
                       "END", "DECLARE", "MODULE", "DRIVER", "PROGRAM",
                       "GET_VALUE", "PRINT",
                       "USE", "WITH", "PARAMETERS", "TRUE", "FALSE", "TAKES",
                       "INPUT", "RETURNS", "AND", "OR", "FOR", "IN", "SWITCH",
                       "CASE", "BREAK", "DEFAULT", "WHILE","ASSIGNOP",
					    "BC",
					    "BO",
					    "COLON",
					    "COMMA",
					    "COMMENT",
					    "DIV",
					    "DRIVERDEF",
					    "DRIVERENDDEF",
					    "ENDDEF",
					    "DEF",
					    "EQ",
					    "GE",
					    "GT",
					    "ID",
					    "LE",
					    "LT",
					    "MINUS",
					    "MUL",
					    "NE",
					    "NUM",
					    "PLUS",
					    "RANGEOP",
					    "SEMICOL",
					    "SQBC",
					    "SQBO",
					    "RNUM"};

enum TERM
{
    INTEGER,
    REAL,
    BOOLEAN,
    OF,
    ARRAY,
    START,
    END,
    DECLARE,
    MODULE,
    DRIVER,
    PROGRAM,
    GET_VALUE,
    PRINT,
    USE,
    WITH,
    PARAMETERS,
    TRUE,
    FALSE,
    TAKES,
    INPUT,
    RETURNS,
    AND,
    OR,
    FOR,
    IN,
    SWITCH,
    CASE,
    BREAK,
    DEFAULT,
    WHILE,
    ASSIGNOP,
    BC,
    BO,
    COLON,
    COMMA,
    COMMENT,
    DIV,
    DRIVERDEF,
    DRIVERENDDEF,
    ENDDEF,
    DEF,
    EQ,
    GE,
    GT,
    ID,
    LE,
    LT,
    MINUS,
    MUL,
    NE,
    NUM,
    PLUS,
    RANGEOP,
    SEMICOL,
    SQBC,
    SQBO,
    RNUM
};

typedef enum TERM Terminal;
typedef enum TERM nonTerminal;


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

typedef struct
{
    nonTerminal cell;
    rhsNode *head;
} Grammar[NOOFRULES];

Grammar *populateGrammar(FILE *fp);

typedef struct
{

	Symbol s;
	char str[30];
	int tag;
	
}map_node;

typedef struct hash_node hash_node;

struct hash_node{

	map_node* m;
	hash_node* next;
};