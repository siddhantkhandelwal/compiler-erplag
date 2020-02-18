#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexerDef.h"
#define BUFFLEN 4096

FILE *getStream(FILE *fp);
tokenInfo *getNextToken(FILE **fp);
void removeComments(char *testCaseFile, char *cleanFile);
int keywordChecker(char val[]);
void flush(char *str, int elementsToFlush);
/*
node *createNode();
header initializeHead();
*/