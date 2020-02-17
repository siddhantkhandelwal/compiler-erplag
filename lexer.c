#include "lexer.h"


#define MAXLEN 20
#define NUM_TOKENS 30
static int line = 1;

int buffer_ptr = -1;

char buffer[BUFFLEN];

unsigned int ended = 0;

FILE *getStream(FILE *fp)
{
    for (int i = 0; i < BUFFLEN; i++)
    {
        buffer[i] = '\0';
    }
    if (fp == NULL)
    {
        printf("Unable to open file\n");
        exit(-1);
    }
    if (fread(buffer, (size_t)BUFFLEN, 1, fp))
    {
        buffer_ptr = 0;
        return fp;
    }
    //printf("%s\n", buffer);
    return NULL;
}

FILE* reloadBuffer(FILE* fp){


    fp = getStream(fp);
    return fp;
}

void removeComments(char *testCaseFile, char *cleanFile)
{
    FILE *fi = fopen(testCaseFile, "r+");
    if (fi == NULL)
    {
        printf("Unable to open input file");
        exit(-1);
    }
    FILE *fo = fopen(cleanFile, "w+");
    if (fi == NULL)
    {
        printf("Unable to open output file");
        exit(-1);
    }
    while (!feof(fi))
    {
        char c1 = fgetc(fi);
        char c2;
        if (c1 == '*')
        {
            c2 = fgetc(fi);
            if (c2 == '*')
            {
                while ((c1 = fgetc(fi)) != '*' || (c2 = fgetc(fi)) != '*')
                {
                    if (c1 == '\n')
                    {
                        fputc(c1, fo);
                    }
                    continue;
                }
            }
            else
            {
                fputc(c1, fo);
                fputc(c2, fo);
            }
        }
        else
        {
            if (c1 == EOF)
            {
                break;
            }
            fputc(c1, fo);
        }
    }
    fclose(fi);
    fclose(fo);
}


// node *createNode()
// {
//     node *thisNode = (node *)malloc(sizeof(node));
//     thisNode->tk = (tokenInfo *)malloc(sizeof(tokenInfo));
//     return thisNode;
// }

// header initializeHead()
// {
//     header newHeader = (header)malloc(sizeof(header));
//     newHeader->headLink = NULL;
//     return newHeader;
// }

int keywordChecker(char val[])
{
    for (int i = 0; i < NUM_TOKENS; i++)
    {
        if (strcmp(val, keywordDict[i]) == 0)
        {
            return i;
        }
    }
    
    return -1;
}

int removeCommentsSource(char *buffer, int pointer)
{
    while (!((buffer[pointer] == '*') && (buffer[pointer + 1] == '*')))
    {
        pointer++;
        if (buffer[pointer] == '\n')
        {
            line++;
        }
    }
    return pointer;
}

void flush(char *str, int elementsToFlush)
{
    for (int i = 0; i < elementsToFlush; i++)
    {
        str[i] = '\0';
    }
}

void check_buffer(FILE** fp){

    if(buffer_ptr>=BUFFLEN){
        printf("%d\n",buffer_ptr);
        *fp = reloadBuffer(*fp);
        buffer_ptr = 0;

    }
}

tokenInfo* createToken(Token t,char* lexeme,unsigned int line){

    tokenInfo* token = malloc(sizeof(tokenInfo));

    if(t==NUM){
        int temp = atoi(lexeme);
        (token->value).v1 = temp;
        token->tag = 0;
    }
    if(t==RNUM){
        double temp = atof(lexeme);
        (token->value).v2 = temp;
        token->tag = 1;
    }

    token->t = t;
    strcpy(token->lexeme,lexeme);
    token->line = line;

    return token;
}

