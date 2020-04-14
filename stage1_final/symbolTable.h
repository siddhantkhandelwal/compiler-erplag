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
	tNode* start_dyn;
	tNode* end_dyn;
};

struct symbol_entry{

	char lexeme[25];
	int func_use;  
	type_info* type;
	se* next;
	scope* scope_info;
	int is_func;
	int is_array;
	int is_control_variable;
	int is_control_changed;
	int offset;
	int width;
	int num_used; // Number of times this variable is used;
	int used_on_lines[50];
	int error_flag;
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
	int is_func_used;
	int label_ip;
	int label_op;
};

scope* make_st(tNode* head);
void printSymbolTable(scope* sc);