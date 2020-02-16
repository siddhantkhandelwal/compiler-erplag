#include "lexer.h"

#define BUFFLEN 4000
#define NOOFKEYWORDS 30

static int lineNo = 1;
char buffer[BUFFLEN];

FILE *getStream(FILE *fp)
{

    for (int i = 0; i < BUFFLEN; i++)
    {
        buffer[i] = '\0';
    }
    if (fp == NULL)
    {
        printf("Unable to open file\n");
        exit(-1);
    }
    if (fread(buffer, (size_t)BUFFLEN, 1, fp))
    {
        return fp;
    }
    printf("%s\n", buffer);
    return NULL;
}

void removeComments(char *testCaseFile, char *cleanFile)
{
    FILE *fi = fopen(testCaseFile, "r+");
    if (fi == NULL)
    {
        printf("Unable to open input file");
        exit(-1);
    }
    FILE *fo = fopen(cleanFile, "w+");
    if (fi == NULL)
    {
        printf("Unable to open output file");
        exit(-1);
    }
    while (!feof(fi))
    {
        char c1 = fgetc(fi);
        char c2;
        if (c1 == '*')
        {
            c2 = fgetc(fi);
            if (c2 == '*')
            {
                while ((c1 = fgetc(fi)) != '*' || (c2 = fgetc(fi)) != '*')
                {
                    if (c1 == '\n')
                    {
                        fputc(c1, fo);
                    }
                    continue;
                }
            }
            else
            {
                fputc(c1, fo);
                fputc(c2, fo);
            }
        }
        else
        {
            if (c1 == EOF)
            {
                break;
            }
            fputc(c1, fo);
        }
    }
    fclose(fi);
    fclose(fo);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Incorrect number of arguments\n");
        exit(-1);
    }
    removeComments(argv[1], argv[2]);
    return 0;
}

node *createNode()
{
    node *thisNode = (node *)malloc(sizeof(node));
    thisNode->tk = (tokenInfo *)malloc(sizeof(tokenInfo));
    return thisNode;
}

header initializeHead()
{
    header newHeader = (header)malloc(sizeof(header));
    newHeader->headLink = NULL;
    return newHeader;
}

char *keywordChecker(char val[])
{
    char *keywordTokenKey = (char *)malloc(sizeof(char) * NOOFKEYWORDS);
    for (int i = 0; i < NOOFKEYWORDS; i++)
    {
        if (strcmp(val, keywordDict[i]) == 0)
        {
            strcpy(keywordTokenKey, tokenKey[i]);
            return keywordTokenKey;
        }
    }
    char notAKeyword[] = "NotAKeyword";
    strcpy(keywordTokenKey, notAKeyword);
    return keywordTokenKey;
}

int removeCommentsSource(char *buffer, int pointer)
{
    while (!((buffer[pointer] == '*') && (buffer[pointer + 1] == '*')))
    {
        pointer++;
        if (buffer[pointer] == '\n')
        {
            lineNo++;
        }
    }
    return pointer;
}

void flush(char *str, int elementsToFlush)
{
    for (int i = 0; i < elementsToFlush; i++)
    {
        str[i] = '\0';
    }
}