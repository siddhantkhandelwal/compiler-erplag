/* LAVANYA SONI - 2017A7PS0151P
    KUSHAGRA AGRAWAL - 2017A7PS0107P
    SIDDHANT KHANDELWAL - 2017A7PS0127P
    GROUP - 11
*/
#include "symbolTable.h"

int OFFSET = 0;
int OFFSET_PRINT = 0;
int offsetCounter = 0;
int dyn_arrays = 0;
char func_name[25];

int symbolTableSemanticsErrors = 0;

scope *create_new_scope(scope *parent, char *stamp)
{

	scope *temp = malloc(sizeof(scope));

	temp->next = NULL;
	temp->prev = NULL;
	temp->left_child = NULL;
	temp->right_child = NULL;
	temp->input_list = NULL;
	temp->output_list = NULL;
	temp->head = NULL;
	temp->is_func_used = 0;
	temp->label_ip = 0;
	temp->label_op = 0;
	temp->scope_start_line = -1;
	temp->scope_end_line = -1;
	temp->scope_level = -1;

	memset(temp->stamp, 0, 25);
	strcpy(temp->stamp, stamp);

	if (parent != NULL)
	{

		if (parent->left_child == NULL)
		{
			parent->left_child = temp;
			parent->right_child = temp;
		}
		else
		{

			parent->right_child->next = temp;
			temp->prev = parent->right_child;
			parent->right_child = temp;
		}
	}

	if (strcmp("module", stamp) == 0 || strcmp("drivermodule", stamp) == 0)
	{
		OFFSET = 0;
		OFFSET_PRINT = 0;
	}

	strcpy(temp->func_name, func_name);

	temp->parent = parent;

	return temp;
}

se *lookupst(char *identifier, scope *sc, int is_func, int line_num)
{

	se *head;

	while (sc != NULL)
	{

		head = sc->head;

		while (head != NULL)
		{
			if (strcmp(head->lexeme, identifier) == 0)
			{
				if (!is_func || (is_func && head->is_func))
					head->used_on_lines[head->num_used++] = line_num;
				if (is_func && head->is_func)
				{
					if (head->func_use == 0)
					{
						head->error_flag = 0;
					}
					else if (head->func_use == 2)
					{
						if (head->error_flag != 0)
						{
							printf("Line %d: Declaration of function %s is redundant\n", head->used_on_lines[1], head->lexeme);
							symbolTableSemanticsErrors = 1;
							//return NULL;
						}
					}
				}
				return head;
			}

			head = head->next;
		}

		if (strcmp(sc->stamp, "module") == 0)
		{
			se *itemp = sc->input_list;
			while (itemp)
			{
				if (strcmp(itemp->lexeme, identifier) == 0)
				{
					itemp->used_on_lines[itemp->num_used++] = line_num;
					return itemp;
				}

				itemp = itemp->next;
			}

			se *otemp = sc->output_list;
			while (otemp)
			{
				if (strcmp(otemp->lexeme, identifier) == 0)
				{
					otemp->used_on_lines[otemp->num_used++] = line_num;
					return otemp;
				}

				otemp = otemp->next;
			}
		}

		sc = sc->parent;
	}

	/* ERROR ? */
	printf("Line %d: %s Not found\n", line_num, identifier);
	symbolTableSemanticsErrors = 1;
	return NULL;
}

// se* addElement(se* list, se* temp)
// {
// 	printf("Inside Add element. Adding %s\n", temp->lexeme);
// 	if(list == NULL)
// 	{
// 		return temp;
// 	}
// 	se* temp1 = list;
// 	while (temp1->next)
// 	{
// 		temp1 = temp1->next;
// 	}
// 	temp1->next = temp;
// 	temp->next = NULL;
// 	return list;
// }

