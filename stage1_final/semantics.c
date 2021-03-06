#include "semantics.h"

int semanticErrors = 0;

type_info *arrayChecker(tNode *head)
{
    se *entry = head->entry;
    if (entry && head->parent->node.n->is_operator)
    {
        if (head->parent->node.n->child == head)
        {
            if ((head->node.l->sibling->leafTag == 0 && head->node.l->sibling->node.l->sibling == NULL) || head->node.l->sibling->leafTag)
            {
                printf("Line %d: Array variable used without de-referencing.\n", head->node.l->ti->line);
                semanticErrors = 1;
                head->type = NULL;
                return NULL;
            }
        }
        else
        {
            if (head->node.l->sibling == NULL)
            {
                printf("Line %d: Array variable used without de-referencing.\n", head->node.l->ti->line);
                semanticErrors = 1;
                head->type = NULL;
                return NULL;
            }
        }
    }
    if (entry && head->node.l->sibling != NULL)
    {
        tNode *index = head->node.l->sibling;
        if (index->leafTag == 0 && index->node.l->s.T == NUM)
        {
            if (entry->type->isStatic == 1)
            {
                //perform bound checking.
                if (index->node.l->ti->value.v1 >= entry->type->start && index->node.l->ti->value.v1 <= entry->type->end)
                {
                    head->type = malloc(sizeof(type_info)); //create a new type_info structure so that we can adjust is_static as per our needs.
                    head->type->basic_type = ARRAY;
                    head->type->element_type = entry->type->element_type;
                    head->type->start = entry->type->start;
                    head->type->end = entry->type->end;
                    head->type->isStatic = 1;
                    return head->type;
                }
                else
                {
                    head->type = NULL;
                    printf("Line %d: Array Index out of bounds error.\n", head->node.l->ti->line);
                    semanticErrors = 1;
                    return NULL;
                }
            }
            else
            {
                head->type = malloc(sizeof(type_info));
                head->type->basic_type = ARRAY;
                head->type->element_type = entry->type->element_type;
                head->type->start = entry->type->start;
                head->type->end = entry->type->end;
                head->type->isStatic = 0; //array is dynamic.
                return head->type;
            }
        }
        else
        {
            if (index->leafTag == 0 && index->entry && index->entry->type->basic_type == INTEGER)
            {
                index->type = malloc(sizeof(type_info));
                index->type->basic_type = index->entry->type->basic_type;
                head->type = malloc(sizeof(type_info));
                head->type->basic_type = ARRAY;
                head->type->element_type = entry->type->element_type;
                head->type->start = entry->type->start;
                head->type->end = entry->type->end;
                head->type->isStatic = 0; //index is dynamic.
                return head->type;
            }
            else
            {
                head->type = NULL;
                printf("Line %d: Index of array should be of integer type.\n", head->node.l->ti->line);
                semanticErrors = 1;
                return NULL;
            }
        }
    }
    else
    {
        if (entry)
        {
            printf("Line %d: Array identifier used in an expression without de-referencing, it must be referenced by an integer index.\n", head->node.l->ti->line);
            semanticErrors = 1;
        }
        head->type = NULL;
        return NULL;
    }
}

