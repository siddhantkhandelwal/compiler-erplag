extern printf
extern scanf
section .data
intinputFormat: db "%d",0
realinputFormat: db "%f",0
intvar: times 100 dd 0
realvar: times 100 dd 0
men:  db "Output: %d ", 10,0
section .text
global main
main:
mov ebp, esp
push EAX
push EAX
mov EDX, ESP
SUB EDX, 4
push EDX
SUB ESP, 8
XOR ECX, ECX
FOR_LOOP_1 : 

PUSH EDX
PUSH ECX
PUSH intvar+0
PUSH intinputFormat
call scanf
POP ECX
POP ECX
POP ECX
POP EDX
MOV EAX,dword[intvar+0]
PUSH EDX
MOV EDX,EBP
SUB EDX,ECX
MOV dword[SS:EDX- 16],EAX
POP EDX
ADD ECX, 4
CMP ECX, 8
JL FOR_LOOP_1
XOR ECX, ECX
PUSH EDX
MOV EDX, dword[EBP-12]
FOR_LOOP_2 : SUB EDX,ECX
mov EAX,dword[SS:EDX-4]
push edx
push ecx
push eax
push men
call printf
pop eax
pop eax
pop ecx
pop edx
ADD ECX, 4
CMP ECX, 8
JL FOR_LOOP_2
POP EDX
mov eax,1
mov ebx,0
int 80h