se *add_to_scope(tNode *to_add, scope *sc, int is_func, int func_use, type_info *t, scope *func_scope)
{

	se *temp;

	if (to_add->parent->node.n->s.N == INPUTPLIST)
		temp = sc->input_list;
	else if (to_add->parent->node.n->s.N == OUTPUTPLIST)
		temp = sc->output_list;
	else
		temp = sc->head;

	se *temp1 = temp;
	//se* prev = NULL;
	while (temp1)
	{
		//printf("%s\n", temp1->lexeme);
		if (strcmp(temp1->lexeme, to_add->node.l->ti->lexeme) == 0)
		{

			if (is_func)
			{
				if (temp1->func_use == 2 || temp1->func_use == func_use)
				{
					printf("Line %d: Function re-declaration on re-definition for %s\n", to_add->node.l->ti->line, to_add->node.l->ti->lexeme);
					symbolTableSemanticsErrors = 1;
					return NULL;
				}
				// Multiple function declarations or definitions?
				temp1->func_use = 2;
				//printf("%s", func_scope->stamp);
				temp1->scope_info = func_scope;
				temp1->used_on_lines[temp1->num_used++] = to_add->node.l->ti->line;
				return temp1;
			}

			printf("Line %d: Variable %s Declared Earlier\n", to_add->node.l->ti->line, temp1->lexeme);
			symbolTableSemanticsErrors = 1;
			return NULL;
		}
		//printf("Inside if\n");
		//prev = temp1;
		temp1 = temp1->next;
		// if(temp1)
		// {
		// 	printf("%s:prev\n", prev->lexeme);
		// }
	}

	temp1 = malloc(sizeof(se));
	strcpy(temp1->lexeme, to_add->node.l->ti->lexeme);
	temp1->is_func = is_func;
	temp1->func_use = func_use;
	temp1->type = t;
	temp1->is_array = 0;
	if (t && t->basic_type == ARRAY)
	{
		temp1->is_array = 1;
	}
	temp1->num_used = 1;
	temp1->used_on_lines[0] = to_add->node.l->ti->line;
	temp1->scope_info = sc;
	temp1->is_control_variable = 0;
	temp1->is_control_changed = 0;
	temp1->error_flag = 1;
	if (is_func)
		temp1->scope_info = func_scope;

	if (sc->parent != NULL)
	{

		if (temp1->is_array == 0)
		{
			temp1->offset = OFFSET;
			temp1->offset_print = OFFSET_PRINT;

			if (temp1->type->basic_type == INTEGER)
			{
				OFFSET = OFFSET + 4;
				OFFSET_PRINT = OFFSET_PRINT + 2;
			}
			else if (temp1->type->basic_type == REAL)
			{
				OFFSET = OFFSET + 4;
				OFFSET_PRINT = OFFSET_PRINT + 4;
			}
			else if (temp1->type->basic_type == BOOLEAN)
			{
				OFFSET = OFFSET + 4;
				OFFSET_PRINT = OFFSET_PRINT + 1;
			}
		}
		else
		{
			if (to_add->parent->node.n->s.N != INPUTPLIST)
			{

				if (temp1->type->isStatic == 1)
				{
					temp1->offset = OFFSET;
					temp1->offset_print = OFFSET_PRINT;
					OFFSET = OFFSET + 4;
					OFFSET_PRINT = OFFSET_PRINT + 1;

					int start = temp1->type->start;
					int end = temp1->type->end;
					int width = (end - start + 1);
					if (width < 0)
					{
						printf("Line %d: Array end index should be greater than start index.\n", to_add->node.l->ti->line);
						symbolTableSemanticsErrors = 1;
					}
					if (temp1->type->element_type == INTEGER)
					{
						OFFSET = OFFSET + (4 * width);
						OFFSET_PRINT += 2 * width;
					}
					else if (temp1->type->element_type == REAL)
					{
						OFFSET = OFFSET + (4 * width);
						OFFSET_PRINT += 4 * width;
					}
					else if (temp1->type->element_type == BOOLEAN)
					{
						OFFSET = OFFSET + (4 * width);
						OFFSET_PRINT += 1 * width;
					}
				}
				else
				{

					temp1->offset = OFFSET;
					temp1->offset_print = OFFSET_PRINT;
					temp1->type->dyn_index = dyn_arrays;
					dyn_arrays++;
					OFFSET = OFFSET + 4;
					OFFSET_PRINT = OFFSET_PRINT + 1;
				}
			}
			else
			{

				if (temp1->type->isStatic == 0)
				{
					temp1->type->dyn_index = dyn_arrays;
					dyn_arrays++;
				}
				temp1->offset = OFFSET;
				temp1->offset_print = OFFSET_PRINT;
				OFFSET = OFFSET + 4;
				OFFSET_PRINT = OFFSET_PRINT + 5;
			}
		}
	}
	temp1->next = temp;
	if (to_add->parent->node.n->s.N == INPUTPLIST)
	{
		sc->input_list = temp1;
	}

	else if (to_add->parent->node.n->s.N == OUTPUTPLIST)
	{
		sc->output_list = temp1;
	}
	else
	{
		sc->head = temp1;
	}

	return temp1;
}

