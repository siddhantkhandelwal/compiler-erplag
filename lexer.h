#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexerDef.h"
#define buffLen 4000

FILE *getStream(FILE *fp);
node *getNextToken();
void removeComments(char *testCaseFile, char *cleanFile);
int removeCommentsSource(char *buffer, int pointer);
char *keywordChecker(char val[]);
void flush(char *str, int elementsToFlush);
node *createNode();
header initializeHead();
