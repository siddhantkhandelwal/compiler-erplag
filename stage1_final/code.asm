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
compute_expr2:
SUB ESP, 8
MOV EDX, ESP
SUB ESP, 4
PUSH EDX
MOV EAX, 15
PUSH EAX
POP EAX
MOV [dword EBP-4-12],EAX
POP EDX
mov EAX,dword[EBP - 12]
PUSH EDX
push eax
push men
call printf
pop eax
pop eax
POP EDX
MOV ESP, EBP
SUB ESP, 8
POP EBP
ret
compute_expr:
SUB ESP, 16
MOV EDX, ESP
SUB ESP, 4
PUSH EDX
MOV ECX, [dword EBP-8-12]
MOV EAX, [dword ECX-8]
PUSH EAX
POP EAX
MOV [dword EBP-12-12],EAX
POP EDX
PUSH EDX
MOV EAX, [dword EBP-0-12]
PUSH EAX
POP EAX
MOV ECX, [dword EBP-8-12]
MOV [dword ECX-8], EAX
POP EDX
XOR ECX, ECX
PUSH EDX
FOR_LOOP_1 : MOV EDX, dword[EBP-20]
SUB EDX,ECX
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
CMP ECX, 16
JL FOR_LOOP_1
POP EDX
mov EAX,dword[EBP - 16]
PUSH EDX
push eax
push men
call printf
pop eax
pop eax
POP EDX
PUSH EDX
MOV [dword ESP-8], EBP
MOV EDX, [dword EBP-12-12]
MOV [dword ESP-12-0], EDX
MOV EBP, ESP
call compute_expr2
SUB EDX, 4
MOV ECX, [dword EDX]
MOV [dword EBP-12-12], ECX
POP EDX
MOV ESP, EBP
SUB ESP, 8
POP EBP
ret
main:
mov ebp, esp
push EAX
push EAX
mov EDX, ESP
SUB EDX, 4
push EDX
SUB ESP, 16
PUSH EAX
PUSH EAX
PUSH EAX
XOR ECX, ECX
FOR_LOOP_2 : PUSH EDX
PUSH ECX
PUSH intvar+0
PUSH intinputFormat
call scanf
ADD ESP,8
POP ECX
POP EDX
MOV EAX,dword[intvar+0]
PUSH EDX
MOV EDX,EBP
SUB EDX,ECX
MOV dword[SS:EDX- 16],EAX
POP EDX
ADD ECX, 4
CMP ECX, 16
JL FOR_LOOP_2
PUSH EDX
MOV EAX, 8
PUSH EAX
POP EAX
MOV [dword EBP-24-12],EAX
POP EDX
PUSH EDX
MOV EAX, 9
PUSH EAX
POP EAX
MOV [dword EBP-28-12],EAX
POP EDX
PUSH EDX
MOV [dword ESP-8], EBP
MOV EDX, [dword EBP-12-24]
MOV [dword ESP-12-0], EDX
MOV EDX, [dword EBP-12-28]
MOV [dword ESP-12-4], EDX
MOV EDX, [dword EBP-12-0]
MOV [dword ESP-12-8], EDX
MOV EBP, ESP
call compute_expr
SUB EDX, 4
MOV ECX, [dword EDX]
MOV [dword EBP-20-12], ECX
POP EDX
mov EAX,dword[EBP - 32]
PUSH EDX
push eax
push men
call printf
pop eax
pop eax
POP EDX
mov eax,1
mov ebx,0
int 80h
END : 
PUSH errorMsg
call printf
mov eax,1
mov ebx,0
int 80h