void populate_st(tNode *head, scope *sc)
{

	tNode *child = head->node.n->child;
	scope *next_scope = sc;

	//printf("%s\n",nonTerminalDict[head->node.n->s.N]);

	/*if(head->node.n->is_operator==1)
		printf("Yes\n");*/

	while (child != NULL)
	{

		if (child->leafTag == 0)
		{

			if (child->node.l->s.T == ID)
			{

				//	printf("%s %s\n",child->node.l->ti->lexeme,nonTerminalDict[head->node.n->s.N]);
				//printf("%s\n",child->node.l->ti->lexeme);

				if (head->node.n->s.N == MODULEDECLARATIONS)
				{

					child->entry = add_to_scope(child, sc, 1, 0, NULL, NULL);
					child->sc = sc;
				}

				else if (head->node.n->s.N == MODULE_NT)
				{

					child->entry = add_to_scope(child, sc->parent, 1, 1, NULL, sc);
					child->sc = sc;
					//next_scope = create_new_scope(sc,"module");
				}

				else if (head->node.n->s.N == OUTPUTPLIST && child->node.l->s.T == ID)
				{

					type_info *t = malloc(sizeof(type_info));
					tNode *dt = child->node.l->sibling;
					dt = dt->node.n->child;
					t->basic_type = dt->node.l->s.T;
					child->entry = add_to_scope(child, sc, 0, 0, t, NULL);
					child->sc = sc;
				}

				else if (head->node.n->s.N == INPUTPLIST && child->node.l->s.T == ID)
				{

					type_info *t = malloc(sizeof(type_info));
					tNode *dt = child->node.l->sibling;
					dt = dt->node.n->child;
					t->basic_type = dt->node.l->s.T;

					if (t->basic_type == ARRAY)
					{
						dt = dt->node.l->sibling;		 // rangearrays
						tNode *tp = dt->node.n->sibling; // type
						tp = tp->node.n->child;
						dt = dt->node.n->child;
						if (dt->node.l->s.T == NUM && dt->node.l->sibling->node.l->s.T == NUM)
						{
							t->start = dt->node.l->ti->value.v1;
							t->end = dt->node.l->sibling->node.l->ti->value.v1;
							t->isStatic = 1;
						}
						else
						{
							t->isStatic = 0;
							t->start_dyn = dt;
							t->end_dyn = dt->node.l->sibling;
						}

						t->element_type = tp->node.l->s.T;
					}

					child->entry = add_to_scope(child, sc, 0, 0, t, NULL);
					child->sc = sc;
				}

				else if (head->node.n->s.N == IDLIST && head->parent->node.n->s.N == DECLARESTMT)
				{

					type_info *t = malloc(sizeof(type_info));
					tNode *dt = head->node.l->sibling; // datatype
					dt = dt->node.n->child;
					t->basic_type = dt->node.l->s.T;

					if (t->basic_type == ARRAY)
					{
						dt = dt->node.l->sibling;		 // rangearrays
						tNode *tp = dt->node.n->sibling; // type
						tp = tp->node.n->child;
						dt = dt->node.n->child;
						if (dt->node.l->s.T == NUM && dt->node.l->sibling->node.l->s.T == NUM)
						{
							t->start = dt->node.l->ti->value.v1;
							t->end = dt->node.l->sibling->node.l->ti->value.v1;
							t->isStatic = 1;
						}
						else
						{
							t->isStatic = 0;
							t->start_dyn = dt;
							t->end_dyn = dt->node.l->sibling;
						}

						t->element_type = tp->node.l->s.T;
					}

					child->entry = add_to_scope(child, sc, 0, 0, t, NULL);
					child->sc = sc;
				}

				else
				{

					//printf("%s\n",child->node.l->ti->lexeme);

					if (head->node.n->s.N == MODULEREUSESTMT)
					{
						se *func_entry = lookupst(child->node.l->ti->lexeme, sc, 1, child->node.l->ti->line);
						// Putting scope of the function in MODULEREUSESTMT node. Used later.
						head->entry = func_entry;
						if (func_entry == NULL)
						{
							printf("Line %d: Function should be declared before use. Function semantics will not be checked.\n", child->node.l->ti->line);
							symbolTableSemanticsErrors = 1;
						}
						else
						{
							if (func_entry->scope_info && func_entry->scope_info->is_func_used)
							{
								printf("Line %d: Recursion Detected\n", child->node.l->ti->line);
								symbolTableSemanticsErrors = 1;
							}
						}
					}
					else
					{
						if (child->parent->node.n->s.N != RANGEARRAYS) /*&& (child->parent->parent && child->parent->parent->parent->node.n->s.N!= INPUTPLIST)*/
							child->entry = lookupst(child->node.l->ti->lexeme, sc, 0, child->node.l->ti->line);
						else
						{
							if (child->parent->parent->parent->node.n->s.N == INPUTPLIST)
								;
							//printf("yes\n");
							else
								child->entry = lookupst(child->node.l->ti->lexeme, sc, 0, child->node.l->ti->line);
						}
						//printf("%s %s\n",child->node.l->ti->lexeme,sc->stamp);
					}
				}
			}
			else if (child->node.l->s.T == NUM)
			{
				child->type = malloc(sizeof(type_info));
				child->type->basic_type = INTEGER;
				child->type->isStatic = 1;
			}
			else if (child->node.l->s.T == RNUM)
			{
				child->type = malloc(sizeof(type_info));
				child->type->basic_type = REAL;
				child->type->isStatic = 1;
			}
			else if (child->node.l->s.T == TRUE || child->node.l->s.T == FALSE)
			{
				child->type = malloc(sizeof(type_info));
				child->type->basic_type = BOOLEAN;
				child->type->isStatic = 1;
			}
		}

		else
		{

			// else
			// {
			// 	printf("Scope Marker is not set in the module\n");
			// }
			if ((child->node.n->s.N == MODULE_NT) || (child->node.n->s.N == CONDITIONALSTMT) || (child->node.n->s.N == ITERATIVESTMT) || (child->node.n->s.N == DRIVERMODULE))
			{
				next_scope = create_new_scope(sc, nonTerminalDict[child->node.n->s.N]);
				if (child->scope_marker == 1)
				{
					next_scope->scope_start_line = child->scope_start_line;
					next_scope->scope_end_line = child->scope_end_line;
				}
			}
			if (child->node.n->s.N == MODULE_NT)
			{
				next_scope->is_func_used = 1;
				next_scope->scope_level = 1;
				strcpy(func_name, child->node.n->child->node.l->ti->lexeme);
				strcpy(next_scope->func_name, func_name);
			}
			else if (child->node.n->s.N == DRIVERMODULE)
			{
				next_scope->scope_level = 1;
				strcpy(func_name, "driver");
				strcpy(next_scope->func_name, func_name);
			}
			else if (child->node.n->s.N == CONDITIONALSTMT || child->node.n->s.N == ITERATIVESTMT)
			{
				next_scope->scope_level = next_scope->parent->scope_level + 1;
			}
			if ((child->node.n->s.N == DRIVERMODULE))
			{
				next_scope->scope_level = 1;
			}
			//child->sc = next_scope;
			populate_st(child, next_scope);

			if (child->node.n->s.N == MODULE_NT)
			{
				next_scope->is_func_used = 0;
				next_scope->scope_level = 1;
				next_scope->activationRecordSize = OFFSET_PRINT;
				if (next_scope->output_list != NULL)
				{
					next_scope->activationRecordSize -= next_scope->output_list->offset_print;
					if (next_scope->output_list->type->basic_type == INTEGER)
					{
						next_scope->activationRecordSize -= 2;
					}
					else if (next_scope->output_list->type->basic_type == REAL)
					{
						next_scope->activationRecordSize -= 4;
					}
					else if (next_scope->output_list->type->basic_type == BOOLEAN)
					{
						next_scope->activationRecordSize -= 1;
					}
				}
			}

			else if (child->node.n->s.N == DRIVERMODULE)
			{
				next_scope->activationRecordSize = OFFSET_PRINT;
			}

			next_scope = sc;
		}

		if (child->leafTag)
		{
			child = child->node.n->sibling;
		}
		else
		{
			child = child->node.l->sibling;
		}
	}
}

