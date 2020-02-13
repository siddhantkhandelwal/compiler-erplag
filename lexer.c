#include <stdio.h>
#include <stdlib.h>

#define buffLen 4000
FILE *getStream(FILE *fp);
char *getNextToken();
void removeComments(char *testCaseFile, char *cleanFile);
char buff[buffLen];

FILE *getStream(FILE *fp)
{

    for (int i = 0; i < buffLen; i++)
    {
        buff[i] = '\0';
    }
    if (fp == NULL)
    {
        printf("Unable to open file\n");
        exit(-1);
    }
    if (fread(buff, (size_t)buffLen, 1, fp))
    {
        return fp;
    }
    printf("%s\n", buff);
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
    }
    removeComments(argv[1], argv[2]);
    return 0;
}