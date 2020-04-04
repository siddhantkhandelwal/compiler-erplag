#include "semantics.h"


se* reverse_list(se* head){

	if(!head)
		return NULL;

	se* head_new = head;
	se* temp = head->next;
	head_new->next = NULL;
	while(temp){
		se* temp2 = temp->next;
		temp->next = head_new;
		head_new = temp;
		temp = temp2;
	}

	return head_new;
}

void check_input_parameters(scope* scope_of_func, tNode* input_list){

	se* func_list = scope_of_func->input_list;
	func_list = reverse_list(func_list);

	scope_of_func->input_list = func_list; // Storing the new reversed as the input list in function scope

	input_list = input_list->node.n->child; // input_list is "idlist" initially....Now it is the first child

	if(func_list==NULL){
		if(input_list!=NULL){
			printf("Error : Function does'nt take any input.\n");
		}

		return;
	}

	while(func_list && input_list){

		if(input_list->entry==NULL){
			func_list = func_list->next;
			input_list = input_list->node.l->sibling;			
			continue;
		}

		if(func_list->type->basic_type != input_list->entry->type->basic_type){
			printf("Error : Types of input function parameters don't match.\n");
			return;
		}

		if(func_list->type->basic_type==ARRAY){

			if(func_list->type->element_type != input_list->entry->type->element_type){
				printf("Error : Array passed in the function is of different type than expected\n");
				return;
			}

			if(func_list->type->isStatic && input_list->entry->type->isStatic){

				int out_end = func_list->type->end;
				int out_start = func_list->type->start;
				int passed_end = input_list->entry->type->end;
				int passed_start = input_list->entry->type->start;

				if((out_end-out_start)!=(passed_end-passed_start)){
					printf("Error : Array ranges don't match\n");
					return;
				}
			}
		}

		func_list = func_list->next;
		input_list = input_list->node.l->sibling;
	}

	if(func_list || input_list){
		printf("Error : The number of input function parameters don't match.\n");
	}
}

void check_output_parameters(scope* scope_of_func, tNode* idlist_node){

	// Assuming that idlist_node will not be NULL. Check this condition in the wrapper function.

	// Doubt: Will it be fine if a function returns something but we don't catch it?

	se* output_list = scope_of_func->output_list;

	if(output_list == NULL){
		printf("Error : Function doesn't return anything\n");
		return;
	}

	output_list = reverse_list(output_list);
	scope_of_func->output_list = output_list;
	idlist_node = idlist_node->node.n->child;

	while(output_list && idlist_node){

		if(output_list->num_used==1){
			printf("Variable %s in function is not assigned a value.\n",output_list->lexeme);
		}

		if(idlist_node->entry==NULL){
			output_list = output_list->next;
			idlist_node = idlist_node->node.l->sibling;
			continue;
		}

		if(output_list->type->basic_type != idlist_node->entry->type->basic_type){
			printf("Error : Types of output function parameters don't match.\n");
			return;
		}

		output_list = output_list->next;
		idlist_node = idlist_node->node.l->sibling;
	}

	if(output_list || idlist_node){
		printf("Error : The number of output function parameters don't match.\n");
	}
}

void check_module(tNode* reusestmt_node){

	if(reusestmt_node->entry==NULL){
		return;
	}

	scope* scope_of_func = reusestmt_node->entry->scope_info;

	if(scope_of_func==NULL){
		printf("Error : Function definition not found\n");
		return;
	}

	tNode* child = reusestmt_node->node.n->child;
	tNode* input_list_node = NULL;

	if(child->leafTag==1){

		check_output_parameters(scope_of_func,child);
		input_list_node = child->node.n->sibling->node.l->sibling;
	}else{
		input_list_node = child->node.l->sibling;
	}

	check_input_parameters(scope_of_func,input_list_node);
	
}

void check_semantics(tNode* head){

	tNode* child = head->node.n->child;

	while(child){

		if(child->leafTag==1){

			if(child->node.n->s.N == MODULEREUSESTMT){
				//printf("Yes\n");
				check_module(child);
			}else{
				check_semantics(child);
			}
		}

		if(child->leafTag==0)
			child = child->node.l->sibling;
		else
			child = child->node.n->sibling;
	}
}

/*	
type_info* expressionChecker(tNode* head)
{   
    if(head->leafTag)
    {
        se* temp = head->entry;
        if(temp && temp->type->basic_type == ARRAY)
        {
            //function call.-checkArraysemantics
            //cases to be checked-bound checking, index type checking, array name not used in an expression.
        }
        else{
            if(head->node.l->s.T == ID)
            {
                head->type = temp->type;
            }
            return head->type;
        }
    }
    else if(head->node.n->is_operator  && (head->node.n->s.T == AND || head->node.n->s.T == OR))
    {
        type_info* t1 = expressionChecker(head->node.n->child);
        
        type_info* t2;
        if(head->node.n->child->leafTag == 1)
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
                    printf("Type Error for %s\n", terminalDict[head->node.n->s.T]);
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
        
        type_info* t2;
        if(head->node.n->child->leafTag == 1)
        {
            if(head->node.n->child->node.n->sibling)
                t2 = expressionChecker(head->node.n->child->node.n->sibling);
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
            head->type = t1;
            return t1;
        }
        if(t1->basic_type == ARRAY)
        {
            if(t2->basic_type == ARRAY)
            {
                if(t1->element_type == t2->element_type && (t1->element_type == REAL || t1->element_type == INTEGER))
                {
                    head->type = t1;
                    return t1;
                }
                else{
                    printf("Type Error for %s\n", terminalDict[head->node.n->s.T]);
                    return NULL;
                }
            }
            else{
                if(t1->element_type == t2->basic_type && t2->basic_type != BOOLEAN)
                {
                    head->type = t1;
                    return t1;
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
                head->type = t1;
                return t1;
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
}
*/