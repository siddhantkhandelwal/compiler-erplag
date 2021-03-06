/* LAVANYA SONI - 2017A7PS0151P
    KUSHAGRA AGRAWAL - 2017A7PS0107P
    SIDDHANT KHANDELWAL - 2017A7PS0127P
    GROUP - 11
*/
#include "ast.h"

int isImportant(int nodeSymbol, Terminal *keep_symbols, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (nodeSymbol == keep_symbols[i])
        {
            return 1;
        }
    }
    return 0;
}

// void printParseTree(tNode* temp, FILE* fp)
// {
//     if(!temp)
//     {
//         return;
//     }
//     if(temp->leafTag == 0)
//     {

//         if(temp->node.l->s.T == EPSILON)
//         {
//             fprintf(fp, "Lexeme: -----\t\tLine no: -----\t\tToken: EPSILON\t\tValue: -----\t\tParent Node: %-10s\t\tLeaf Node: Yes\t\tNode Symbol: -----\n\n", nonTerminalDict[temp->parent->node.n->s.N]);
//             return;
//         }

//         if(temp->node.l->ti==NULL){
//             fprintf(fp, "Lexeme: -----\t Line no: -----\t\tToken: %-10s\tValue: -----\t\tParent Node: -----\t\tLeaf Node: ERROR\t Node Symbol: -----\n\n",terminalDict[temp->node.n->s.T]);
//             return;
//         }

//         if(temp->node.l->s.T != EPSILON){
//             if(temp->node.l->ti->t == NUM)
//             {
//             fprintf(fp, "Lexeme: %-10s\t\tLine no: %-10d\t\tToken: %-10s\t\tValue: %-10d\t\tParent Node: %-10s\t\tLeaf Node: Yes\t\tNode Symbol: -----\n\n", temp->node.l->ti->lexeme, temp->node.l->ti->line,  terminalDict[temp->node.n->s.T], temp->node.l->ti->value.v1, nonTerminalDict[temp->parent->node.n->s.N]);}
//             else if(temp->node.l->ti->t == RNUM)
//             fprintf(fp, "Lexeme: %-10s\t Line no: %-10d\t\tToken: %-10s\tValue: %-10lf\t\tParent Node: %-10s\t\tLeaf Node: Yes\t Node Symbol: -----\n\n", temp->node.l->ti->lexeme, temp->node.l->ti->line,  terminalDict[temp->node.n->s.T], temp->node.l->ti->value.v2, nonTerminalDict[temp->parent->node.n->s.N]);
//             else
//             fprintf(fp, "Lexeme: %-10s\t Line no: %-10d\t\tToken: %-10s\tValue: -----\t\tParent Node: %-10s\t\tLeaf Node: Yes\t\tNode Symbol: -----\n\n", temp->node.l->ti->lexeme, temp->node.l->ti->line,  terminalDict[temp->node.n->s.T],  nonTerminalDict[temp->parent->node.n->s.N]);}
//             return;
//     }
//     printParseTree(temp->node.n->child, fp);
//     if(temp->parent == NULL)
//     {
//         fprintf(fp, "Lexeme: -----\t\tLine no: -----\t\tToken: -----\t\tValue: -----\t\tParent Node: ROOT\t\tLeaf Node: No\t\tNode Symbol: %s\n\n",  nonTerminalDict[temp->node.n->s.N]);
//     }
//     else
//     {
//         fprintf(fp, "Lexeme: -----\t\tLine no: -----\t\tToken: -----\t\tValue: -----\t\tParent Node: %s\t\tLeaf Node: No\t\tNode Symbol: %s\n\n",  nonTerminalDict[temp->parent->node.n->s.N], nonTerminalDict[temp->node.n->s.N]);

//     }

//     if(temp->node.n->child == NULL)
//         return;
//     tNode* temp2 = temp->node.n->child->node.n->sibling;
//     while(temp2)
//     {
//         printParseTree(temp2, fp);
//         temp2 = temp2->node.n->sibling;
//     }
// }