tokenInfo* getNextToken(FILE** fp){

    char curr_char;
    int len = 0;
    char lexeme_read[30];

    int i;
    for(i=0;i<MAXLEN;i++)
        lexeme_read[i] = '\0';

    unsigned int state = 0;

    while(1){

        buffer_ptr++;
        check_buffer(fp);
        curr_char = buffer[buffer_ptr];
        
        tokenInfo* token;

        switch(state){

            case 0:


                    if(curr_char=='\0'){
                        ended = 1;
                        return NULL;
                    }

                    if(curr_char==' '||curr_char=='\t'){
                        

                        break;
                    }

                    if(curr_char=='\n'){
                        line++;
                        break;
                    }

                    if((curr_char>='a'&&curr_char<='z')||(curr_char>='A'&&curr_char<='Z')){
                       
                        lexeme_read[len] = curr_char;
                        len++;
                        //buffer_ptr++;
                        check_buffer(fp);
                        state = 1;
                        break;
                    }


                    if(curr_char>='0'&&curr_char<='9'){
                        lexeme_read[len] = curr_char;
                        len++;
                        //buffer_ptr++;
                        check_buffer(fp);
                        state = 2;
                        break;
                    }

                    if(curr_char == '*')
                    {
                        lexeme_read[len] = curr_char;
                        len++;
                        //buffer_ptr++;
                        check_buffer(fp);
                        state = 6;
                        break;
                    }
                    

            case 1:



                curr_char = buffer[buffer_ptr];  // Check this assignment...Redundant!

                while((curr_char>='a'&&curr_char<='z')||(curr_char>='A'&&curr_char<='Z')||(curr_char>='0'&&curr_char<='9')||curr_char=='_'){
                       
                       if(len<=MAXLEN)
                        lexeme_read[len] = curr_char;
                        len++;
                        buffer_ptr++;
                        check_buffer(fp);
                        
                        curr_char = buffer[buffer_ptr];

                    }

                    buffer_ptr--;

                    if(len>MAXLEN){
                        printf("Lexical Error...Identifier Size too long\n");
                        return NULL;
                    }

                    int index;

                    if((index = keywordChecker(lexeme_read))!=-1){
                        printf("Hello:%d\n",index );
                         token = createToken(index,lexeme_read,line);
                       
                    }else
                        token = createToken(ID,lexeme_read,line);
                    
                    printf("%u\n",token->t);
                    return token;
            
            

            case 2: 

                  //Maximum Length of a numeric token can be 30.

                    while(curr_char>='0'&&curr_char<='9'){
                        lexeme_read[len] = buffer[buffer_ptr];
                        buffer_ptr++;
                        check_buffer(fp);
                        len++;

                        curr_char = buffer[buffer_ptr];

                    }
                    if(curr_char == '.')
                    {

                        lexeme_read[len] = buffer[buffer_ptr];
                        len++;
                        state = 3;
                        break;
                    }
                    buffer_ptr--;


                    token = createToken(NUM,lexeme_read,line);
                    //printf("%s\n",token->lexeme);
                    printf("%d\n", token->t);
                    //printf("%s\n", );
                    printf("%s\n",lexeme_read);
                    return token;

            case 3: 
                    if(curr_char == '.')
                    {
                        lexeme_read[len - 1] = '\0';
                        len--;
                        token = createToken(NUM, lexeme_read, line);
                        printf("%u\n", token->t);
                        buffer_ptr -= 2; //Handle edge case where buffer ends after reading first dot.
                        return token;
                    }
                    if(curr_char < '0' || curr_char > '9')
                    {
                        printf("Lexical  error at %d\n", line);
                        buffer_ptr--;
                        return NULL;
                    }
                    while(curr_char>='0'&&curr_char<='9'){
                        lexeme_read[len] = buffer[buffer_ptr];
                        buffer_ptr++;
                        check_buffer(fp);
                        len++;
                        curr_char = buffer[buffer_ptr];
                    }
                    if(curr_char == 'e' || curr_char == 'E')
                    {
                        lexeme_read[len] = buffer[buffer_ptr];
                        state = 4;
                        len++;
                        break;
                    }
                    buffer_ptr--;
                    token = createToken(RNUM, lexeme_read, line);
                    //printf("%s\n", token->lexeme);
                    printf("%u\n", token->t);
                    return token;

            case 4:

                    if(curr_char == '+' || curr_char == '-')
                    {
                        lexeme_read[len] = buffer[buffer_ptr];
                        len++;
                        state = 5;
                        break;
                    }

                    if(curr_char < '0' || curr_char > '9')
                    {
                        lexeme_read[--len] = '\0';
                        token = createToken(RNUM, lexeme_read, line);
                        printf("%u\n", token->t);
                        buffer_ptr -= 2;
                        return token;
                    }
                    while(curr_char>='0'&&curr_char<='9'){
                        lexeme_read[len] = buffer[buffer_ptr];
                        buffer_ptr++;
                        check_buffer(fp);
                        len++;
                        curr_char = buffer[buffer_ptr];
                    }
                    buffer_ptr--;
                    token = createToken(RNUM, lexeme_read, line);
                    printf("%s\n", token->lexeme);
                    printf("%u\n", token->t);
                    return token;

            case 5:
                    if(curr_char < '0' || curr_char > '9')
                    {
                        lexeme_read[len-2] = '\0';
                        //lexeme_read[len-1] = '\0';
                        len -= 2;
                        token = createToken(RNUM, lexeme_read, line);
                        printf("%u\n", token->t);
                        buffer_ptr -= 3;
                        return token;
                    }
                    while(curr_char>='0'&&curr_char<='9'){
                        lexeme_read[len] = buffer[buffer_ptr];
                        buffer_ptr++;
                        check_buffer(fp);
                        len++;
                        curr_char = buffer[buffer_ptr];
                    }
                    buffer_ptr--;
                    token = createToken(RNUM, lexeme_read, line);
                    printf("%u\n", token->t);
                    printf("%s\n", token->lexeme);
                    return token;

            case 6:
                if(curr_char == '*')
                {
                    lexeme_read[--len] = '\0';
                    state = 7;
                    break;
                }
                token = createToken(MUL, lexeme_read, line);
                printf("%u\n", token->t);
                printf("%s\n", token->lexeme);
                buffer_ptr--;
                return token;


            case 7: 
                while(curr_char != '*' && curr_char != '\n')
                {
                    buffer_ptr++;
                    check_buffer(fp);
                    curr_char = buffer[buffer_ptr];
                }
                if(curr_char == '*')
                {
                    state = 8;
                    break;
                }
                else if(curr_char == '\n')
                {
                    state = 9;
                    break;
                }

            case 8:
                if(curr_char == '*')
                {
                    state = 0;
                   // len = 0;
                    break;
                }
                else if(curr_char == '\n')
                {
                    state = 9;
                    break;
                }
                else
                {
                    state = 7;
                    break;
                }
            case 9:
                line++;
                if(curr_char != '\n')
                {
                    state = 7;
                    break;
                }
                else
                {
                    state = 9;
                    break;
                }
       }         
    }
}



int main(int argc, char *argv[])
{

    FILE* fp1 = fopen("test","r");
    fp1 = getStream(fp1);
    FILE** fp = &fp1;

    //tokenI getNextToken(fp);
    //printf("%s\n",buffer);

    while(ended==0){
        getNextToken(fp);}
}
