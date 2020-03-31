#include "symbolTable.h"

scope* create_new_scope(scope* parent, char* stamp){

	scope* temp = malloc(sizeof(scope));

	temp->next = NULL;
	temp->prev = NULL;
	temp->left_child = NULL;
	temp->right_child = NULL;
	temp->input_list = NULL;
	temp->output_list = NULL;
	temp->head = NULL;

	memset(temp->stamp,0,25);
	strcpy(temp->stamp,stamp);

	if(parent!=NULL){

		if(parent->left_child==NULL){
			parent->left_child = temp;
			parent->right_child = temp;
		}else{

			parent->right_child->next = temp;
			temp->prev = parent->right_child;
			parent->right_child = temp;
		}

		temp->parent = parent;
	}

	return temp;
}

se* lookupst(char* identifier,scope* sc, int is_func){

	se* head;


	while(sc!=NULL){

		head = sc->head;

		while(head!=NULL){
			if(strcmp(head->lexeme,identifier)==0){
				if(!is_func || (is_func && head->is_func))
					return head;
			}

			head = head->next;
		}

		sc = sc->parent;

		if(head->is_func){
			se* itemp = sc->input_list;
			while(itemp){
				if(strcmp(itemp->lexeme,identifier)==0){
					return itemp;
				}

				itemp = itemp->next;
			}

			se* otemp = sc->output_list;
			while(otemp){
				if(strcmp(otemp->lexeme,identifier)==0){
					return otemp;
				}

				otemp = otemp->next;
			}
		}
	}

	/* ERROR ? */
	printf("Error %s Not found.\n",identifier);
	return NULL;
}


se* add_to_scope(tNode* to_add, scope* sc,int is_func, int func_use, type_info* t){

	se* temp;

	if(to_add->parent->node.n->s.N == INPUTPLIST)
		temp = sc->input_list;
	else if(to_add->parent->node.n->s.N == OUTPUTPLIST)
		temp = sc->output_list;
	else
		temp = sc->head;

	se* temp1 = temp;


	while(temp1){
		if(strcmp(temp1->lexeme,to_add->node.l->ti->lexeme)==0){

			if(is_func){
				if(temp1->func_use!=func_use){
					temp1->func_use = 2;
				}

				temp1->used_on_lines[temp1->num_used++] = to_add->node.l->ti->line;
				return temp1;
			}

			printf("Variable Declared Earlier\n");
			return NULL;
		}

		temp1 = temp1->next;
	}

	temp1 = malloc(sizeof(se));
	strcpy(temp1->lexeme,to_add->node.l->ti->lexeme);
	temp1->is_func = is_func;
	temp1->func_use = func_use;
	temp1->type = t;
	temp1->is_array = 0;
	if(t->basic_type==ARRAY){
		temp1->is_array = 1;
	}
	temp1->num_used = 1;
	temp1->used_on_lines[0] = to_add->node.l->ti->line;

	temp1->next = temp;

	if(to_add->parent->node.n->s.N == INPUTPLIST)
		sc->input_list = temp1;
	else if(to_add->parent->node.n->s.N == OUTPUTPLIST)
		sc->output_list = temp1;
	else
		sc->head = temp1;

	return temp1;
}


void populate_st(tNode* head, scope* sc){

	tNode* child = head->node.n->child;
	scope* next_scope = sc;

	while(child!=NULL){

		if(child->leafTag==0){


			if(head->node.n->s.N == MODULEDECLARATIONS){

				child->entry = add_to_scope(child,sc,1,0,NULL);
				child->sc = sc;
			}

			else if(head->node.n->s.N==MODULE){
				child->entry = add_to_scope(child,sc->parent,1,1,NULL);
				child->sc = sc;
				//next_scope = create_new_scope(sc,"module");
			}

			else if(head->node.n->s.N == OUTPUTPLIST && child->node.l->s.T==ID){

				type_info* t = malloc(sizeof(type_info));
				tNode* dt = child->node.l->sibling;
				dt = dt->node.n->child;
				t->basic_type = dt->node.l->s.T;
				child->entry = add_to_scope(child,sc,0,0,t);
				child->sc = sc;
			}

			else if(head->node.n->s.N == INPUTPLIST && child->node.l->s.T==ID){

				type_info* t = malloc(sizeof(type_info));
				tNode* dt = child->node.l->sibling;
				dt = dt->node.n->child;
				t->basic_type = dt->node.l->s.T;

				if(t->basic_type==ARRAY){
					dt = dt->node.l->sibling; // rangearrays
					tNode* tp = dt->node.n->sibling; // type
					tp = tp->node.n->child;
					dt = dt->node.n->child;
					if(dt->node.l->s.T == NUM && dt->node.l->sibling->node.l->s.T==NUM){
						t->start = dt->node.l->ti->value.v1;
						t->end = dt->node.l->sibling->node.l->ti->value.v1;
						t->isStatic = 1;
					}else{
						t->isStatic = 0;
					}

					t->element_type = dt->node.l->s.T;
				}

				child->entry = add_to_scope(child,sc,0,0,t);
				child->sc = sc;
			}			


			else if(head->node.n->s.N==IDLIST && head->parent->node.n->s.N == DECLARESTMT){

				type_info* t = malloc(sizeof(type_info));
				tNode* dt = head->node.l->sibling; // datatype
				dt = dt->node.n->child;
				t->basic_type = dt->node.l->s.T;

				if(t->basic_type==ARRAY){
					dt = dt->node.l->sibling; // rangearrays
					tNode* tp = dt->node.n->sibling; // type
					tp = tp->node.n->child;
					dt = dt->node.n->child;
					if(dt->node.l->s.T == NUM && dt->node.l->sibling->node.l->s.T==NUM){
						t->start = dt->node.l->ti->value.v1;
						t->end = dt->node.l->sibling->node.l->ti->value.v1;
						t->isStatic = 1;
					}else{
						t->isStatic = 0;
					}

					t->element_type = dt->node.l->s.T;
				}

				child->entry = add_to_scope(child,sc,0,0,t);
				child->sc = sc;

			}

			else{

				if(head->node.n->s.N == MODULEREUSESTMT){
					lookupst(child->node.l->ti->lexeme,sc,1);
				}else{
					lookupst(child->node.l->ti->lexeme,sc,0);
				}

			}

		}else{

			if((child->node.n->s.N==MODULE) || (child->node.n->s.N==CONDITIONALSTMT)||(child->node.n->s.N==ITERATIVESTMT)||(child->node.n->s.N==DRIVERMODULE)){
				next_scope = create_new_scope(sc,nonTerminalDict[child->node.n->s.N]);
			}

			populate_st(child,next_scope);

		}

		if(child->leafTag)
        {
            child = child->node.n->sibling;
        }
        else
        {
            child = child->node.l->sibling;
        }
	}

}








