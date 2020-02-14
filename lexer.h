#include <stdio.h>
#include <stdlib.h>
#include "lexerDef.h"
#define buffLen 4000

FILE *getStream(FILE *fp);
char *getNextToken();
void removeComments(char *testCaseFile, char *cleanFile);
