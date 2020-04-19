/* LAVANYA SONI - 2017A7PS0151P
    KUSHAGRA AGRAWAL - 2017A7PS0107P
    SIDDHANT KHANDELWAL - 2017A7PS0127P
    GROUP - 11
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "codegen.h"

extern unsigned int ended;
int nodeCount = 0;

void printParseTree(tNode *temp, FILE *fp)
{
    if (!temp)
    {
        return;
    }
    nodeCount++;
    if (temp->leafTag == 0)
    {

        if (temp->node.l->s.T == EPSILON)
        {
            fprintf(fp, "Lexeme: -----\t\tLine no: -----\t\tToken: EPSILON\t\tValue: -----\t\tParent Node: %-10s\t\tLeaf Node: Yes\t\tNode Symbol: -----\n\n", nonTerminalDict[temp->parent->node.n->s.N]);
            return;
        }

        if (temp->node.l->ti == NULL)
        {
            fprintf(fp, "Lexeme: -----\t Line no: -----\t\tToken: %-10s\tValue: -----\t\tParent Node: -----\t\tLeaf Node: ERROR\t Node Symbol: -----\n\n", terminalDict[temp->node.n->s.T]);
            return;
        }

        if (temp->node.l->s.T != EPSILON)
        {
            if (temp->node.l->ti->t == NUM)
            {
                fprintf(fp, "Lexeme: %-10s\t\tLine no: %-10d\t\tToken: %-10s\t\tValue: %-10d\t\tParent Node: %-10s\t\tLeaf Node: Yes\t\tNode Symbol: -----\n\n", temp->node.l->ti->lexeme, temp->node.l->ti->line, terminalDict[temp->node.n->s.T], temp->node.l->ti->value.v1, nonTerminalDict[temp->parent->node.n->s.N]);
            }
            else if (temp->node.l->ti->t == RNUM)
                fprintf(fp, "Lexeme: %-10s\t Line no: %-10d\t\tToken: %-10s\tValue: %-10lf\t\tParent Node: %-10s\t\tLeaf Node: Yes\t Node Symbol: -----\n\n", temp->node.l->ti->lexeme, temp->node.l->ti->line, terminalDict[temp->node.n->s.T], temp->node.l->ti->value.v2, nonTerminalDict[temp->parent->node.n->s.N]);
            else
                fprintf(fp, "Lexeme: %-10s\t Line no: %-10d\t\tToken: %-10s\tValue: -----\t\tParent Node: %-10s\t\tLeaf Node: Yes\t\tNode Symbol: -----\n\n", temp->node.l->ti->lexeme, temp->node.l->ti->line, terminalDict[temp->node.n->s.T], nonTerminalDict[temp->parent->node.n->s.N]);
        }
        return;
    }
    printParseTree(temp->node.n->child, fp);
    if (temp->parent == NULL)
    {
        fprintf(fp, "Lexeme: -----\t\tLine no: -----\t\tToken: -----\t\tValue: -----\t\tParent Node: ROOT\t\tLeaf Node: No\t\tNode Symbol: %s\n\n", nonTerminalDict[temp->node.n->s.N]);
    }
    else
    {
        fprintf(fp, "Lexeme: -----\t\tLine no: -----\t\tToken: -----\t\tValue: -----\t\tParent Node: %s\t\tLeaf Node: No\t\tNode Symbol: %s\n\n", nonTerminalDict[temp->parent->node.n->s.N], nonTerminalDict[temp->node.n->s.N]);
    }

    if (temp->node.n->child == NULL)
        return;
    tNode *temp2 = temp->node.n->child->node.n->sibling;
    while (temp2)
    {
        printParseTree(temp2, fp);
        temp2 = temp2->node.n->sibling;
    }
}

void printAST(tNode *temp, FILE *fp)
{
    if (!temp)
    {
        return;
    }
    nodeCount++;
    if (temp->leafTag == 0)
    {

        if (temp->node.l->s.T == EPSILON)
        {
            fprintf(fp, "Lexeme: -----\t\tLine no: -----\t\tToken: EPSILON\t\tValue: -----\t\tParent Node: %-10s\t\tLeaf Node: Yes\t\tNode Symbol: -----\n\n", nonTerminalDict[temp->parent->node.n->s.N]);
            return;
        }

        if (temp->node.l->ti == NULL)
        {
            fprintf(fp, "Lexeme: -----\t Line no: -----\t\tToken: %-10s\tValue: -----\t\tParent Node: -----\t\tLeaf Node: ERROR\t Node Symbol: -----\n\n", terminalDict[temp->node.n->s.T]);
            return;
        }

        if (temp->node.l->s.T != EPSILON)
        {
            if (temp->node.l->ti->t == NUM)
            {
                if (temp->parent->node.n->is_operator == 0)
                    fprintf(fp, "Lexeme: %-10s\t\tLine no: %-10d\t\tToken: %-10s\t\tValue: %-10d\t\tParent Node: %-10s\t\tLeaf Node: Yes\t\tNode Symbol: -----\n\n", temp->node.l->ti->lexeme, temp->node.l->ti->line, terminalDict[temp->node.n->s.T], temp->node.l->ti->value.v1, nonTerminalDict[temp->parent->node.n->s.N]);
                else
                {
                    fprintf(fp, "Lexeme: %-10s\t\tLine no: %-10d\t\tToken: %-10s\t\tValue: %-10d\t\tParent Node: %-10s\t\tLeaf Node: Yes\t\tNode Symbol: -----\n\n", temp->node.l->ti->lexeme, temp->node.l->ti->line, terminalDict[temp->node.n->s.T], temp->node.l->ti->value.v1, terminalDict[temp->parent->node.n->s.T]);
                }
            }
            else if (temp->node.l->ti->t == RNUM)
                if (temp->parent->node.n->is_operator == 0)
                    fprintf(fp, "Lexeme: %-10s\t Line no: %-10d\t\tToken: %-10s\tValue: %-10lf\t\tParent Node: %-10s\t\tLeaf Node: Yes\t Node Symbol: -----\n\n", temp->node.l->ti->lexeme, temp->node.l->ti->line, terminalDict[temp->node.n->s.T], temp->node.l->ti->value.v2, nonTerminalDict[temp->parent->node.n->s.N]);
                else
                    fprintf(fp, "Lexeme: %-10s\t Line no: %-10d\t\tToken: %-10s\tValue: %-10lf\t\tParent Node: %-10s\t\tLeaf Node: Yes\t Node Symbol: -----\n\n", temp->node.l->ti->lexeme, temp->node.l->ti->line, terminalDict[temp->node.n->s.T], temp->node.l->ti->value.v2, terminalDict[temp->parent->node.n->s.T]);
            else if (temp->parent->node.n->is_operator == 0)
                fprintf(fp, "Lexeme: %-10s\t Line no: %-10d\t\tToken: %-10s\tValue: -----\t\tParent Node: %-10s\t\tLeaf Node: Yes\t\tNode Symbol: -----\n\n", temp->node.l->ti->lexeme, temp->node.l->ti->line, terminalDict[temp->node.n->s.T], nonTerminalDict[temp->parent->node.n->s.N]);
            else
                fprintf(fp, "Lexeme: %-10s\t Line no: %-10d\t\tToken: %-10s\tValue: -----\t\tParent Node: %-10s\t\tLeaf Node: Yes\t\tNode Symbol: -----\n\n", temp->node.l->ti->lexeme, temp->node.l->ti->line, terminalDict[temp->node.n->s.T], terminalDict[temp->parent->node.n->s.T]);
        }
        return;
    }
    printAST(temp->node.n->child, fp);
    if (temp->parent == NULL)
    {
        fprintf(fp, "Lexeme: -----\t\tLine no: -----\t\tToken: -----\t\tValue: -----\t\tParent Node: ROOT\t\tLeaf Node: No\t\tNode Symbol: %s\n\n", nonTerminalDict[temp->node.n->s.N]);
    }
    else
    {
        if (temp->node.n->is_operator == 0)
        {

            fprintf(fp, "Lexeme: -----\t\tLine no: -----\t\tToken: -----\t\tValue: -----\t\tParent Node: %s\t\tLeaf Node: No\t\tNode Symbol: %s\n\n", nonTerminalDict[temp->parent->node.n->s.N], nonTerminalDict[temp->node.n->s.N]);
        }
        else
        {
            if (temp->parent->node.n->is_operator == 0)
                fprintf(fp, "Lexeme: -----\t\tLine no: -----\t\tToken: -----\t\tValue: -----\t\tParent Node: %s\t\tLeaf Node: No\t\tNode Symbol: %s\n\n", nonTerminalDict[temp->parent->node.n->s.N], terminalDict[temp->node.n->s.T]);
            else
                fprintf(fp, "Lexeme: -----\t\tLine no: -----\t\tToken: -----\t\tValue: -----\t\tParent Node: %s\t\tLeaf Node: No\t\tNode Symbol: %s\n\n", terminalDict[temp->parent->node.n->s.T], terminalDict[temp->node.n->s.T]);
        }
    }

    if (temp->node.n->child == NULL)
        return;
    tNode *temp2 = temp->node.n->child->node.n->sibling;
    while (temp2)
    {
        printAST(temp2, fp);
        temp2 = temp2->node.n->sibling;
    }
}

void printLexerOutput(char *testCaseFile)
{
    FILE *fp1 = fopen(testCaseFile, "r");
    fp1 = getStream(fp1);
    FILE **fp = &fp1;

    while (ended == 0)
    {
        tokenInfo *ti = getNextToken(fp);
        if (ended)
        {
            return;
        }
        if (ti == NULL)
        {
            printf("%d\t ******Lexical Errror *******\n", line);
        }
        else
        {
            printf("%d\t%s\t%s\n", ti->line, ti->lexeme, terminalDict[ti->t]);
        }
    }
    fclose(fp1);
}

// void removeComments(char *testCaseFile)
// {
//     FILE *fi = fopen(testCaseFile, "r");
//     if (fi == NULL)
//     {
//         printf("Unable to open input file");
//         exit(-1);
//     }
//     while (!feof(fi))
//     {
//         char c1 = fgetc(fi);
//         char c2;
//         if (c1 == '*')
//         {
//             c2 = fgetc(fi);
//             if (c2 == '*')
//             {
//                 while ((c1 = fgetc(fi)) != '*' || (c2 = fgetc(fi)) != '*')
//                 {
//                     if (c1 == '\n')
//                     {
//                         putchar(c1);
//                     }
//                     continue;
//                 }
//             }
//             else
//             {
//                 putchar(c1);
//                 putchar(c2);
//             }
//         }
//         else
//         {
//             if (c1 == EOF)
//             {
//                 break;
//             }
//             putchar(c1);
//         }
//     }
//     fclose(fi);
// }

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Incorrect number of arguements.\n");
        exit(-1);
    }

    FILE *fpGrammar, *fpSource, *fpPT, *fpASM;
    clock_t start_time, end_time;
    scope *sc;
    tNode *temp;
    double total_CPU_time, total_CPU_time_in_seconds;
    int choice;
    // printf("a. First and follow sets automated.\n");
    // printf("b. Both lexical and syntax analysis modules implemented\n");
    // printf("c. Modules working fine with all the given test cases.\n");

    do
    {
        printf("\n\nEnter a number from 0 - 4:\n");
        printf("0. For exit \n");
        printf("1. For printing the token list (on the console) generated by the lexer\n");
        printf("2. For parsing to verify the syntactic correctness of the input source code and to produce parse tree (On Console)\n");
        printf("3. For printing the Abstract Syntax Tree in appropriate format (On Console)\n");
        printf("4. For displaying the amount of allocated memory and number of nodes to each of parse tree and abstract syntax tree\n");
        printf("5. For printing the Symbol Table giving following information (ten in number) for each variable identifier\n");
        printf("6. For printing the total memory requirement (sum total of widths of all variables in the function scope) for each function\n");
        printf("7. For printing the type expressions and width of array variables in a line for a test case\n");
        printf("8. For compiling to verify the syntactic and semantic correctness of the input source code. Also (on the console) the total time taken by the integrated compiler\n");
        printf("9. For producing assembly code\n");

        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 0:
            break;

        case 1:
            ended = 0;
            printLexerOutput(argv[1]);
            ended = 0;
            break;

        case 2:
            // print PT
            ended = 0;
            flushHash();
            flush_grammar();
            populateHashTable();
            fpGrammar = fopen("grammar", "r");
            populateGrammar(fpGrammar);
            ComputeFirstAndFollowSets();
            populateParseTable();
            fpSource = fopen(argv[1], "r");
            fpSource = getStream(fpSource);
            head = NULL;
            parseInput(&fpSource);
            temp = head;
            printParseTree(temp, stdout);
            fclose(fpGrammar);
            fclose(fpSource);
            ended = 0;
            break;

        case 3:
            // Print AST
            ended = 0;
            flushHash();
            flush_grammar();
            populateHashTable();
            fpGrammar = fopen("grammar", "r");
            populateGrammar(fpGrammar);
            ComputeFirstAndFollowSets();
            populateParseTable();
            fpSource = fopen(argv[1], "r");
            fpSource = getStream(fpSource);
            head = NULL;
            parseInput(&fpSource);
            temp = head;
            printf("Order of traversal: Post-Order. Visit left-most child, then visit node, then visit remaining children.\n");
            constructAST(head);
            sc = make_st(head);
            printAST(temp, stdout);
            fclose(fpGrammar);
            fclose(fpSource);
            ended = 0;
            break;

        case 4:
            // Memory allocated to PT & AST
            ended = 0;
            flushHash();
            flush_grammar();
            populateHashTable();
            fpGrammar = fopen("grammar", "r");
            populateGrammar(fpGrammar);
            ComputeFirstAndFollowSets();
            populateParseTable();
            fpSource = fopen(argv[1], "r");
            fpSource = getStream(fpSource);
            head = NULL;
            parseInput(&fpSource);
            temp = head;
            fpPT = fopen("pt.txt", "w");
            printParseTree(temp, fpPT);
            fclose(fpPT);
            int nodesPT = nodeCount;
            nodeCount = 0;
            temp = head;
            printf("Order of traversal: Post-Order. Visit left-most child, then visit node, then visit remaining children.\n");
            constructAST(head);
            sc = make_st(head);
            fpPT = fopen("ast.txt", "w");
            printAST(temp, fpPT);
            fclose(fpPT);
            fclose(fpGrammar);
            fclose(fpSource);
            int nodesAST = nodeCount;
            printf("\nNodes of Parse Tree: %d\t Allocated Memory: %ld Bytes\n", nodesPT, nodesPT * sizeof(tNode));
            printf("Nodes of Abstract Syntax Tree: %d\t Allocated Memory: %ld Bytes\n", nodesAST, nodesAST * sizeof(tNode));
            double compression = (double)(nodesPT - nodesAST) / nodesPT;
            printf("Compression Percentage: %f\n", compression * 100);
            ended = 0;
            break;

        case 5:
            ended = 0;
            flushHash();
            flush_grammar();
            populateHashTable();
            fpGrammar = fopen("grammar", "r");
            populateGrammar(fpGrammar);
            ComputeFirstAndFollowSets();
            populateParseTable();
            fpSource = fopen(argv[1], "r");
            fpSource = getStream(fpSource);
            head = NULL;
            parseInput(&fpSource);
            temp = head;
            constructAST(head);
            sc = make_st(head);
            fclose(fpGrammar);
            fclose(fpSource);
            printSymbolTable(sc);
            ended = 0;
            break;

        case 6:
            // Printing total memory requirement function wise

        case 7:
            // Static & Dynamic Arrays
            ended = 0;
            flushHash();
            flush_grammar();
            populateHashTable();
            fpGrammar = fopen("grammar", "r");
            populateGrammar(fpGrammar);
            ComputeFirstAndFollowSets();
            populateParseTable();
            fpSource = fopen(argv[1], "r");
            fpSource = getStream(fpSource);
            head = NULL;
            parseInput(&fpSource);
            temp = head;
            constructAST(head);
            sc = make_st(head);
            fclose(fpGrammar);
            fclose(fpSource);
            printStaticDynamicArrays(sc);
            ended = 0;
            break;

        case 8:
            ended = 0;
            start_time = clock();
            flushHash();
            flush_grammar();
            populateHashTable();
            fpGrammar = fopen("grammar", "r");
            populateGrammar(fpGrammar);
            ComputeFirstAndFollowSets();
            populateParseTable();
            fpSource = fopen(argv[1], "r");
            fpSource = getStream(fpSource);
            head = NULL;
            parseInput(&fpSource);
            temp = head;
            constructAST(head);
            sc = make_st(head);
            checkSemantics(head);
            end_time = clock();
            total_CPU_time = (double)(end_time - start_time);
            total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
            printf("\nTotal CPU time: %lf\nTotal CPU time in seconds: %lf\n", total_CPU_time, total_CPU_time_in_seconds);
            ended = 0;
            fclose(fpGrammar);
            fclose(fpSource);
            break;

        case 9:
            ended = 0;
            flushHash();
            flush_grammar();
            populateHashTable();
            fpGrammar = fopen("grammar", "r");
            populateGrammar(fpGrammar);
            ComputeFirstAndFollowSets();
            populateParseTable();
            fpSource = fopen(argv[1], "r");
            fpSource = getStream(fpSource);
            head = NULL;
            parseInput(&fpSource);
            constructAST(head);
            sc = make_st(head);
            fclose(fpGrammar);
            fclose(fpSource);
            checkSemantics(head);
            fpASM = fopen(argv[2], "w");
            codeGenInit(fpASM, head);
            fclose(fpASM);
            ended = 0;
            break;

        default:
            printf("Incorrect choice. Enter a number from 0 to 9.\n");
        }
    } while (choice != 0);
    printf("Bye\n");
    return 0;
}