void rotate(tNode *above, tNode *below)
{

    tNode *rsib_above = above->parent->node.n->child;
    tNode *prev = NULL;
    tNode* sib = above->node.n->sibling;

    if ((rsib_above->leafTag == 0) || (rsib_above->leafTag && rsib_above->node.n->is_operator == 0))
    {

        while (1)
        {

            if (rsib_above->leafTag == 0)
            {
                tNode *temp = rsib_above->node.l->sibling;
                if (temp->leafTag == 0)
                {
                    prev = rsib_above;
                    rsib_above = temp;
                    continue;
                }
                else
                {
                    if (temp->node.n->is_operator && temp->node.n->s.T == above->node.n->s.T)
                    {
                        prev = rsib_above;
                        rsib_above = temp;
                        break;
                    }
                }
            }
            else
            {
                tNode *temp = rsib_above->node.n->sibling;
                if (temp->leafTag == 0)
                {
                    prev = rsib_above;
                    rsib_above = temp;
                    continue;
                }
                else
                {
                    if (temp->node.n->is_operator && temp->node.n->s.T == above->node.n->s.T)
                    {
                        prev = rsib_above;
                        rsib_above = temp;
                        break;
                    }
                }
            }
        }
    }

    if (prev)
    {
        if (prev->leafTag == 0)
            prev->node.l->sibling = below;
        else
            prev->node.n->sibling = below;
    }
    else
    {
        above->parent->node.n->child = below;
    }

    tNode *below_child1 = below->node.n->child;
    tNode *below_child2;

    if (below_child1->leafTag == 0)
    {
        tNode *temp;
        temp = below_child1->node.l->sibling;
        if (temp->leafTag == 0)
        {
            if (temp->node.l->sibling == NULL)
            {
                below_child2 = temp;
                below_child1->node.l->sibling = NULL;
            }
            else
            {
                below_child2 = temp->node.l->sibling;
                temp->node.l->sibling = NULL;
            }
        }
        else
        {
            if (temp->node.n->sibling == NULL)
            {
                below_child2 = temp;
                below_child1->node.l->sibling = NULL;
            }
            else
            {
                below_child2 = temp->node.n->sibling;
                temp->node.n->sibling = NULL;
            }
        }
    }
    else
    {
        tNode *temp;
        temp = below_child1->node.n->sibling;
        if (temp->leafTag == 0)
        {
            if (temp->node.l->sibling == NULL)
            {
                below_child2 = temp;
                below_child1->node.n->sibling = NULL;
            }
            else
            {
                below_child2 = temp->node.l->sibling;
                temp->node.l->sibling = NULL;
            }
        }
        else
        {
            if (temp->node.n->sibling == NULL)
            {
                below_child2 = temp;
                below_child1->node.n->sibling = NULL;
            }
            else
            {
                below_child2 = temp->node.n->sibling;
                temp->node.n->sibling = NULL;
            }
        }
    }

    tNode *above_child = above->node.n->child;
    if (above_child->leafTag == 0)
    {
        tNode *temp = above_child->node.l->sibling;

        if (temp->leafTag == 0)
        {
            if (temp->node.l->sibling == NULL)
            {
                above_child->node.l->sibling = below_child1;
                above->node.n->sibling = below_child2;
                below->node.n->child = above;
            }
            else
            {
                temp->node.l->sibling = below_child1;
                above->node.n->sibling = below_child2;
                below->node.n->child = above;
            }
        }
        else
        {
            if (temp->node.n->sibling == NULL)
            {
                above_child->node.l->sibling = below_child1;
                above->node.n->sibling = below_child2;
                below->node.n->child = above;
            }
            else
            {
                temp->node.n->sibling = below_child1;
                above->node.n->sibling = below_child2;
                below->node.n->child = above;
            }
        }
    }
    else
    {
        tNode *temp = above_child->node.n->sibling;

        if (temp->leafTag == 0)
        {
            if (temp->node.l->sibling == NULL)
            {
                above_child->node.n->sibling = below_child1;
                above->node.n->sibling = below_child2;
                below->node.n->child = above;
            }
            else
            {
                temp->node.l->sibling = below_child1;
                above->node.n->sibling = below_child2;
                below->node.n->child = above;
            }
        }
        else
        {
            if (temp->node.n->sibling == NULL)
            {
                above_child->node.n->sibling = below_child1;
                above->node.n->sibling = below_child2;
                below->node.n->child = above;
            }
            else
            {
                temp->node.n->sibling = below_child1;
                above->node.n->sibling = below_child2;
                below->node.n->child = above;
            }
        }
    }

    below->parent = above->parent;
    above->parent = below;
    below_child1->parent = above;
    below->node.n->sibling = sib;
}