scope *make_st(tNode *head)
{

	scope *first_scope = create_new_scope(NULL, "program");
	first_scope->scope_level = 0;
	populate_st(head, first_scope);
	return first_scope;
}

// void printScope(scope *sc)
// {
// 	printf("Scope Stamp: %s\n", sc->stamp);
// 	if (strcmp(sc->stamp, "module") == 0)
// 	{
// 		se *ip_list = sc->input_list;
// 		while (ip_list)
// 		{
// 			if (ip_list->type->basic_type == ARRAY)
// 			{
// 				printf("Lexeme: %s\tType: %s\t Element Type: %s\tStart: %d\tENd: %d\tNum_USed: %d\n", ip_list->lexeme, terminalDict[ip_list->type->basic_type], terminalDict[ip_list->type->element_type],
// 					   ip_list->type->start, ip_list->type->end, ip_list->num_used);
// 			}
// 			printf("Lexeme: %s\tType: %s\tNum_USed: %d\n", ip_list->lexeme, terminalDict[ip_list->type->basic_type], ip_list->num_used);
// 			ip_list = ip_list->next;
// 		}
// 		se *op_list = sc->output_list;
// 		while (op_list)
// 		{
// 			if (op_list->type->basic_type == ARRAY)
// 			{
// 				printf("Lexeme: %s\tType: %s\t Element Type: %s\tStart: %d\tENd: %d\tNum_USed: %d\n", op_list->lexeme, terminalDict[op_list->type->basic_type], terminalDict[op_list->type->element_type],
// 					   op_list->type->start, op_list->type->end, op_list->num_used);
// 			}
// 			printf("Lexeme: %s\tType: %s\tNum_USed: %d\n", op_list->lexeme, terminalDict[op_list->type->basic_type], op_list->num_used);
// 			op_list = op_list->next;
// 		}
// 	}
// 	se *head = sc->head;
// 	while (head)
// 	{
// 		if (!head->is_func)
// 		{
// 			if (head->type->basic_type == ARRAY)
// 			{
// 				printf("Lexeme: %s\tType: %s\t Element Type: %s\tStart: %d\tENd: %d\tNum_USed: %d\n", head->lexeme, terminalDict[head->type->basic_type], terminalDict[head->type->element_type],
// 					   head->type->start, head->type->end, head->num_used);
// 			}
// 			printf("Lexeme: %s\tType: %s\tNum_USed: %d\n", head->lexeme, terminalDict[head->type->basic_type], head->num_used);
// 		}
// 		else
// 		{
// 			printf("Lexeme: %s\tType: Function\tNum_USed: %d\n", head->lexeme, head->num_used);
// 		}