type_info *assignmentChecker(tNode *head)
{
    //head is child of <assignment statement>.
    se *entry = head->entry;
    tNode *head_sibling = head->node.l->sibling;
    if (entry)
    {
        // entry->is_control_changed = 1;
        if (entry->type->basic_type == ARRAY)
        {
            if (head_sibling->leafTag == 0)
            {
                type_info *t1 = arrayChecker(head);
                if (t1)
                {
                    type_info *t2 = expressionChecker(head_sibling->node.l->sibling->node.n->child);
                    if (t2)
                    {

                        if (t2->basic_type == ARRAY)
                        {
                            if (t1->element_type != t2->element_type)
                            {
                                printf("Line %d: Type error for assignment statement.\n", head->node.l->ti->line);
                                semanticErrors = 1;
                                head->type = NULL;
                                return NULL;
                            }
                            else
                            {
                                return entry->type;
                            }
                        }
                        else if (t1->element_type != t2->basic_type)
                        {
                            printf("Line %d: Type error for assignment statement.\n", head->node.l->ti->line);
                            semanticErrors = 1;
                            head->type = NULL;
                            return NULL;
                        }
                    }
                    else
                    {

                        head->type = NULL;
                        return NULL;
                    }
                }
                else
                {
                    head->type = NULL;
                    return NULL;
                }
            }
            else
            {
                type_info *t1 = entry->type;
                if (head_sibling->node.n->child->leafTag == 0 && head_sibling->node.n->child->node.l->s.T == ID && head_sibling->node.n->child->node.l->sibling == NULL)
                {

                    if (head_sibling->node.n->child->entry)
                    {
                        type_info *t2 = head_sibling->node.n->child->entry->type;
                        if (t2->basic_type == ARRAY && t2->element_type == t2->element_type && t1->isStatic == t2->isStatic &&
                            t2->start == t1->start && t2->end == t1->end)
                        {
                            head->type = malloc(sizeof(type_info));
                            head->type->basic_type = ARRAY;
                            head->type->element_type = t1->element_type;
                            head->type->start = t1->start;
                            head->type->end = t1->end;
                            return head->type;
                        }
                        else
                        {
                            printf("Line %d: Type mismatch for array assignment.\n", head->node.l->ti->line);
                            semanticErrors = 1;
                            head->type = NULL;
                            return NULL;
                        }
                    }
                    else
                    {
                        head->type = NULL;
                        return NULL;
                    }
                }
                else
                {
                    printf("Line %d: Array variable used without dereferencing.\n", head->node.l->ti->line);
                    semanticErrors = 1;
                }
            }
        }
        else
        {
            type_info *t1 = entry->type;
            //printf("%s\t%s\n", entry->lexeme, terminalDict[t1->basic_type]);
            type_info *t2 = expressionChecker(head_sibling->node.n->child);
            if (t2)
            {
                if (t1->basic_type == t2->basic_type)
                {

                    head->type = malloc(sizeof(type_info));
                    head->type->basic_type = t1->basic_type;
                    head->type->isStatic = 1;
                    //head->entry->is_control_changed = 1;
                    return head->type;
                }
                else if (t2->basic_type == ARRAY)
                {
                    if (t1->basic_type == t2->element_type)
                    {

                        head->type = malloc(sizeof(type_info));
                        head->type->basic_type = t1->basic_type;
                        head->type->isStatic = 1;
                        return head->type;
                    }

                    head->type = NULL;
                    printf("Line %d: Type error for  in assignment statement.\n", head->node.l->ti->line);
                    semanticErrors = 1;
                    return NULL;
                }
                else
                {
                    head->type = NULL;
                    printf("Line %d: Type error for  in assignment statement.\n", head->node.l->ti->line);
                    semanticErrors = 1;
                    return NULL;
                }
            }
            else
            {
                head->type = NULL;
                return NULL;
            }
        }
    }
    return NULL;
}

type_info *ioStmtChecker(tNode *head)
{
    //head is iostmt;
    tNode *var;
    if (head->node.n->child)
    {
        if (head->node.n->child->leafTag == 0 && (head->node.n->child->node.l->s.T == PRINT || head->node.n->child->node.l->s.T == GET_VALUE))
        {
            var = head->node.n->child->node.l->sibling;
            if (var->entry && var->entry->type->basic_type == ARRAY)
            {
                if (var->node.l->sibling)
                {
                    arrayChecker(var);
                }
            }
        }
    }
}

type_info *expressionCheckerInit(tNode *head)
{
    return expressionChecker(head->node.n->child);
}

