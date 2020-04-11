/* LAVANYA SONI - 2017A7PS0151P
    KUSHAGRA AGRAWAL - 2017A7PS0107P
    SIDDHANT KHANDELWAL - 2017A7PS0127P
    GROUP - 11
*/

#ifndef lexHeader
#define lexHeader

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexerDef.h"
extern unsigned int ended;
#define BUFFLEN 4096
extern int line;
FILE *getStream(FILE *fp);
tokenInfo *getNextToken(FILE **fp);
void removeComments(char *testCaseFile);
void printLexerOutput(char *testCaseFile);

int keywordChecker(char val[]);
void flush(char *str, int elementsToFlush);
tokenInfo *createToken(Token t, char *lexeme, unsigned int line);
#endif