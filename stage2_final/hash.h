/* LAVANYA SONI - 2017A7PS0151P
    KUSHAGRA AGRAWAL - 2017A7PS0107P
    SIDDHANT KHANDELWAL - 2017A7PS0127P
    GROUP - 11
*/

#ifndef hasheader
#define hashHeader

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define HASHNUM 2
#define HASHSIZE 200
#include "parserDef.h"

hash_node *hash_table[HASHSIZE];

int compute_hash(char *str);

void flushHash();

map_node *create_map_node(Symbol s, char *str, int tag);

hash_node *create_hash_node(map_node *mnode);

void insert_hash_node(char *str, int tag, Symbol s);

map_node *search_hash(char *str);

#endif