type_info *expressionChecker(tNode *head)
{
    if (head->leafTag == 0)
    {
        se *temp = head->entry;
        if (temp && temp->type->basic_type == ARRAY)
        {
            //function call-checkArraysemantics
            //cases to be checked-bound checking, index type checking, array name not used in an expression.
            //printf("%s")
            //printf("%s\n", head->node.l->ti->lexeme);
            return arrayChecker(head);
        }
        else
        {
            if (temp && head->node.l->s.T == ID)
            {
                head->type = malloc(sizeof(type_info));
                head->type->basic_type = temp->type->basic_type;
                head->type->element_type = temp->type->element_type;
                head->type->start = temp->type->start;
                head->type->end = temp->type->end;
                head->type->isStatic = 1;
            } //Type of NUM and RNUM nodes extracted while creating the symbol table.
            if (temp || head->node.l->s.T == NUM || head->node.l->s.T == RNUM || head->node.l->s.T == TRUE || head->node.l->s.T == FALSE)
            {
                return head->type;
            }
            return NULL;
        }
    }
    else if (head->node.n->is_operator && (head->node.n->s.T == AND || head->node.n->s.T == OR))
    {
        type_info *t1 = expressionChecker(head->node.n->child);
        //printf("%s\n", terminalDict[t1->basic_type]);
        if (t1)
        {
            type_info *t2;
            if (head->node.n->child->leafTag == 1)
            {

                t2 = expressionChecker(head->node.n->child->node.n->sibling);
                //printf("%s\n", terminalDict[t2->basic_type]);
            }
            else
            {
                if (t1->basic_type != ARRAY && head->node.n->child->node.l->sibling)
                {
                    t2 = expressionChecker(head->node.n->child->node.l->sibling);
                    //printf("%s\n", terminalDict[t2->basic_type]);
                }
                else if (t1->basic_type == ARRAY && head->node.n->child->node.l->sibling)
                {
                    t2 = expressionChecker(head->node.n->child->node.l->sibling->node.l->sibling);
                    //printf("%s\n", terminalDict[t2->basic_type]);
                }
            }
            if (t2 == NULL)
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
                        printf("Line %d: Type Error for %s\n", head->node.n->line, terminalDict[head->node.n->s.T]);
                        semanticErrors = 1;
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
                        printf("Line %d: Type Error for %s\n", head->node.n->line, terminalDict[head->node.n->s.T]);
                        semanticErrors = 1;
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
                    printf("Line %d: Type Error for %s\n", head->node.n->line, terminalDict[head->node.n->s.T]);
                    semanticErrors = 1;
                    return NULL;
                }
            }
            //else
            //{
            printf("Line %d: Type Error for %s\n", head->node.n->line, terminalDict[head->node.n->s.T]);
            semanticErrors = 1;
            return NULL;
            //}
        }
        else
        {

            return NULL;
        }
    }

    else if (head->node.n->is_operator && (head->node.n->s.T == GE || head->node.n->s.T == LE || head->node.n->s.T == LT || head->node.n->s.T == GT || head->node.n->s.T == EQ))
    {
        type_info *t1 = expressionChecker(head->node.n->child);
        if (t1)
        {
            type_info *t2;
            if (head->node.n->child->leafTag == 1)
            {

                t2 = expressionChecker(head->node.n->child->node.n->sibling);
            }
            else
            {
                if (t1->basic_type != ARRAY && head->node.n->child->node.l->sibling)
                    t2 = expressionChecker(head->node.n->child->node.l->sibling);
                else if (t1->basic_type == ARRAY && head->node.n->child->node.l->sibling)
                {
                    t2 = expressionChecker(head->node.n->child->node.l->sibling->node.l->sibling);
                }
            }
            if (t2 == NULL)
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
                        printf("Line %d: Type Error for %s\n", head->node.n->line, terminalDict[head->node.n->s.T]);
                        semanticErrors = 1;
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
                        printf("Line %d: Type Error for %s\n", head->node.n->line, terminalDict[head->node.n->s.T]);
                        semanticErrors = 1;
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
                    printf("Line %d: Type Error for %s\n", head->node.n->line, terminalDict[head->node.n->s.T]);
                    semanticErrors = 1;
                    return NULL;
                }
            }
            else
            {
                printf("Line %d: Type Error for %s\n", head->node.n->line, terminalDict[head->node.n->s.T]);
                semanticErrors = 1;
                return NULL;
            }
        }
        else
        {
            return NULL;
        }
    }

    else if (head->node.n->is_operator && (head->node.n->s.T == PLUS || head->node.n->s.T == MINUS || head->node.n->s.T == DIV || head->node.n->s.T == MUL))
    {
        type_info *t1 = expressionChecker(head->node.n->child);
        //printf("%s\n", terminalDict[head->node.n->s.T]);
        type_info *t2;
        if (t1)
        {
            if (head->node.n->child->leafTag == 1)
            {
                if (head->node.n->child->node.n->sibling)
                {
                    t2 = expressionChecker(head->node.n->child->node.n->sibling);
                }
                else

                    return t1;
            }
            else
            {
                if (t1->basic_type != ARRAY && head->node.n->child->node.l->sibling)
                    t2 = expressionChecker(head->node.n->child->node.l->sibling);
                else if (t1->basic_type == ARRAY && head->node.n->child->node.l->sibling && head->node.n->child->node.l->sibling->node.l->sibling)
                    t2 = expressionChecker(head->node.n->child->node.l->sibling->node.l->sibling);
                else
                    return t1;
            }
            if (t2 == NULL)
            {
                head->type = NULL;
                return NULL;
            }
            if ((t1->basic_type == REAL && t2->basic_type == REAL) || (t1->basic_type == INTEGER && t2->basic_type == INTEGER))
            {
                head->type = malloc(sizeof(type_info));
                head->type->basic_type = t1->basic_type;
                head->type->isStatic = 1;
                return head->type;
            }
            else if (t1->basic_type == ARRAY)
            {
                if (t2->basic_type == ARRAY)
                {
                    if ((t1->element_type == t2->element_type) && (t1->element_type == REAL || t1->element_type == INTEGER))
                    {
                        head->type = malloc(sizeof(type_info));
                        head->type->basic_type = t2->element_type;
                        return head->type;
                    }
                    else
                    {
                        printf("Line %d: Type Error for %s\n", head->node.n->line, terminalDict[head->node.n->s.T]);
                        semanticErrors = 1;
                        return NULL;
                    }
                }
                else
                {
                    if (t1->element_type == t2->basic_type && t2->basic_type != BOOLEAN)
                    {
                        head->type = malloc(sizeof(type_info));
                        head->type->basic_type = t2->basic_type;
                        return head->type;
                    }
                    else
                    {
                        printf("Line %d: Type Error for %s\n", head->node.n->line, terminalDict[head->node.n->s.T]);
                        semanticErrors = 1;
                        return NULL;
                    }
                }
            }
            else if (t2->basic_type == ARRAY)
            {
                if (t1->basic_type == t2->element_type && t1->basic_type != BOOLEAN)
                {
                    head->type = malloc(sizeof(type_info));
                    head->type->basic_type = t1->basic_type;
                    return head->type;
                }
                else
                {
                    printf("Line %d: Type Error for %s\n", head->node.n->line, terminalDict[head->node.n->s.T]);
                    semanticErrors = 1;
                    return NULL;
                }
            }
            else
            {

                printf("Line %d: Type Error for %s\n", head->node.n->line, terminalDict[head->node.n->s.T]);
                semanticErrors = 1;
                return NULL;
            }
        }
        else
        {
            return NULL;
        }
    }
}

