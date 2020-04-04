#include "semantics.h"


type_info* arrayChecker(tNode* head)
{
    se* entry = head->entry;
    if(entry && head->node.n->sibling != NULL)
    {
        tNode* index = head->node.n->sibling;
        if(index->leafTag == 0 && index->node.l->s.T == NUM)
        {
            if(entry->type->isStatic == 1)
            {
                //perform bound checking.
                if(index->node.l->ti->value.v1 >= entry->type->start && index->node.l->ti->value.v1 <= entry->type->end)
                {
                    head->type = malloc(sizeof(type_info)); //create a new type_infor structure so that we can adjust is_static as per our needs.
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
                    printf("Error: Array Index out of bounds error\n");
                    return NULL;
                }
            }
            else{
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
            if(index->leafTag == 0 && index->entry && index->entry->type->basic_type == INTEGER)
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
            else{
                head->type = NULL;
                printf("Error: Index of array should be of integer type.\n");
                return NULL;
            }
        }
    }
    else{
        if(entry){
        printf("Error: Array identifier used in an expression without de-referencing, it must be referenced by an integer index.\n");
        }
        head->type = NULL;
        return NULL;
    }
}


type_info* assignmentChecker(tNode* head)
{   
    //head is child of <assignment statement>.
    se* entry = head->entry;
    tNode* head_sibling = head->node.l->sibling;
    if(entry)
    {
        if(entry->type->basic_type == ARRAY)
        {
            if(head_sibling->leafTag == 0)
            {
                type_info* t1 = arrayChecker(head);
                if(t1)
                {
                    type_info* t2 = expressionChecker(head_sibling->node.l->sibling->node.n->child);
                    if(t2)
                    {
                        if(t1->element_type != t2->basic_type)
                        {
                            printf("Error: type error for assignment statement\n");
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
                else{
                    head->type = NULL;
                    return NULL;
                }
            }
            else
            {
                type_info* t1 = entry->type;
                if(head_sibling->node.n->child->leafTag == 0 && head_sibling->node.n->child->node.l->s.T == ID && head_sibling->node.n->child->node.l->sibling == NULL)
                {
                    
                    if(head_sibling->node.n->child->entry)
                    {
                        type_info* t2 = head_sibling->node.n->child->entry->type;
                        if(t2->basic_type == ARRAY && t2->element_type == t2->element_type && t1->isStatic == t2->isStatic &&
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
                            printf("Type mismatch for array assignment\n");
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
            }
            
        }
        else
        {
            type_info* t1 = entry->type;
            type_info* t2 = expressionChecker(head_sibling->node.n->child);
            if(t2)
            {
                if(t1->basic_type == t2->basic_type)
                {
                    head->type = malloc(sizeof(type_info));
                    head->type->basic_type = t1->basic_type;
                    head->type->isStatic = 1;
                    return head->type;
                }
                else{
                    printf("%s\t%s\n", terminalDict[t1->basic_type], terminalDict[t2->basic_type]);
                    head->type = NULL;
                    printf("Type error for %s in assignment statement.\n", head->node.l->ti->lexeme);
                    return NULL;
                }
            }
            else{
                head->type = NULL;
                return NULL;    
            }
        }
        
    }
    return NULL;
}

type_info* ioStmtChecker(tNode* head)
{
    //head is iostmt;
    tNode* var;
    if(head->node.n->child)
    {
        if(head->node.n->child->leafTag == 0 && (head->node.n->child->node.l->s.T == PRINT || head->node.n->child->node.l->s.T == GET_VALUE))
        {
            var = head->node.n->child->node.l->sibling;
            if(var->entry && var->entry->type->basic_type == ARRAY)
            {
                arrayChecker(var);
            }
        }
    }
}

type_info* expressionCheckerInit(tNode* head)
{
    return expressionChecker(head->node.n->child);
}

type_info* expressionChecker(tNode* head)
{   
    if(head->leafTag == 0)
    {
        se* temp = head->entry;
        if(temp && temp->type->basic_type == ARRAY)
        {
            //function call-checkArraysemantics
            //cases to be checked-bound checking, index type checking, array name not used in an expression.
            return arrayChecker(head);
        }
        else{
            if(temp && head->node.l->s.T == ID)
            {
                head->type = malloc(sizeof(type_info));
                head->type->basic_type = temp->type->basic_type;
                head->type->element_type = temp->type->element_type;
                head->type->start = temp->type->start;
                head->type->end = temp->type->end;
                head->type->isStatic = 1;
            }//Type of NUM and RNUM nodes extracted while creating the symbol table.
            if(temp || head->node.l->s.T == NUM || head->node.l->s.T == RNUM || head->node.l->s.T == TRUE || head->node.l->s.T == FALSE)
            {
                return head->type;
            }
            return NULL;
        }
    }
    else if(head->node.n->is_operator  && (head->node.n->s.T == AND || head->node.n->s.T == OR))
    {
        type_info* t1 = expressionChecker(head->node.n->child);
        
        type_info* t2;
        if(head->node.n->child->leafTag == 1)
        {
            
                t2 = expressionChecker(head->node.n->child->node.n->sibling);
            
                
        }
        else
        {
            
                t2 = expressionChecker(head->node.n->child->node.l->sibling);
            
            
        }
        if(t1 == NULL || t2 == NULL)
        {
            head->type = NULL;
            return NULL;
        }
        if(t1->basic_type == BOOLEAN && t2->basic_type == BOOLEAN )
        {
            head->type = malloc(sizeof(type_info));
            head->type->basic_type = BOOLEAN;
            return head->type;
        }
        if(t1->basic_type == ARRAY)
        {
            if(t2->basic_type == ARRAY)
            {
                if(t1->element_type == t2->element_type && t1->element_type == BOOLEAN)
                {
                    head->type = malloc(sizeof(type_info));
                    head->type->basic_type = BOOLEAN;
                    return head->type;
                }
                else{
                    printf("Type Error for %s in line\n", terminalDict[head->node.n->s.T]);
                    return NULL;
                }
            }
            else{
                if(t1->element_type == t2->basic_type && t2->basic_type == BOOLEAN)
                {
                    
                    head->type = malloc(sizeof(type_info));
                    head->type->basic_type = BOOLEAN;
                    return head->type;
                }
                else{
                    printf("Type Error for %s\n", terminalDict[head->node.n->s.T]);
                    return NULL;
                }
            }
        }
        if(t2->basic_type == ARRAY)
        {
            if(t1->basic_type == t2->element_type && t1->basic_type == BOOLEAN)
            {
                
                head->type = malloc(sizeof(type_info));
                head->type->basic_type = BOOLEAN;
                return head->type;
            }
            else{
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

    else if(head->node.n->is_operator  && (head->node.n->s.T == GE || head->node.n->s.T == LE || head->node.n->s.T == LT || head->node.n->s.T == GT || head->node.n->s.T == EQ))
    {
        type_info* t1 = expressionChecker(head->node.n->child);
        
        type_info* t2;
        if(head->node.n->child->leafTag == 1)
        {
            
            t2 = expressionChecker(head->node.n->child->node.n->sibling);
               
        }
        else
        {  
            t2 = expressionChecker(head->node.n->child->node.l->sibling);   
        }
        if(t1 == NULL || t2 == NULL)
        {
            head->type = NULL;
            return NULL;
        }
       
        if((t1->basic_type == REAL && t2->basic_type == REAL )|| (t1->basic_type == INTEGER && t2->basic_type == INTEGER))
        {
            head->type = malloc(sizeof(type_info));
            head->type->basic_type = BOOLEAN;
            return head->type;
        }
        if(t1->basic_type == ARRAY)
        {
            if(t2->basic_type == ARRAY)
            {
                if(t1->element_type == t2->element_type && (t1->element_type == REAL || t1->element_type == INTEGER))
                {
                    head->type = malloc(sizeof(type_info));
                    head->type->basic_type = BOOLEAN;
                    return head->type;
                }
                else{
                    printf("Type Error for %s\n", terminalDict[head->node.n->s.T]);
                    return NULL;
                }
            }
            else{
                if(t1->element_type == t2->basic_type && t2->basic_type != BOOLEAN)
                {
                    
                    head->type = malloc(sizeof(type_info));
                    head->type->basic_type = BOOLEAN;
                    return head->type;
                }
                else{
                    printf("Type Error for %s\n", terminalDict[head->node.n->s.T]);
                    return NULL;
                }
            }
        }
        if(t2->basic_type == ARRAY)
        {
            if(t1->basic_type == t2->element_type && t1->basic_type != BOOLEAN)
            {
                
                head->type = malloc(sizeof(type_info));
                head->type->basic_type = BOOLEAN;
                return head->type;
            }
            else{
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

    else if(head->node.n->is_operator  && (head->node.n->s.T == PLUS || head->node.n->s.T == MINUS || head->node.n->s.T == DIV || head->node.n->s.T == MUL))
    {
        type_info* t1 = expressionChecker(head->node.n->child);
        //printf("%s\n", terminalDict[head->node.n->s.T]);
        type_info* t2;
        if(head->node.n->child->leafTag == 1)
        {
            if(head->node.n->child->node.n->sibling){
                t2 = expressionChecker(head->node.n->child->node.n->sibling);}
            else
               
                return t1;
        }
        else
        {
            if(head->node.n->child->node.l->sibling)
                t2 = expressionChecker(head->node.n->child->node.l->sibling);
            else
                return t1;
        }
        if(t1 == NULL || t2 == NULL)
        {
            head->type = NULL;
            return NULL;
        }
        if((t1->basic_type == REAL && t2->basic_type == REAL )|| (t1->basic_type == INTEGER && t2->basic_type == INTEGER))
        {
            head->type = malloc(sizeof(type_info));
            head->type->basic_type = t1->basic_type;
            head->type->isStatic = 1;
            return head->type;
        }
        else if(t1->basic_type == ARRAY)
        {
            if(t2->basic_type == ARRAY)
            {
                if(t1->element_type == t2->element_type && (t1->element_type == REAL || t1->element_type == INTEGER))
                {
                    head->type = malloc(sizeof(type_info));
                    head->type->basic_type = t2->basic_type;
                    return head->type;
                }
                else{
                    printf("Type Error for %s\n", terminalDict[head->node.n->s.T]);
                    return NULL;
                }
            }
            else{
                if(t1->element_type == t2->basic_type && t2->basic_type != BOOLEAN)
                {
                    head->type = malloc(sizeof(type_info));
                    head->type->basic_type = t2->basic_type;
                    return head->type;
                }
                else{
                    printf("Type Error for %s\n", terminalDict[head->node.n->s.T]);
                    return NULL;
                }
            }
        }
        else if(t2->basic_type == ARRAY)
        {
            if(t1->basic_type == t2->element_type && t1->basic_type != BOOLEAN)
            {
                head->type = malloc(sizeof(type_info));
                head->type->basic_type = t1->basic_type;
                return head->type;
            }
            else{
                printf("Type Error for %s\n", terminalDict[head->node.n->s.T]);
                return NULL;
            }
        }
        else
        {

            printf("Type Error for %s\t%s\t%s\n", terminalDict[head->node.n->child->node.n->s.T], terminalDict[t1->basic_type], terminalDict[t2->basic_type]);
            return NULL;
        }
    }
}

void checkSemantics(tNode* astNode)
{
    tNode* child;
    if(astNode->leafTag == 1)
    {
        child = astNode->node.n->child;
    }
    else
    {
        return;
    }
    while (child != NULL)
    {
        if(child->leafTag != 0)
        {
            //printf("Here\n");
            checkSemantics(child);
            // if(child->node.n->s.N == EXPRESSION)
            // {
            //     //printf("Here1\n");
            //     expressionCheckerInit(child);
            // }
            if(child->node.n->s.N == ASSIGNMENTSTMT)
            {
                //printf("Here2\n");
                assignmentChecker(child->node.n->child);
            }
            else if (child->node.n->s.N == IOSTMT)
            {
                //printf("Here3\n");
                ioStmtChecker(child);
            }
            child = child->node.n->sibling;
        }   
        else
        {
            child = child->node.l->sibling;
        }
        
    }
    
}