#include "parser.h"


//typedef struct symbol_entry se;
//typedef struct scope scope;
//typedef struct type type_info;

struct type_info{

	Terminal basic_type;
	Terminal element_type;
	int start;
	int end;
	int isStatic;
};

struct symbol_entry{

	char lexeme[25];
	int func_use;  
	type_info* type;
	se* next;
	scope* scope_info;
	int is_func;
	int is_array;
	int is_used;
	int offset;
	int width;
	int num_used; // Number of times this variable is used;
	int used_on_lines[50];
};

struct scope{

	char stamp[25];
	scope* parent;
	scope* left_child;
	scope* right_child;
	scope* next;
	scope* prev;
	se* input_list;
	se* output_list;
	se* head;
};

scope* make_st(tNode* head);
void printSymbolTable(scope* sc);