extern printf
extern scanf
section .data
intinputFormat: db "%d",0
realinputFormat: db "%f",0
intvar: times 100 dd 0
realvar: times 100 dd 0
men:  db "Output: %d ", 10,0
;errorMsg : db "Array Index Out of Bounds",0
section .text
global main
main:
mov ebp, esp
push EAX
push EAX
PUSH EAX
mov EDX, ESP
SUB EDX, 4
push EDX
SUB ESP, 20
mov EDX, ESP
SUB EDX, 4
push EDX
SUB ESP, 20
MOV EAX, 7
PUSH EAX
POP EAX
MOV ECX, [dword EBP-8-12]
MOV [dword ECX-8], EAX
mov eax,1
mov ebx,0
int 80h
