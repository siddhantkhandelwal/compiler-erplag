#include "hash.h"
char *terminalDict[] = {

    "INTEGER",
    "REAL",
    "BOOLEAN",
    "OF",
    "ARRAY",
    "START",
    "END",
    "DECLARE",
    "MODULE",
    "DRIVER",
    "PROGRAM",
    "GET_VALUE",
    "PRINT",
    "USE",
    "WITH",
    "PARAMETERS",
    "TRUE",
    "FALSE",
    "TAKES",
    "INPUT",
    "RETURNS",
    "AND",
    "OR",
    "FOR",
    "IN",
    "SWITCH",
    "CASE",
    "BREAK",
    "DEFAULT",
    "WHILE",
    "ASSIGNOP",
    "BC",
    "BO",
    "COLON",
    "COMMA",
    "COMMENT",
    "DIV",
    "DRIVERDEF",
    "DRIVERENDDEF",
    "ENDDEF",
    "DEF",
    "EQ",
    "GE",
    "GT",
    "ID",
    "LE",
    "LT",
    "MINUS",
    "MUL",
    "NE",
    "NUM",
    "PLUS",
    "RANGEOP",
    "SEMICOL",
    "SQBC",
    "SQBO",
    "RNUM",
    "E"
};


char* nonTerminalDict[] = {

    "program",
    "moduledeclarations",
    "othermodules",
    "drivermodule",
    "moduledeclaration",
    "module",
    "moduledef",
    "inputplist",
    "ret",
    "inputplistnew",
    "datatype",
    "outputplist",
    "type",
    "outputplistnew",
    "statements",
    "statement",
    "iostmt",
    "simplestmt",
    "declarestmt",
    "conditionalstmt",
    "iterativestmt",
    "var",
    "varidnum",
    "boolconstt",
    "whichid",
    "index",
    "assignmentstmt",
    "modulereusestmt",
    "whichstmt",
    "lvalueidstmt",
    "lvaluearrstmt",
    "expression",
    "optional",
    "idlist",
    "idlistnew",
    "arithmeticorbooleanexpression",
    "u",
    "unaryop",
    "newnt",
    "arithmeticexpr",
    "anyterm",
    "n7",
    "logicalop",
    "n8",
    "relationalop",
    "op1",
    "term",
    "n4",
    "op2",
    "factor",
    "n5",
    "casestmts",
    "default",
    "value",
    "n9",
    "range",
    "rangearrays"

};  

rhsNode* create_rhsNode(map_node* mnode){
    rhsNode* temp = malloc(sizeof(rhsNode));
    temp->S = mnode->s;
    temp->tag = mnode->tag;
    temp->next = NULL;
    return temp; 
}

rule_header* create_rule_header(){
   
   rule_header* temp = malloc(sizeof(rule_header));
    temp->curr_rule = NULL;
    temp->next_rule = NULL;
    return temp;
}

void insert_rhsNode(rule_header* rule,rhsNode* rnode){

    rhsNode* curr = rule->curr_rule;

    if(curr==NULL){
        rule->curr_rule = rnode;
        return;
    }

    while(curr->next){
        curr = curr->next;
    }

    curr->next = rnode;
    return;

}

rule_header* new_rule_header(rule_header* first){

    if(!first){
        return create_rule_header();
    }

    else{
        rule_header* curr = first;
        while(first->next_rule){
            first=first->next_rule;
        }

        first->next_rule = create_rule_header();
        return first->next_rule;
    }
}


void flush_grammar(){
    int i;
    for(i=0;i<NOOFRULES;i++)
        g[i] = NULL;
}


void populateGrammar(FILE *fp)
{
    if (fp == NULL)
    {
        printf("Unable to open file\n");
        exit(-1);
    }

    char line_read[MAXLENGTHTNT];
    char delim[2] = " ";
    int i=1;
    while(fgets(line_read,MAXLENGTHTNT,fp)){
        
        line_read[strlen(line_read)-1] = 0;

        char* non_term = strtok(line_read,delim);
        map_node* temp = search_hash(non_term);
        rule_header* this_rule;
        int index;
        if(temp->tag==0)
            index = (temp->s).T;
        else
            index = (temp->s).N;

        this_rule = g[index];
        if(!this_rule){
            this_rule = (g[index]=new_rule_header(this_rule));
        }else
            this_rule = new_rule_header(this_rule);

        non_term = strtok(NULL,delim);
        while(non_term){
            temp = search_hash(non_term);
            rhsNode* new_node = create_rhsNode(temp);
            insert_rhsNode(this_rule,new_node);
            non_term = strtok(NULL,delim);
        }

    }
}

void populateHashTable(){

    int i;

    for(i=0;i<TERMINALS;i++){
        Symbol s;
        s.T = i;
        insert_hash_node(terminalDict[i],0,s);
    }

    for(i=0;i<NTERMINALS;i++){
        Symbol s;
        s.T = i;
        insert_hash_node(nonTerminalDict[i],1,s);
    }
}

//int l = 1;

void print_rule(rule_header* rh){
    
    rhsNode* temp = rh->curr_rule;
    while(temp){
        if(temp->tag==0)
            printf("%s\t",terminalDict[(temp->S).T]);
        else
            printf("%s\t",nonTerminalDict[(temp->S).N]);

        temp=temp->next;
    }

    printf("\n");
}



void printgrammar(){

    int i;

    for(i=0;i<NOOFRULES;i++){
        //printf("%d\t",i+1);
        rule_header* temp = g[i];
        if(temp==NULL)
            return;
        while(temp){
            printf("%d\t",l++);
            printf("%s\t",nonTerminalDict[i]);
            print_rule(temp);
            temp = temp->next_rule;
        }
    }
}


int main()
{
   // FILE *fp = fopen("grammar", "r");
   // Grammar *grammar = populateGrammar(fp);

    populateHashTable();
    /*int i;
    map_node* temp;
    for(i=0;i<(TERMINALS);i++){
        temp = search_hash(terminalDict[i]);
        printf("%s\t%d\t%d\n",temp->str,(temp->s).T,temp->tag);
   }

    for(i=0;i<(NTERMINALS);i++){
        temp = search_hash(nonTerminalDict[i]);
        printf("%s\t%d\t%d\n",temp->str,(temp->s).T,temp->tag);
    }*/

    FILE* fp = fopen("grammar","r");
    populateGrammar(fp);


    printgrammar();



    return 0;
}