// 		head = head->next;
// 	}
// }

void printScope(scope *sc)
{
	if (strcmp(sc->stamp, "drivermodule") == 0)
	{
		offsetCounter = 0;
	}
	if (strcmp(sc->stamp, "module") == 0)
	{
		offsetCounter = 0;
		// input list; head points to start of input list
		se *head = sc->input_list;
		while (head)
		{
			if (head->type->basic_type == ARRAY)
			{
				int width = (head->type->end - head->type->start) + 1;
				int array_size;
				if (head->type->element_type == INTEGER)
				{
					array_size = (width)*2;
				}
				else if (head->type->element_type == BOOLEAN)
				{
					array_size = (width)*1;
				}
				else
				{
					array_size = (width)*4;
				}

				if (head->type->isStatic == 1)
				{
					printf("%s\t%s\t%d-%d\t%d\tyes\tstatic array\t[%d, %d]\t%s\t%d\t%d\n", head->lexeme, head->scope_info->func_name, head->scope_info->scope_start_line, head->scope_info->scope_end_line, array_size + 1, head->type->start, head->type->end, terminalDict[head->type->element_type], head->offset_print, 0);
				}
				else
					// need to add the ids of start and end index
					printf("%s\t%s\t%d-%d\t%d\tyes\tdynamic array\t[%s, %s]\t%s\t%d\t%d\n", head->lexeme, head->scope_info->func_name, head->scope_info->scope_start_line, head->scope_info->scope_end_line, head->width + 1, head->type->start_dyn->node.l->ti->lexeme, head->type->end_dyn->node.l->ti->lexeme, terminalDict[head->type->element_type], head->offset_print, 0);
				offsetCounter += 5;
			}
			else
			{
				if (head->type->basic_type == REAL)
				{
					printf("%s\t%s\t%d-%d\t4\tno\t---\t---\t%s\t%d\t%d\n", head->lexeme, head->scope_info->func_name, head->scope_info->scope_start_line, head->scope_info->scope_end_line, terminalDict[head->type->basic_type], head->offset_print, 0);
					offsetCounter += 4;
				}
				else if (head->type->basic_type == INTEGER)
				{
					printf("%s\t%s\t%d-%d\t2\tno\t---\t---\t%s\t%d\t%d\n", head->lexeme, head->scope_info->func_name, head->scope_info->scope_start_line, head->scope_info->scope_end_line, terminalDict[head->type->basic_type], head->offset_print, 0);
					offsetCounter += 2;
				}
				else
				{
					printf("%s\t%s\t%d-%d\t1\tno\t---\t---\t%s\t%d\t%d\n", head->lexeme, head->scope_info->func_name, head->scope_info->scope_start_line, head->scope_info->scope_end_line, terminalDict[head->type->basic_type], head->offset_print, 0);
					offsetCounter += 1;
				}
			}
			head = head->next;
		}
		// output list; head points to start of output list
		head = sc->output_list;
		while (head)
		{
			// if (head->type->basic_type == ARRAY)
			// {
			// 	int width = (head->type->end - head->type->start) + 1;
			// 	int array_size;
			// 	if (head->type->element_type == INTEGER)
			// 	{
			// 		array_size = (width)*2;
			// 	}
			// 	else if (head->type->element_type == BOOLEAN)
			// 	{
			// 		array_size = (width)*1;
			// 	}
			// 	else
			// 	{
			// 		array_size = (width)*4;
			// 	}

			// 	if (head->type->isStatic == 1)
			// 	{
			// 		printf("%s\t%s\t%d-%d\t%d\tyes\tstatic array\t[%d, %d]\t%s\t%d\t%d\n", head->lexeme, head->scope_info->func_name, head->scope_info->scope_start_line, head->scope_info->scope_end_line, array_size + 1, head->type->start, head->type->end, terminalDict[head->type->element_type], head->offset_print, head->scope_info->scope_level);
			// 	}
			// 	else
			// 		// need to add the ids of start and end index
			// 		printf("%s\t%s\t%d-%d\t%d\tyes\tdynamic array\t[%d, %d]\t%s\t%d\t%d\n", head->lexeme, head->scope_info->func_name, head->scope_info->scope_start_line, head->scope_info->scope_end_line, head->width + 1, head->type->start, head->type->end, terminalDict[head->type->element_type], head->offset_print, head->scope_info->scope_level);
			// }
			// else
			// {
			if (head->type->basic_type == REAL)
			{
				printf("%s\t%s\t%d-%d\t4\tno\t---\t---\t%s\t%d\t%d\n", head->lexeme, head->scope_info->func_name, head->scope_info->scope_start_line, head->scope_info->scope_end_line, terminalDict[head->type->basic_type], head->offset_print, 0);
				offsetCounter += 4;
			}
			else if (head->type->basic_type == INTEGER)
			{
				printf("%s\t%s\t%d-%d\t2\tno\t---\t---\t%s\t%d\t%d\n", head->lexeme, head->scope_info->func_name, head->scope_info->scope_start_line, head->scope_info->scope_end_line, terminalDict[head->type->basic_type], head->offset_print, 0);
				offsetCounter += 2;
			}
			else
			{
				printf("%s\t%s\t%d-%d\t1\tno\t---\t---\t%s\t%d\t%d\n", head->lexeme, head->scope_info->func_name, head->scope_info->scope_start_line, head->scope_info->scope_end_line, terminalDict[head->type->basic_type], head->offset_print, 0);
				offsetCounter += 1;
			}
			// }
			head = head->next;
		}
	}
	se *head = sc->head;
	while (head)
	{
		if (!head->is_func)
		{
			if (head->type->basic_type == ARRAY)
			{
				int width = (head->type->end - head->type->start) + 1;
				int array_size;
				if (head->type->element_type == INTEGER)
				{
					array_size = (width)*2;
				}
				else if (head->type->element_type == BOOLEAN)
				{
					array_size = (width)*1;
				}
				else
				{
					array_size = (width)*4;
				}

				if (head->type->isStatic == 1)
				{
					printf("%s\t%s\t%d-%d\t%d\tyes\tstatic array\t[%d, %d]\t%s\t%d\t%d\n", head->lexeme, head->scope_info->func_name, head->scope_info->scope_start_line, head->scope_info->scope_end_line, array_size + 1, head->type->start, head->type->end, terminalDict[head->type->element_type], head->offset_print - offsetCounter, head->scope_info->scope_level);
				}
				else
					// need to add the ids of start and end index
					printf("%s\t%s\t%d-%d\t%d\tyes\tdynamic array\t[%s, %s]\t%s\t%d\t%d\n", head->lexeme, head->scope_info->func_name, head->scope_info->scope_start_line, head->scope_info->scope_end_line, head->width + 1, head->type->start_dyn->node.l->ti->lexeme, head->type->end_dyn->node.l->ti->lexeme, terminalDict[head->type->element_type], head->offset_print - offsetCounter, 0);
			}
			else
			{
				if (head->type->basic_type == REAL)
					printf("%s\t%s\t%d-%d\t4\tno\t---\t---\t%s\t%d\t%d\n", head->lexeme, head->scope_info->func_name, head->scope_info->scope_start_line, head->scope_info->scope_end_line, terminalDict[head->type->basic_type], head->offset_print - offsetCounter, head->scope_info->scope_level);
				else if (head->type->basic_type == INTEGER)
					printf("%s\t%s\t%d-%d\t2\tno\t---\t---\t%s\t%d\t%d\n", head->lexeme, head->scope_info->func_name, head->scope_info->scope_start_line, head->scope_info->scope_end_line, terminalDict[head->type->basic_type], head->offset_print - offsetCounter, head->scope_info->scope_level);
				else
					printf("%s\t%s\t%d-%d\t1\tno\t---\t---\t%s\t%d\t%d\n", head->lexeme, head->scope_info->func_name, head->scope_info->scope_start_line, head->scope_info->scope_end_line, terminalDict[head->type->basic_type], head->offset_print - offsetCounter, head->scope_info->scope_level);
			}
		}
		head = head->next;
	}
}

