#include <stdio.h>
#include <stdlib.h>

char *keyword_dict[] = {"integer", "real", "boolean", "of", "array", "start",
                        "end", "declare", "module", "driver", "program",
                        "record", "tagged", "union", "get_value", "print",
                        "use", "with", "parameters", "true", "false", "takes",
                        "input", "returns", "AND", "OR", "for", "in", "switch",
                        "case", "break", "default", "while"};

char *token_key[] = {"INTEGER", "REAL", "BOOLEAN", "OF", "ARRAY", "START",
                     "END", "DECLARE", "MODULE", "DRIVER", "PROGRAM",
                     "RECORD", "TAGGED", "UNION", "GET_VALUE", "PRINT",
                     "USE", "WITH", "PARAMETERS", "TRUE", "FALSE", "TAKES",
                     "INPUT", "RETURNS", "AND", "OR", "FOR", "IN", "SWITCH",
                     "CASE", "BREAK", "DEFAULT", "WHILE"};

typedef char *buffer;

int state;

struct TokenInfo
{
    char token[30];
    char value[30];
    int line;
};

typedef struct TokenInfo tokenInfo;

struct Node
{
    tokenInfo *tk;
};

typedef struct Node node;

struct Header
{
    node *headLink;
};

typedef struct Header header;