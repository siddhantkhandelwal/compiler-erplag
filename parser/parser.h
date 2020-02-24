
#include "hash.h"
//#include "tree.h"
#include "stack.h"

extern char *terminalDict[];
extern char *nonTerminalDict[];

void parseInput(FILE** fp);
void populateParseTable();
long long unsigned calculate_follow(int nterm);
long long unsigned calculate_first(int nterm);
void ComputeFirstAndFollowSets () ;
void populateGrammar(FILE *fp);
void populateHashTable();
void populateGrammar(FILE *fp);
void initiliazeParseTable();
void flush_grammar();