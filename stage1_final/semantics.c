#include "symbolTable.h"

type_info *expressionChecker(tNode *head)
{
    if (head->leafTag)
    {
        se *temp = head->entry;
        if (temp && temp->type->basic_type == ARRAY)
        {
            //function call.-checkArraysemantics
            //cases to be checked-bound checking, index type checking, array name not used in an expression.
        }
        else
        {
            if (head->node.l->s.T == ID)
            {
                head->type = temp->type;
            }
            return head->type;
        }
    }
    else if (head->node.n->is_operator && (head->node.n->s.T == AND || head->node.n->s.T == OR))
    {
        type_info *t1 = expressionChecker(head->node.n->child);

        type_info *t2;
        if (head->node.n->child->leafTag == 1)
        {
            //if(head->node.n->child->node.n->sibling)
            t2 = expressionChecker(head->node.n->child->node.n->sibling);
            //else
            return t1;
        }
        else
        {
            //if(head->node.n->child->node.l->sibling)
            t2 = expressionChecker(head->node.n->child->node.l->sibling);
            //else
            return t1;
        }
        if (t1 == NULL || t2 == NULL)
        {
            head->type = NULL;
            return NULL;
        }
        if (t1->basic_type == BOOLEAN && t2->basic_type == BOOLEAN)
        {
            head->type = malloc(sizeof(type_info));
            head->type->basic_type = BOOLEAN;
            return head->type;
        }
        if (t1->basic_type == ARRAY)
        {
            if (t2->basic_type == ARRAY)
            {
                if (t1->element_type == t2->element_type && t1->element_type == BOOLEAN)
                {
                    head->type = malloc(sizeof(type_info));
                    head->type->basic_type = BOOLEAN;
                    return head->type;
                }
                else
                {
                    printf("Type Error for %s\n", terminalDict[head->node.n->s.T]);
                    return NULL;
                }
            }
            else
            {
                if (t1->element_type == t2->basic_type && t2->basic_type == BOOLEAN)
                {

                    head->type = malloc(sizeof(type_info));
                    head->type->basic_type = BOOLEAN;
                    return head->type;
                }
                else
                {
                    printf("Type Error for %s\n", terminalDict[head->node.n->s.T]);
                    return NULL;
                }
            }
        }
        if (t2->basic_type == ARRAY)
        {
            if (t1->basic_type == t2->element_type && t1->basic_type == BOOLEAN)
            {

                head->type = malloc(sizeof(type_info));
                head->type->basic_type = BOOLEAN;
                return head->type;
            }
            else
            {
                printf("Type Error for %s\n", terminalDict[head->node.n->s.T]);
                return NULL;
            }
        }
        else
        {
            printf("Type Error for %s\n", terminalDict[head->node.n->s.T]);
            return NULL;
        }
    }

    else if (head->node.n->is_operator && (head->node.n->s.T == GE || head->node.n->s.T == LE || head->node.n->s.T == LT || head->node.n->s.T == GT || head->node.n->s.T == EQ))
    {
        type_info *t1 = expressionChecker(head->node.n->child);

        type_info *t2;
        if (head->node.n->child->leafTag == 1)
        {
            //if(head->node.n->child->node.n->sibling)
            t2 = expressionChecker(head->node.n->child->node.n->sibling);
            //else
            return t1;
        }
        else
        {
            //if(head->node.n->child->node.l->sibling)
            t2 = expressionChecker(head->node.n->child->node.l->sibling);
            //else
            return t1;
        }
        if (t1 == NULL || t2 == NULL)
        {
            head->type = NULL;
            return NULL;
        }
        if ((t1->basic_type == REAL && t2->basic_type == REAL) || (t1->basic_type == INTEGER && t2->basic_type == INTEGER))
        {
            head->type = malloc(sizeof(type_info));
            head->type->basic_type = BOOLEAN;
            return head->type;
        }
        if (t1->basic_type == ARRAY)
        {
            if (t2->basic_type == ARRAY)
            {
                if (t1->element_type == t2->element_type && (t1->element_type == REAL || t1->element_type == INTEGER))
                {
                    head->type = malloc(sizeof(type_info));
                    head->type->basic_type = BOOLEAN;
                    return head->type;
                }
                else
                {
                    printf("Type Error for %s\n", terminalDict[head->node.n->s.T]);
                    return NULL;
                }
            }
            else
            {
                if (t1->element_type == t2->basic_type && t2->basic_type != BOOLEAN)
                {

                    head->type = malloc(sizeof(type_info));
                    head->type->basic_type = BOOLEAN;
                    return head->type;
                }
                else
                {
                    printf("Type Error for %s\n", terminalDict[head->node.n->s.T]);
                    return NULL;
                }
            }
        }
        if (t2->basic_type == ARRAY)
        {
            if (t1->basic_type == t2->element_type && t1->basic_type != BOOLEAN)
            {

                head->type = malloc(sizeof(type_info));
                head->type->basic_type = BOOLEAN;
                return head->type;
            }
            else
            {
                printf("Type Error for %s\n", terminalDict[head->node.n->s.T]);
                return NULL;
            }
        }
        else
        {
            printf("Type Error for %s\n", terminalDict[head->node.n->s.T]);
            return NULL;
        }
    }

    else if (head->node.n->is_operator && (head->node.n->s.T == PLUS || head->node.n->s.T == MINUS || head->node.n->s.T == DIV || head->node.n->s.T == MUL))
    {
        type_info *t1 = expressionChecker(head->node.n->child);

        type_info *t2;
        if (head->node.n->child->leafTag == 1)
        {
            if (head->node.n->child->node.n->sibling)
                t2 = expressionChecker(head->node.n->child->node.n->sibling);
            else
                return t1;
        }
        else
        {
            if (head->node.n->child->node.l->sibling)
                t2 = expressionChecker(head->node.n->child->node.l->sibling);
            else
                return t1;
        }
        if (t1 == NULL || t2 == NULL)
        {
            head->type = NULL;
            return NULL;
        }
        if ((t1->basic_type == REAL && t2->basic_type == REAL) || (t1->basic_type == INTEGER && t2->basic_type == INTEGER))
        {
            head->type = t1;
            return t1;
        }
        if (t1->basic_type == ARRAY)
        {
            if (t2->basic_type == ARRAY)
            {
                if (t1->element_type == t2->element_type && (t1->element_type == REAL || t1->element_type == INTEGER))
                {
                    head->type = t1;
                    return t1;
                }
                else
                {
                    printf("Type Error for %s\n", terminalDict[head->node.n->s.T]);
                    return NULL;
                }
            }
            else
            {
                if (t1->element_type == t2->basic_type && t2->basic_type != BOOLEAN)
                {
                    head->type = t1;
                    return t1;
                }
                else
                {
                    printf("Type Error for %s\n", terminalDict[head->node.n->s.T]);
                    return NULL;
                }
            }
        }
        if (t2->basic_type == ARRAY)
        {
            if (t1->basic_type == t2->element_type && t1->basic_type != BOOLEAN)
            {
                head->type = t1;
                return t1;
            }
            else
            {
                printf("Type Error for %s\n", terminalDict[head->node.n->s.T]);
                return NULL;
            }
        }
        else
        {
            printf("Type Error for %s\n", terminalDict[head->node.n->s.T]);
            return NULL;
        }
    }
}

void iterativeSemantics(tNode *head)
{
    if (head->node.n->child->node.n->s.T == FOR)
    {
        tNode *forHeader = head->node.n->child;
        tNode *id = forHeader->node.n->sibling;
        if (id->entry == NULL)
        {
            printf("ERROR : (semantics) The identifier '%s' used as the control variable of the FOR loop at line %d, has is not declared\n", id->node.l->ti->lexeme, forHeader->node.n->line);
        }
        else if (forHeader->node.n->sibling->entry->type->basic_type != NUM)
        {
            printf("ERROR : (semantics) The identifier '%s' used as the control variable of the FOR loop at line %d, has to be of INTEGER type\n", id->node.l->ti->lexeme, forHeader->node.n->line);
        }
    }
    else if (head->node.n->child->node.n->s.T == WHILE)
    {
        tNode *whileHeader = head->node.n->child;
        tNode *arbexp = whileHeader->node.n->sibling;
        type_info *ti = expressionChecker(arbexp);
        if (ti->basic_type != BOOLEAN)
        {
            printf("ERROR : (semantics)The control expression of the WHILE loop at line %d, has to be of BOOLEAN type\n", arbexp->node.n->line);
        }
    }
}