void deleteNode(tNode *astNode, tNode *child)
{
    tNode *prev = NULL;
    tNode *temp = astNode->node.n->child;
    if (temp == child && child->leafTag == 0)
    {
        astNode->node.n->child = child->node.l->sibling;
    }
    else if (temp == child && child->leafTag == 1)
    {
        astNode->node.n->child = child->node.n->sibling;
    }
    else
    {
        while (temp != child)
        {
            prev = temp;
            if (temp->leafTag)
            {
                temp = temp->node.n->sibling;
            }
            else
            {
                temp = temp->node.l->sibling;
            }
        }
        if (prev && prev->leafTag)
        {
            if (child->leafTag)
            {
                prev->node.n->sibling = child->node.n->sibling;
            }
            else
            {
                prev->node.n->sibling = child->node.l->sibling;
            }
        }
        else
        {
            if (child->leafTag)
            {
                prev->node.l->sibling = child->node.n->sibling;
            }
            else
            {
                prev->node.l->sibling = child->node.l->sibling;
            }
        }
    }
    if (child->leafTag)
    {
        //free(child->node.n);
        return;
    }
    //free(child->node.l);
}

void pullNode(tNode *child_node)
{
    //liftUpNode() is called only when child is a non-terminal.
    tNode *g_child;
    if (child_node->parent->node.n->child == child_node)
    {
        child_node->parent->node.n->child = child_node->node.n->child;
    }
    else
    {
        tNode *prev = NULL;
        tNode *temp = child_node->parent->node.n->child;
        while (temp != child_node)
        {
            prev = temp;
            if (temp->leafTag)
            {
                temp = temp->node.n->sibling;
            }
            else
            {
                temp = temp->node.l->sibling;
            }
        }
        if (prev->leafTag)
        {
            prev->node.n->sibling = child_node->node.n->child;
        }
        else
        {
            prev->node.l->sibling = child_node->node.n->child;
        }
    }
    g_child = child_node->node.n->child;
    tNode *prev = NULL;
    while (g_child)
    {
        g_child->parent = child_node->parent;
        prev = g_child;
        if (g_child->leafTag)
        {
            g_child = g_child->node.n->sibling;
        }
        else
        {
            g_child = g_child->node.l->sibling;
        }
    }
    if (prev->leafTag)
    {
        prev->node.n->sibling = child_node->node.n->sibling;
    }
    else
    {
        prev->node.l->sibling = child_node->node.n->sibling;
    }
}

