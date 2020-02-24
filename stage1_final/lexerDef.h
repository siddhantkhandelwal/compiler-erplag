/* LAVANYA SONI - 2017A7PS0151P
    KUSHAGRA AGRAWAL - 2017A7PS0107P
    SIDDHANT KHANDELWAL - 2017A7PS0127P
    GROUP - 11
*/


#ifndef lexerHeader
#define lexerHeader

#include <stdio.h>
#include <stdlib.h>


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