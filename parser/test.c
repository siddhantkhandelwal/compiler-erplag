#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

	FILE* fp = fopen("grammar","r");

	char line_read[100];
	int i = 1;
	fgets(line_read,100,fp);
	//line_read[strlen(line_read)-1] = 0;
	char delim[2] = " ";

	char* token = strtok(line_read,delim);

	while(token && (token[strlen(token)-1]!='\n')){
		printf("%c\n",token);
		token = strtok(NULL,delim);
	}

}