void constructAST(tNode *astNode)
{
    tNode *child;

    Terminal keep[] = {PRINT, AND, OR, FOR, INTEGER, REAL, BOOLEAN, ARRAY, GET_VALUE, TRUE, FALSE, WHILE, ID, NUM, RNUM, PLUS, MINUS, MUL, DIV, LT, LE, GT, GE, NE, EQ, DEFAULT /*, ASSIGNOP*/};

    if (astNode->leafTag == 1)
    {
        child = astNode->node.n->child;
        while (child != NULL)
        {
            if (child->leafTag == 0)
            {
                if (child->node.l->s.T == START)
                {
                    if (astNode->node.n->s.N == MODULEDEF)
                    {
                        astNode->parent->scope_marker = 1;
                        astNode->parent->scope_start_line = child->node.l->ti->line;
                    }
                    else
                    {
                        astNode->scope_marker = 1;
                        astNode->scope_start_line = child->node.l->ti->line;
                    }
                }
                else if (child->node.l->s.T == END)
                {
                    if (astNode->scope_marker != 1 && astNode->node.n->s.N != MODULEDEF)
                    {
                        printf("\nError in ast.c: Corresponding Start Symbol not found for the End Symbol");
                    }
                    if (astNode->node.n->s.N == MODULEDEF)
                    {
                        astNode->parent->scope_marker = 1;
                        astNode->parent->scope_end_line = child->node.l->ti->line;
                    }
                    else
                    {
                        astNode->scope_end_line = child->node.l->ti->line;
                    }
                }
                if (child->node.l->s.T == EPSILON || !isImportant(child->node.l->ti->t, keep, KEEP_LENGTH))
                {
                    deleteNode(astNode, child);
                }
            }
            else
            {
                constructAST(child);
                if ((child->leafTag && child->node.n->child == NULL))
                {
                    deleteNode(astNode, child);
                }
                else
                {
                    if (child->node.n->s.N == MODULEDECLARATIONS)
                    {
                        pullNode(child->node.n->child);
                        if (child->parent->node.n->s.N == MODULEDECLARATIONS)
                        {
                            pullNode(child);
                        }
                    }
                    else if ((child->node.n->s.N == INPUTPLISTNEW || (child->node.n->s.N == OTHERMODULES /*&& child->parent->node.n->s.N == OTHERMODULES*/) ||
                              child->node.n->s.N == OUTPUTPLISTNEW || (child->node.n->s.N == STATEMENTS && child->parent->node.n->s.N == STATEMENTS) ||
                              child->node.n->s.N == N9 || child->node.n->s.N == IDLISTNEW || child->node.n->s.N == RET ||
                              child->node.n->s.N == OPTIONAL || child->node.n->s.N == STATEMENT || child->node.n->s.N == OP1 || child->node.n->s.N == OP2 ||
                              child->node.n->s.N == RELATIONALOP || child->node.n->s.N == LOGICALOP || child->node.n->s.N == WHICHID || child->node.n->s.N == VARIDNUM ||
                              child->node.n->s.N == LVALUEARRSTMT || child->node.n->s.N == LVALUEIDSTMT || child->node.n->s.N == INDEX || child->node.n->s.N == NEWNT || child->node.n->s.N == BOOLCONSTT) &&
                             !child->node.n->is_operator)
                    {
                        pullNode(child);
                    }
                    else if (child->node.n->child != NULL &&
                             (((child->node.n->child->node.l->ti->t == AND || child->node.n->child->node.l->ti->t == OR) && (child->parent->node.n->s.N == ARITHMETICORBOOLEANEXPRESSION || child->parent->node.n->s.N == N7)) ||
                              ((child->node.n->child->node.l->ti->t == GE || child->node.n->child->node.l->ti->t == GT || child->node.n->child->node.l->ti->t == EQ || child->node.n->child->node.l->ti->t == LE || child->node.n->child->node.l->ti->t == LT) && (child->parent->node.n->s.N == ANYTERM || child->parent->node.n->s.N == N8)) ||
                              ((child->node.n->child->node.l->ti->t == PLUS || child->node.n->child->node.l->ti->t == MINUS) && (child->parent->node.n->s.N == ARITHMETICEXPR || child->parent->node.n->s.N == N4)) ||
                              ((child->node.n->child->node.l->ti->t == MUL || child->node.n->child->node.l->ti->t == DIV) && (child->parent->node.n->s.N == TERM || child->parent->node.n->s.N == N5))))
                    {
                        tNode *temp1 = child->parent->node.n->child;
                        tNode *last = NULL;
                        while (temp1)
                        {
                            last = temp1;
                            if (temp1->leafTag)
                            {
                                temp1 = temp1->node.n->sibling;
                            }
                            else
                            {
                                temp1 = temp1->node.l->sibling;
                            }
                        }

                        if (last == child)
                        {
                            child->parent->node.n->s = child->node.n->child->node.l->s;
                            //printf("%s\n", child->node.n->child->node.l->ti->lexeme);
                            child->parent->node.n->line = child->node.n->child->node.l->ti->line;
                            child->parent->node.n->is_operator = 1;
                            deleteNode(child, child->node.n->child);
                            if (child->node.n->child->leafTag)
                            {
                                if (child->node.n->child->node.n->sibling == NULL)
                                {
                                    if (!child->node.n->is_operator)
                                        pullNode(child);
                                }
                            }
                            else
                            {
                                if (child->node.n->child->node.l->sibling == NULL || child->node.n->child->node.l->sibling->node.l->sibling == NULL)
                                {
                                    if (!child->node.n->is_operator)
                                        pullNode(child);
                                }
                            }
                        }
                    }
                    else if (child->node.n->s.N == ARITHMETICORBOOLEANEXPRESSION ||
                             child->node.n->s.N == ANYTERM || child->node.n->s.N == ARITHMETICEXPR || child->node.n->s.N == TERM || child->node.n->s.N == FACTOR)
                    {
                        if (child->node.n->is_operator == 0)
                            pullNode(child);
                    }
                }
            }
            // if (child->leafTag && child->node.n->is_operator && child->node.n->s.T == MINUS)
            // {
            //     tNode *rchild = child->node.n->child;
            //     tNode *below = NULL;
            //     tNode *temp;
            //     if (rchild->leafTag == 0)
            //         temp = rchild->node.l->sibling;
            //     else
            //         temp = rchild->node.n->sibling;
            //     if (temp->leafTag && temp->node.n->is_operator && (temp->node.n->s.T == PLUS || temp->node.n->s.T == MINUS))
            //     {
            //         below = temp;
            //     }
            //     else
            //     {
            //         tNode *temp2;
            //         if (temp->leafTag == 0)
            //             temp2 = temp->node.l->sibling;
            //         else
            //             temp2 = temp->node.n->sibling;

            //         if (temp2 && temp2->leafTag && temp2->node.n->is_operator && temp2->node.n->s.T == PLUS)
            //             below = temp2;
            //     }

            //     if (below)
            //     {
            //         rotate(child, below);
            //     }
            // }
            // if (child->leafTag && child->node.n->is_operator && (child->node.n->s.T == MINUS || child->node.n->s.T == PLUS) && child->node.n->child->leafTag && child->node.n->child->node.n->is_operator && child->node.n->child->node.n->s.T == DIV)
            // {
            //     tNode *rchild = child->node.n->child->node.n->child;
            //     tNode *below = NULL;
            //     tNode *temp;
            //     if (rchild->leafTag == 0)
            //         temp = rchild->node.l->sibling;
            //     else
            //         temp = rchild->node.n->sibling;
            //     if (temp->leafTag && temp->node.n->is_operator && (temp->node.n->s.T == MUL || temp->node.n->s.T ==DIV))
            //     {
            //         below = temp;
            //     }
            //     else
            //     {
            //         tNode *temp2;
            //         if (temp->leafTag == 0)
            //             temp2 = temp->node.l->sibling;
            //         else
            //             temp2 = temp->node.n->sibling;

            //         if (temp2 && temp2->leafTag && temp2->node.n->is_operator && temp2->node.n->s.T == MUL)
            //             below = temp2;
            //     }

            //     if (below)
            //     {
            //         rotate(child->node.n->child, below);
            //     }
            //}
            if (child->leafTag)
            {
                child = child->node.n->sibling;
            }
            else
            {
                child = child->node.l->sibling;
            }
        }
       
        if (astNode->node.n->s.N == ASSIGNMENTSTMT)
        {
            tNode *temp = astNode->node.n->child;
            tNode *temp2 = temp->node.l->sibling;
            temp->node.l->sibling = temp2->node.n->child;
            //astNode->node.n->s.T = ASSIGNOP;
        }
    }
}