se *reverse_list(se *head)
{

    if (!head)
        return NULL;

    se *head_new = head;
    se *temp = head->next;
    head_new->next = NULL;
    while (temp)
    {
        se *temp2 = temp->next;
        temp->next = head_new;
        head_new = temp;
        temp = temp2;
    }

    return head_new;
}

void check_input_parameters(scope *scope_of_func, tNode *input_list)
{

    se *func_list = scope_of_func->input_list;
    if (scope_of_func->label_ip == 0)
    {
        func_list = reverse_list(func_list);

        scope_of_func->input_list = func_list; //Storing the new reversed as the input list in function scope

        scope_of_func->label_ip = 1;
    }

    input_list = input_list->node.n->child; // input_list is "idlist" initially....Now it is the first child

    if (func_list == NULL)
    {
        if (input_list != NULL)
        {
            printf("Error : Function does'nt take any input.\n");
            semanticErrors = 1;
        }

        return;
    }

    while (func_list && input_list)
    {

        if (input_list->entry == NULL)
        {
            func_list = func_list->next;
            input_list = input_list->node.l->sibling;
            continue;
        }

        if (func_list->type->basic_type != input_list->entry->type->basic_type)
        {
            printf("Line %d: Types of input function parameters don't match.\n", input_list->node.l->ti->line);
            semanticErrors = 1;
            return;
        }

        if (func_list->type->basic_type == ARRAY)
        {

            if (func_list->type->element_type != input_list->entry->type->element_type)
            {
                printf("Line %d: Array passed in the function is of different type than expected\n", input_list->node.l->ti->line);
                semanticErrors = 1;
                return;
            }

            if (func_list->type->isStatic && input_list->entry->type->isStatic)
            {

                int out_end = func_list->type->end;
                int out_start = func_list->type->start;
                int passed_end = input_list->entry->type->end;
                int passed_start = input_list->entry->type->start;

                if ((out_end - out_start) != (passed_end - passed_start))
                {
                    printf("Line %d: Array ranges don't match\n", input_list->node.l->ti->line);
                    semanticErrors = 1;
                    return;
                }
            }
        }
        if (input_list->node.l->sibling == NULL && func_list->next)
        {
            printf("Line %d: The number of input function parameters don't match.\n", input_list->node.l->ti->line);
            semanticErrors = 1;
        }
        func_list = func_list->next;
        input_list = input_list->node.l->sibling;
    }
    if (input_list)
    {
        printf("Line %d: The number of input function parameters don't match.\n", input_list->node.l->ti->line);
        semanticErrors = 1;
    }
}

