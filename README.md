# compiler-erplag
ERPLAG Compiler for course CS-F363

## Execution

``` bash
make
./compiler [source code] code.asm
nasm -f elf32 code.asm
gcc -m32 code.o -o code
./code
```
