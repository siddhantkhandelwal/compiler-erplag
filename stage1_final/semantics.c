#include "symbolTable.h"

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

		if(func_list->type->basic_type != input_list->sc->type->basic_type){
			printf("Error : Types of input function parameters don't match.\n");
			return;
		}

		if(func_list->type->basic_type==ARRAY){

			if(func_list->type->element_type != input_list->sc->type->element_type){
				printf("Error : Array passed in the function is of different type than expected\n");
				return;
			}

			if(func_list->type->isStatic && input_list->sc->type->isStatic){

				int out_end = func_list->type->end;
				int out_start = func_list->type->start;
				int passed_end = input_list->sc->type->end;
				int passed_start = input_list->sc->type->start;

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
		return
	}

	output_list = reverse_list(output_list);
	scope_of_func->output_list = output_list;
	idlist_node = idlist_node->node.n->child;

	while(output_list && idlist_node){

		if(func_list->type->basic_type != input_list->sc->type->basic_type){
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

	
}