void printSymbolTable(scope *sc)
{
	if (sc == NULL)
	{
		return;
	}
	printScope(sc);
	scope *child = sc->left_child;
	while (child)
	{
		printSymbolTable(child);
		child = child->next;
	}
}

void printStaticDynamicArraysHelper(scope *sc)
{
	se *head = sc->input_list;
	while (head)
	{
		if (head->type->basic_type == ARRAY)
		{
			int width = (head->type->end - head->type->start) + 1;
			int array_size;
			if (head->type->element_type == INTEGER)
			{
				array_size = (width)*2;
			}
			else if (head->type->element_type == BOOLEAN)
			{
				array_size = (width)*1;
			}
			else
			{
				array_size = (width)*4;
			}

			if (head->type->isStatic == 1)
			{
				printf("%s\t%d-%d\t%s\tstatic array\t[%d, %d]\t%s\n", head->scope_info->func_name, head->scope_info->scope_start_line, head->scope_info->scope_end_line, head->lexeme, head->type->start, head->type->end, terminalDict[head->type->element_type]);
			}
			else
				// need to add the ids of start and end index
				printf("%s\t%d-%d\t%s\tdynamic array\t[%s, %s]\t%s\n", head->scope_info->func_name, head->scope_info->scope_start_line, head->scope_info->scope_end_line, head->lexeme, head->type->start_dyn->node.l->ti->lexeme, head->type->end_dyn->node.l->ti->lexeme, terminalDict[head->type->element_type]);
		}
		head = head->next;
	}

	head = sc->head;
	while (head)
	{
		if (!head->is_func)
		{
			if (head->type->basic_type == ARRAY)
			{
				int width = (head->type->end - head->type->start) + 1;
				int array_size;
				if (head->type->element_type == INTEGER)
				{
					array_size = (width)*2;
				}
				else if (head->type->element_type == BOOLEAN)
				{
					array_size = (width)*1;
				}
				else
				{
					array_size = (width)*4;
				}

				if (head->type->isStatic == 1)
				{
					printf("%s\t%d-%d\t%s\tstatic array\t[%d, %d]\t%s\n", head->scope_info->func_name, head->scope_info->scope_start_line, head->scope_info->scope_end_line, head->lexeme, head->type->start, head->type->end, terminalDict[head->type->element_type]);
				}
				else
					// need to add the ids of start and end index
					printf("%s\t%d-%d\t%s\tdynamic array\t[%s, %s]\t%s\n", head->scope_info->func_name, head->scope_info->scope_start_line, head->scope_info->scope_end_line, head->lexeme, head->type->start_dyn->node.l->ti->lexeme, head->type->end_dyn->node.l->ti->lexeme, terminalDict[head->type->element_type]);
			}
		}
		head = head->next;
	}
}

void printStaticDynamicArrays(scope *sc)
{
	if (sc == NULL)
	{
		return;
	}
	printStaticDynamicArraysHelper(sc);
	scope *child = sc->left_child;
	while (child)
	{
		printStaticDynamicArrays(child);
		child = child->next;
	}
}

void activationRecordSize(scope *sc)
{
	se *temp = sc->head;
	while (temp)
	{
		printf("%s\t%d\n", temp->scope_info->func_name, temp->scope_info->activationRecordSize);
		temp = temp->next;
	}
	scope *tempScope = sc->left_child;
	while (tempScope)
	{
		if (strcmp(tempScope->stamp, "drivermodule") == 0)
		{
			printf("%s\t%d\n", tempScope->func_name, tempScope->activationRecordSize);
			break;
		}
		tempScope = tempScope->next;
	}
	return;
}