void checkModuleDef(tNode *head)
{

    tNode *outp_list = head->node.n->child->node.l->sibling->node.n->sibling;
    if (outp_list && outp_list->node.n->s.N == OUTPUTPLIST)
    {
        tNode *temp = outp_list->node.n->child;
        while (temp)
        {
            //printf("%s\t%d\n", temp->node.l->ti->lexeme, temp->entry->is_control_changed);
            if (temp->entry->is_control_changed == 0)
            {
                printf("Line %d-%d: The identifier %s not assigned value\n", temp->sc->scope_start_line, temp->sc->scope_end_line, temp->node.l->ti->lexeme);
                semanticErrors = 1;
            }
            temp = temp->node.l->sibling;
            if (temp)
            {
                temp = temp->node.n->sibling;
            }
        }
    }
}

void check_output_parameters(scope *scope_of_func, tNode *idlist_node)
{

    // Assuming that idlist_node will not be NULL. Check this condition in the wrapper function.

    // Doubt: Will it be fine if a function returns something but we don't catch it?

    se *output_list = scope_of_func->output_list;

    if (output_list == NULL)
    {
        printf("Line %d: Function doesn't return anything\n", idlist_node->node.n->child->node.l->ti->line);
        semanticErrors = 1;
        return;
    }
    if (scope_of_func->label_op == 0)
    {
        output_list = reverse_list(output_list);
        scope_of_func->output_list = output_list;
        scope_of_func->label_op = 1;
    }

    idlist_node = idlist_node->node.n->child;

    while (output_list && idlist_node)
    {

        // if (output_list->is_control_changed == 0)
        // {
        //     printf("Variable %s in function is not assigned a value.\n", output_list->lexeme);
        // }

        if (idlist_node->entry == NULL)
        {
            output_list = output_list->next;
            idlist_node = idlist_node->node.l->sibling;
            continue;
        }

        if (output_list->type->basic_type != idlist_node->entry->type->basic_type)
        {
            printf("Line %d: Types of output function parameters don't match. \n", idlist_node->node.l->ti->line);
            semanticErrors = 1;
            return;
        }

        if (idlist_node->node.l->sibling == NULL && output_list->next)
        {
            printf("Line %d: The number of output function parameters don't match.\n", idlist_node->node.l->ti->line);
            semanticErrors = 1;
        }

        idlist_node->entry->is_control_changed = 1;
        output_list = output_list->next;
        idlist_node = idlist_node->node.l->sibling;
    }

    if (idlist_node)
    {
        printf("Line %d: The number of output function parameters don't match.\n", idlist_node->node.l->ti->line);
        semanticErrors = 1;
    }
}

