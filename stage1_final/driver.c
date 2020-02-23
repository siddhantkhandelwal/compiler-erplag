/* LAVANYA SONI - 2017A7PS0151P
    KUSHAGRA AGRAWAL - 2017A7PS0107P
    SIDDHANT KHANDELWAL - 2017A7PS0127P
    GROUP - 11
*/


#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include "parser.h"

extern unsigned int ended;

void printParseTree(tNode* temp, FILE* fp)
{
    if(!temp)
    {
        return;
    }
    if(temp->leafTag == 0)
    {
        
        if(temp->node.l->s.T == EPSILON)
        {
            fprintf(fp, "Lexeme: -----\t\tLine no: -----\t\tToken: EPSILON\t\tValue: -----\t\tParent Node: %-10s\t\tLeaf Node: Yes\t\tNode Symbol: -----\n\n", nonTerminalDict[temp->parent->node.n->s.N]);
            return;
        }

        if(temp->node.l->ti==NULL){
            fprintf(fp, "Lexeme: -----\t Line no: -----\t\tToken: %-10s\tValue: -----\t\tParent Node: -----\t\tLeaf Node: ERROR\t Node Symbol: -----\n\n",terminalDict[temp->node.n->s.T]);
            return;
        }

        if(temp->node.l->s.T != EPSILON){
            if(temp->node.l->ti->t == NUM)
            {
            fprintf(fp, "Lexeme: %-10s\t\tLine no: %-10d\t\tToken: %-10s\t\tValue: %-10d\t\tParent Node: %-10s\t\tLeaf Node: Yes\t\tNode Symbol: -----\n\n", temp->node.l->ti->lexeme, temp->node.l->ti->line,  terminalDict[temp->node.n->s.T], temp->node.l->ti->value.v1, nonTerminalDict[temp->parent->node.n->s.N]);}
            else if(temp->node.l->ti->t == RNUM)
            fprintf(fp, "Lexeme: %-10s\t Line no: %-10d\t\tToken: %-10s\tValue: %-10lf\t\tParent Node: %-10s\t\tLeaf Node: Yes\t Node Symbol: -----\n\n", temp->node.l->ti->lexeme, temp->node.l->ti->line,  terminalDict[temp->node.n->s.T], temp->node.l->ti->value.v2, nonTerminalDict[temp->parent->node.n->s.N]);
            else
            fprintf(fp, "Lexeme: %-10s\t Line no: %-10d\t\tToken: %-10s\tValue: -----\t\tParent Node: %-10s\t\tLeaf Node: Yes\t\tNode Symbol: -----\n\n", temp->node.l->ti->lexeme, temp->node.l->ti->line,  terminalDict[temp->node.n->s.T],  nonTerminalDict[temp->parent->node.n->s.N]);}
            return;
    }
    printParseTree(temp->node.n->child, fp);
    if(temp->parent == NULL)
    {
        fprintf(fp, "Lexeme: -----\t\tLine no: -----\t\tToken: -----\t\tValue: -----\t\tParent Node: ROOT\t\tLeaf Node: No\t\tNode Symbol: %s\n\n",  nonTerminalDict[temp->node.n->s.N]);
    }
    else
    {
        fprintf(fp, "Lexeme: -----\t\tLine no: -----\t\tToken: -----\t\tValue: -----\t\tParent Node: %s\t\tLeaf Node: No\t\tNode Symbol: %s\n\n",  nonTerminalDict[temp->parent->node.n->s.N], nonTerminalDict[temp->node.n->s.N]);

    }
    
    if(temp->node.n->child == NULL)
        return;
    tNode* temp2 = temp->node.n->child->node.n->sibling;
    while(temp2)
    {
        printParseTree(temp2, fp);
        temp2 = temp2->node.n->sibling;
    }
}

void printLexerOutput(char* testCaseFile)
{
    FILE *fp1 = fopen(testCaseFile, "r");
    fp1 = getStream(fp1);
    FILE **fp = &fp1;


    while (ended == 0)
    {
        tokenInfo *ti = getNextToken(fp);
        if(ended){
            return;
        }
        if (ti == NULL)
        {
            printf("%d\t ******Lexical Errror *******\n", line);
        }else{
            printf("%d\t%s\t%s\n", ti->line, ti->lexeme, terminalDict[ti->t]);
        }
    }
    fclose(fp1);
}

void removeComments(char *testCaseFile)
{
    FILE *fi = fopen(testCaseFile, "r");
    if (fi == NULL)
    {
        printf("Unable to open input file");
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
                        putchar(c1);
                    }
                    continue;
                }
            }
            else
            {
                putchar(c1);
                putchar(c2);
            }
        }
        else
        {
            if (c1 == EOF)
            {
                break;
            }
            putchar(c1);
        }
    }
    fclose(fi);
}





int main(int argc, char* argv[]){
  if(argc != 3)
    {
        printf("Incorrect number of arguements.\n");
        exit(-1);
    }

    FILE* fp2, fp1;
    clock_t start_time, end_time;
    double total_CPU_time, total_CPU_time_in_seconds;
    int choice;
    printf("a. First and follow sets automated.\n");
    printf("b. Both lexical and syntax analysis modules implemented\n");
    printf("c. Modules working fine with all the given test cases.\n");

    do{
        printf("\n\nEnter a number from 0 - 4:\n");
        printf("0. For exit \n");
        printf("1. For removal of comments ‐ Print the comment free code on the console\n");
        printf("2. For printing the token list (on the console) generated by the lexer\n");
        printf("3. For parsing and display syntax and lexical errors. Also print the parse tree in output file.\n");
        printf("4. Total time taken by stage 1 code.\n\n");

        scanf("%d",&choice);
        getchar();

        switch (choice)
        {
        case 0:
            break;
        
        case 1:
            removeComments(argv[1]);
            ended = 0;
            break;

        case 2:
            printLexerOutput(argv[1]);
            ended = 0;
            break;

        case 3:
            flushHash();
            flush_grammar();
            populateHashTable();
            FILE* fp3 = fopen("grammar","r");
            populateGrammar(fp3);
            ComputeFirstAndFollowSets();
            populateParseTable();
            fp2 = fopen(argv[1], "r");
            fp2 = getStream(fp2);
            head = NULL;
            parseInput(&fp2);  
            FILE* fp = fopen(argv[2], "w");
            tNode* temp = head;
            printParseTree(temp, fp);
            fclose(fp);
            fclose(fp2);
            fclose(fp3);
            ended = 0;
            break;

        case 4:
            start_time = clock();
            FILE* fp1 ;
            flushHash();
            flush_grammar();
            populateHashTable();
            fp3 = fopen("grammar", "r");
            populateGrammar(fp3);
            ComputeFirstAndFollowSets();
            populateParseTable();
            fp1 = fopen(argv[1], "r");
            fp1 = getStream(fp1);
            
            
            head = NULL;
            parseInput(&fp1);    
            end_time = clock();

            total_CPU_time  =  (double) (end_time - start_time);

            total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;

            printf("\nTotal CPU time: %lf\nTotal CPU time in seconds: %lf\n", total_CPU_time, total_CPU_time_in_seconds);

            ended = 0;

            break;

        
        default:
            printf("Incorrect choice. Enter a number from 0 to 4.\n");
            
        }
    }while(choice != 0);
    printf("Bye\n");
    return 0;
}