#include "hash.h"

int compute_hash(char* str){

	int i;
	long long int hash = 0;
	int len = strlen(str);
	long long temp = 1;
	for(i=0;i<len;i++){

		hash+= temp*((long long int)str[i]);
		temp = temp*HASHNUM;
	}

	return  (hash%HASHSIZE);

}

void flushHash(){
	int i;
	for(i=0;i<HASHSIZE;i++){
		hash_table[i] = NULL;
	}
}

map_node* create_map_node(Symbol s, char* str,int tag){

	map_node* temp = malloc(sizeof(map_node));
	temp->s = s;
	strcpy(temp->str,str);
	temp->tag = tag;

	return temp;
}

hash_node* create_hash_node(map_node* mnode){

	hash_node* temp = malloc(sizeof(hash_node));
	temp->m = mnode;
	temp->next = NULL;

	return temp;
}

void insert_hash_node(char* str,int tag,Symbol s){


	map_node* mnode = create_map_node(s,str,tag);
	hash_node* hnode = create_hash_node(mnode);
	int hash_val = compute_hash(str);

	//printf("%d\n",hash_val);

	hash_node* temp = hash_table[hash_val];
	if(temp==NULL){
		hash_table[hash_val] = hnode;
	}else{
		hnode->next = temp;
		hash_table[hash_val] = hnode;
	}
}

map_node* search_hash(char* str){

	int hash_val = compute_hash(str);
	hash_node* temp_hnode = hash_table[hash_val];
	map_node* temp = temp_hnode->m;

	while(temp_hnode && strcmp(temp->str,str)!=0){
		temp_hnode = temp_hnode->next;
		temp = temp_hnode->m;
	}

	return temp;
}


/*int main(){

	char* str = "AB";

	flushHash();
	Symbol s;
	s.T = INTEGER;
	insert_hash_node(str,0,s);
	map_node* temp = search_hash("AB");

	printf("%s\t%d\n",temp->str,(temp->s).T);
}*/