#include <stdio.h>
#include <stdlib.h>

char *keywordDict[] = {"integer", "real", "boolean", "of", "array", "start",
                       "end", "declare", "module", "driver", "program",
                       "record", "tagged", "union", "get_value", "print",
                       "use", "with", "parameters", "true", "false", "takes",
                       "input", "returns", "AND", "OR", "for", "in", "switch",
                       "case", "break", "default", "while"};

enum TERM{
    AND, ARRAY, ASSIGNOP, BC, BO, BOOLEAN, BREAK, CASE,  COLON, COMMA, COMMENT, DECLARE,
    DEF, DEFAULT, DELIM, DELIMITER, DIV, DRIVER, DRIVERDEF, DRIVERENDDEF, END,  ENDDEF,  EQ,
    FALSE, FOR, GE, GET_VALUE, GT, ID, IN, INPUT, INTEGER,  LE,  LT, MINUS, MODULE, MUL,
    NE, NUM,  OF, OR, PARAMETERS, PLUS, PRINT, PROGRAM, RANGEOP, REAL, RETURNS,
    RNUM, START,  SEMICOL, SQBC, SQBO, SWITCH, TAKES, TRUE,  USE, WHILE,  WITH
};
typedef enum TERM Token;

int state;

typedef union 
{
    int v1;
    float v2;
} Value;

struct TokenInfo
{
    Token t;
    char lexeme[30];
    Value value;
    unsigned int line;
    int tag;
};

typedef struct TokenInfo tokenInfo;

// struct Node
// {
//     tokenInfo *tk;
// };

// typedef struct Node node;

// struct Header
// {
//     node *headLink;
// };

// typedef struct Header *header;