void check_module(tNode *reusestmt_node)
{

    if (reusestmt_node->entry == NULL)
    {
        return;
    }

    scope *scope_of_func = reusestmt_node->entry->scope_info;

    if (scope_of_func == NULL)
    {
        tNode *child = reusestmt_node->node.n->child;
        if (child->leafTag == 1)
        {
            printf("Line %d: Function definition not found\n", child->node.n->sibling->node.l->ti->line);
        }
        else
        {
            printf("Line %d: Function definition not found\n", child->node.l->ti->line);
        }
        semanticErrors = 1;
        return;
    }

    tNode *child = reusestmt_node->node.n->child;
    tNode *input_list_node = NULL;

    if (child->leafTag == 1)
    {

        check_output_parameters(scope_of_func, child);
        input_list_node = child->node.n->sibling->node.l->sibling;
    }
    else
    {
        input_list_node = child->node.l->sibling;
    }

    check_input_parameters(scope_of_func, input_list_node);
}

void iterativeSemantics(tNode *head)
{
    if (head->node.n->child->node.n->s.T == FOR)
    {
        tNode *forHeader = head->node.n->child;
        tNode *id = forHeader->node.n->sibling;
        if (forHeader->node.n->sibling->entry->type->basic_type != INTEGER)
        {
            printf("Line %d: The identifier '%s' used as the control variable of the FOR loop has to be of INTEGER type\n", id->node.l->ti->line, id->node.l->ti->lexeme);
            semanticErrors = 1;
        }
    }
    else if (head->node.n->child->node.n->s.T == WHILE)
    {
        tNode *whileHeader = head->node.n->child;
        tNode *arbexp = whileHeader->node.n->sibling;
        tNode *id = arbexp->node.n->child;
        type_info *ti = expressionChecker(arbexp);
        if (ti && ti->basic_type != BOOLEAN)
        {
            printf("Line %d: The control expression of the WHILE loop has to be of BOOLEAN type\n", whileHeader->node.l->ti->line);
            semanticErrors = 1;
        }
    }
}

void conditionalSemantics(tNode *head)
{
    tNode *id = head->node.n->child;
    tNode *cs = id->node.l->sibling;
    tNode *df = cs->node.n->sibling;
    if (id->entry && id->entry->type->basic_type != INTEGER && id->entry->type->basic_type != BOOLEAN)
    {
        printf("Line %d: The identifier '%s' in switch statement should be of type INTEGER or BOOLEAN.\n", id->node.l->ti->line, id->entry->lexeme);
        semanticErrors = 1;
    }
    else
    {
        if (id->entry && id->entry->type->basic_type == INTEGER && df == NULL)
        {
            printf("Line %d: The INTEGER identifier '%s' in switch statement must be followed by the DEFAULT statement.\n", head->scope_end_line, id->entry->lexeme);
            semanticErrors = 1;
        }
        else if (id->entry && id->entry->type->basic_type == BOOLEAN && df != NULL)
        {
            printf("Line %d: Switch statements with boolean identifier can have only true and false as labels\n", df->node.n->child->node.l->ti->line);
            semanticErrors = 1;
        }
    }
    tNode *temp = cs->node.n->child;
    if (id->entry && id->entry->type->basic_type == BOOLEAN)
    {
        while (temp)
        {
            if (temp->node.n->child->node.l->s.T != TRUE && temp->node.n->child->node.l->s.T != FALSE)
            {
                printf("Line %d: Switch statements with boolean identifier can have only true and false as labels\n", temp->node.n->child->node.l->ti->line);
                semanticErrors = 1;
            }

            if (temp->node.n->sibling)
            {
                temp = temp->node.n->sibling->node.n->sibling;
            }
            else
            {
                temp = NULL;
            }
        }
    }
    else
    {
        while (temp)
        {
            if (id->entry && id->entry->type->basic_type == INTEGER && temp->node.n->child->node.n->s.T != NUM)
            {
                printf("Line %d: Switch statement with INTEGER typed identifier can have only INTEGER values in case statements\n", temp->node.n->child->node.l->ti->line);
                semanticErrors = 1;
            }
            if (temp->node.n->sibling)
            {
                temp = temp->node.n->sibling->node.n->sibling;
            }
            else
            {
                temp = NULL;
            }
        }
    }
}

