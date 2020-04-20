/* LAVANYA SONI - 2017A7PS0151P
    KUSHAGRA AGRAWAL - 2017A7PS0107P
    SIDDHANT KHANDELWAL - 2017A7PS0127P
    GROUP - 11
*/

#include "hash.h"
//#include "tree.h"
#include "stack.h"

extern char *terminalDict[];
extern char *nonTerminalDict[];
extern int syntacticError;

void parseInput(FILE **fp);
void populateParseTable();
long long unsigned calculate_follow(int nterm);
long long unsigned calculate_first(int nterm);
void ComputeFirstAndFollowSets();
void populateGrammar(FILE *fp);
void populateHashTable();
void populateGrammar(FILE *fp);
void initiliazeParseTable();
void flush_grammar();