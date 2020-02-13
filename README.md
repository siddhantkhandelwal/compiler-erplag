# compiler-erplag
ERPLAG Compiler for course CS-F363

## Compeleted Modules

  - lexer.c
    + `FILE *getStream(FILE *fp);`
    + `void removeComments(char *testCaseFile, char *cleanFile)`

## Execution

  - For testing `void removeComments(char *testCaseFile, char *cleanFile)`

    ```bash
    gcc -o lexer lexer.c
    ./lexer [testCaseFile] [cleanFile]
    ```