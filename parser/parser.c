#include "parserDef.h"

Grammar *populateGrammar(FILE *fp)
{
    if (fp == NULL)
    {
        printf("Unable to open file\n");
        exit(-1);
    }

    char tnt[MAXLENGTHTNT];
    while (!feof(fp))
    {
        fscanf(fp, "%s", tnt);
        printf("%s\t%ld\n", tnt, strlen(tnt));
        break;
    }
}

int main()
{
    FILE *fp = fopen("grammar", "r");
    Grammar *grammar = populateGrammar(fp);
    return 0;
}