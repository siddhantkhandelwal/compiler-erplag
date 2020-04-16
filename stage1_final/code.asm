extern printf
extern scanf
section .data
intinputFormat: db "%d",0
realinputFormat: db "%f",0
intvar: times 100 dd 0
realvar: times 100 dd 0
men:  db "Output: %d ", 10,0
errorMsg : db "Array Index Out of Bounds",10,0
dynOffset : dd 0
start: times 100 dd 0
 end: times 100 dd 0
dyn_arrays: times 100 dd 0
section .text
global main
main:
mov ebp, esp
push EAX
push EAX
PUSH EAX
PUSH EAX
PUSH EAX
PUSH EAX
PUSH EAX
MOV EAX, 19
PUSH EAX
POP EAX
MOV [dword EBP-4-12],EAX
MOV EAX, 56
PUSH EAX
POP EAX
MOV [dword EBP-8-12],EAX
MOV ECX, 3
FOR_LOOP_1 : MOV DWORD[EBP-28], ECX
PUSH intvar+0
PUSH intinputFormat
call scanf
ADD ESP,8
MOV EAX,dword[intvar+0]
MOV dword[EBP-12],EAX
MOV ECX, [dword EBP-12]
CMP ECX, 1
JNZ CASE_1_2
MOV EAX, [dword EBP-0-12]
PUSH EAX
MOV EAX, 2
PUSH EAX
POP EBX
POP EAX
MUL EBX
PUSH EAX
MOV EAX, [dword EBP-4-12]
PUSH EAX
POP EBX
POP EAX
SUB EAX, EBX
PUSH EAX
MOV EAX, [dword EBP-16-12]
PUSH EAX
POP EBX
POP EAX
ADD EAX, EBX
PUSH EAX
POP EAX
MOV [dword EBP-4-12],EAX
mov EAX,dword[EBP - 16]
push eax
push men
call printf
pop eax
pop eax
JMP SWITCH_END1
CASE_1_2 : CMP ECX, 2
JNZ DEFAULT_1
mov EAX,dword[EBP - 12]
push eax
push men
call printf
pop eax
pop eax
mov EAX,dword[EBP - 28]
push eax
push men
call printf
pop eax
pop eax
MOV EAX, [dword EBP-0-12]
PUSH EAX
MOV EAX, 3
PUSH EAX
POP EBX
POP EAX
MUL EBX
PUSH EAX
MOV EAX, [dword EBP-8-12]
PUSH EAX
POP EBX
POP EAX
SUB EAX, EBX
PUSH EAX
MOV EAX, [dword EBP-16-12]
PUSH EAX
POP EBX
POP EAX
ADD EAX, EBX
PUSH EAX
POP EAX
MOV [dword EBP-8-12],EAX
mov EAX,dword[EBP - 20]
push eax
push men
call printf
pop eax
pop eax
JMP SWITCH_END1
DEFAULT_1 : 
mov EAX,dword[EBP - 12]
push eax
push men
call printf
pop eax
pop eax
SWITCH_END1 : 
mov EAX,dword[EBP - 16]
push eax
push men
call printf
pop eax
pop eax
mov EAX,dword[EBP - 20]
push eax
push men
call printf
pop eax
pop eax
MOV ECX, DWORD[EBP-28]
INC ECX
CMP ECX, 6
JLE FOR_LOOP_1
mov eax,1
mov ebx,0
int 80h
END : 
PUSH errorMsg
call printf
mov eax,1
mov ebx,0
int 80h