int controlValidator(tNode *arbexp)
{
    if (arbexp == NULL)
    {
        return 0;
    }
    if (arbexp->leafTag == 0 && arbexp->node.l->s.T == ID)
    {
        if (arbexp->entry)
        {
            if (arbexp->entry->is_control_changed == 1)
            {
                arbexp->entry->is_control_changed = 0;
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else if (arbexp->leafTag == 0 && (arbexp->node.l->s.T == NUM || arbexp->node.l->s.T == RNUM || arbexp->node.l->s.T == TRUE || arbexp->node.l->s.T == FALSE))
    {
        return 0;
    }
    return controlValidator(arbexp->node.n->child) || controlValidator(arbexp->node.n->child->leafTag ? arbexp->node.n->child->node.n->sibling : arbexp->node.n->child->node.l->sibling);
}

void checkSemantics(tNode *astNode)
{
    tNode *child;
    if (astNode->leafTag == 1)
    {
        child = astNode->node.n->child;
    }
    else
    {
        return;
    }
    while (child != NULL)
    {
        if (child->leafTag != 0)
        {
            //printf("Here\n");

            // if(child->node.n->s.N == EXPRESSION)
            // {
            //     //printf("Here1\n");
            //     expressionCheckerInit(child);
            // }
            if (child->node.n->s.N == ITERATIVESTMT)
            {
                if (child->node.n->child->node.n->s.T == WHILE)
                {
                    tNode *whileHeader = child->node.n->child;
                    tNode *arbexp = whileHeader->node.n->sibling;
                    tNode *id = arbexp->node.n->child;
                    if (id->entry)
                    {
                        id->entry->is_control_variable = 1;
                        id->entry->is_control_changed = 0;
                    }
                }
                if (child->node.n->child->node.n->s.T == FOR)
                {
                    tNode *forHeader = child->node.n->child;
                    tNode *id = forHeader->node.n->sibling;
                    // tNode *id = arbexp->node.n->child;
                    if (id->entry)
                    {
                        id->entry->is_control_variable = 1;
                        id->entry->is_control_changed = 0;
                    }
                }
            }
            checkSemantics(child);
            if (child->node.n->s.N == ASSIGNMENTSTMT)
            {
                //printf("Here2\n");
                if (child->node.n->child->entry)
                {
                    child->node.n->child->entry->is_control_changed = 1;
                    //printf("%s\n",child->node.n->child->node.l->ti->lexeme);
                    assignmentChecker(child->node.n->child);
                }
            }
            else if (child->node.n->s.N == IOSTMT)
            {
                //printf("Here3\n");
                ioStmtChecker(child);
            }
            else if (child->node.n->s.N == MODULEREUSESTMT)
            {
                check_module(child);
            }
            else if (child->node.n->s.N == ITERATIVESTMT)
            {
                iterativeSemantics(child);
                if (child->node.n->child->node.n->s.T == WHILE)
                {
                    tNode *whileHeader = child->node.n->child;
                    tNode *arbexp = whileHeader->node.n->sibling;
                    tNode *id = arbexp->node.n->child;
                    int controlResult = controlValidator(arbexp);
                    if (controlResult == 0)
                    {
                        printf("Line %d-%d : The control expression of the WHILE loop has to be modified in the loop body\n", whileHeader->parent->scope_start_line, whileHeader->parent->scope_end_line);
                        semanticErrors = 1;
                    }
                }
                else if (child->node.n->child->node.n->s.T == FOR)
                {
                    tNode *forHeader = child->node.n->child;
                    tNode *id = forHeader->node.n->sibling;
                    if (id->entry)
                    {
                        if (id->entry->is_control_changed)
                        {
                            printf("Line %d-%d: The control variable of the FOR loop has been changed in the loop body\n", forHeader->parent->scope_start_line, forHeader->parent->scope_end_line);
                            semanticErrors = 1;
                        }
                    }
                }
            }
            else if (child->node.n->s.N == CONDITIONALSTMT)
            {
                conditionalSemantics(child);
            }
            else if (child->node.n->s.N == MODULE_NT)
            {
                checkModuleDef(child);
            }

            child = child->node.n->sibling;
        }
        else
        {
            child = child->node.l->sibling;
        }
    }
}
