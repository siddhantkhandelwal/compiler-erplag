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
    "E",
    "$"
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
            //printf("%d\t",l++);
            printf("%s\t",nonTerminalDict[i]);
            print_rule(temp);
            temp = temp->next_rule;
        }
    }
}


long long unsigned calculate_first(int nterm){

    if(First[nterm])
        return First[nterm];

    rule_header* rule = g[nterm];
    rhsNode* temp = rule->curr_rule;
    

    int has_epsilon = 0;

    while(rule){


        if(temp==NULL){
            rule = rule->next_rule;
            continue;
        }


        if(temp->tag==0){
            long long unsigned mask = 1;
            First[nterm] = First[nterm]|(mask<<((temp->S).T));
            rule = rule->next_rule;

            if(rule)
                temp = rule->curr_rule;

            if((temp->S).T==EPSILON)
                has_epsilon = 1;

            continue;
        }else{

            long long unsigned mask = 1;
            int index = (temp->S).N;

            long long unsigned flag = 0;

            //printf("first of %s\n",nonTerminalDict[index]);

            long long unsigned t = calculate_first(index);


           // printf("First Index = %llu  Mask  = %llu\n",First[index],mask<<EPSILON);
            flag = First[index]&(mask<<(EPSILON));
            
                
                if(flag){

                    //printf("Inside flag\n");
                    mask =1;
                    mask = mask<<EPSILON;
                    t = t^mask;
                    First[nterm] = First[nterm]|t;
                    temp = temp->next;

                    if(temp==NULL){
                        has_epsilon = 1;
                        //rule = rule->next_rule;
                    }
                    continue;
                }else{
                    First[nterm] = First[nterm]|First[index];
                    rule = rule->next_rule;

                    if(rule)
                        temp = rule->curr_rule;

                }

        }


    }

    if(has_epsilon){
        long long unsigned mask = 1;
        mask = mask<<(EPSILON);
        First[nterm] = First[nterm]|mask;
    }


    return First[nterm];

}

void print_terminal(long long unsigned num){

    int i;
    unsigned int temp;
    unsigned int mask = 1;
    for(i=0;i<TERMINALS;i++){
        temp = num&mask;
        if(temp)
            printf("%s\t",terminalDict[i]);

        num = num>>1;
    }

    printf("\n");
}

long long unsigned calculate_follow(int nterm){

    if(Follow[nterm]!=0)
        return Follow[nterm];

    rule_header* rule = NULL;
    rhsNode* temp = NULL;
    int i;

    for(i=0;i<NTERMINALS;i++){

        rule = g[i];

        while(rule){
            temp = rule->curr_rule;

            while(temp){

                rhsNode* find_first = NULL;

                if(temp->tag==1 && (temp->S).N == nterm){
                    
                        //printf("Found at %d \n",i);

                        find_first = temp->next;

                        if(find_first==NULL){

                            if((temp->S).N != i){
                                long long unsigned curr_follow = calculate_follow(i);
                                Follow[nterm] = Follow[nterm]|curr_follow;
                            }
                        }else{
                            int index;
                            long long unsigned mask = 1;
                            if((find_first->tag)==0){

                                index = (find_first->S).T;
                                //printf("Fount terminal %s\n",terminalDict[index]);
                                Follow[nterm] = (Follow[nterm]|(mask<<index));
                                //printf("%llu\n",Follow[nterm]);

                            }else{

                                index = (find_first->S).N;
                            

                                long long unsigned flag = 0;
                                long long unsigned next_first = First[index];
                                flag = First[index]&(mask<<(EPSILON));

                                mask = mask<<EPSILON;

                                while(find_first && flag){

                                    
                                    next_first = next_first^mask;
                                    Follow[nterm] = Follow[nterm]|next_first;

                                    find_first = find_first->next;

                                    if(find_first){

                                        if(find_first->tag==1){
                                            index = (find_first->S).N;
                                            flag = First[index]&(mask<<(EPSILON));
                                            next_first = First[index];
                                        }else{

                                            index = (find_first->S).T;
                                            mask = 1;
                                            Follow[nterm] = Follow[nterm]|(mask<<index);
                                            break;
                                        }

                                    }

                                }

                                if(find_first==NULL){
                                    if((temp->S).N != i)
                                        Follow[nterm] = Follow[nterm]|calculate_follow(i);
                                }


                                if(!flag && find_first && find_first->tag==1){
                                    Follow[nterm] = Follow[nterm]|next_first;
                                }

                                
                            }
                        }
                }
                    //printf("%llu\n",Follow[nterm]);
                    temp = temp->next;
            }

            rule = rule->next_rule;
        }
    }

        return Follow[nterm];

}

void ComputeFirstAndFollowSets () {

    for(int i = 0;i<NTERMINALS;i++){
        First[i] = 0;
        Follow[i] = 0;
    }
    for(int i = 0;i<NTERMINALS;i++){
        calculate_first(i);
    }
    long long unsigned mask = 1;
    mask = mask << DOLLAR;
    Follow[0] = Follow[0] | mask;
    for(int i = 0;i<NTERMINALS;i++){
        calculate_follow(i);
    }

    for(int i=0;i<NTERMINALS;i++){
        printf("%d <%s>\t",i,nonTerminalDict[i]);
        long long unsigned t = Follow[i];
        print_terminal(t);
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


   // printgrammar();
    /*long long unsigned t;*/

    ComputeFirstAndFollowSets();

   


    return 0;
}