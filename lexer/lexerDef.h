#ifndef lexerHeader
#define lexerHeader

#include <stdio.h>
#include <stdlib.h>

// char *keywordDict[] = {"integer", "real", "boolean", "of", "array", "start",
//                        "end", "declare", "module", "driver", "program",
//                        "get_value", "print",
//                        "use", "with", "parameters", "true", "false", "takes",
//                        "input", "returns", "AND", "OR", "for", "in", "switch",
//                        "case", "break", "default", "while"};

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
    RNUM,
    DOLLAR,
    EPSILON
};

typedef enum TERM Token;

unsigned int state;

typedef union {
    int v1;
    double v2;
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
#endif