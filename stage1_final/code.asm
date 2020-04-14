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
section .text
global main
main:
mov ebp, esp
push EAX
push EAX
PUSH EAX
PUSH EAX
mov EDX, ESP
SUB EDX, 4
push EDX
SUB ESP, 20
mov EDX, ESP
SUB EDX, 4
push EDX
SUB ESP, 20
mov EDX, ESP
SUB EDX, 4
push EDX
SUB ESP, 20
XOR ECX, ECX
FOR_LOOP_1 : PUSH ECX
PUSH intvar+0
PUSH intinputFormat
call scanf
ADD ESP,8
POP ECX
MOV EAX,dword[intvar+0]
PUSH EDX
MOV EDX,EBP
SUB EDX,ECX
MOV dword[SS:EDX- 24],EAX
POP EDX
ADD ECX, 4
CMP ECX, 20
JL FOR_LOOP_1
XOR ECX, ECX
FOR_LOOP_2 : PUSH ECX
PUSH intvar+0
PUSH intinputFormat
call scanf
ADD ESP,8
POP ECX
MOV EAX,dword[intvar+0]
PUSH EDX
MOV EDX,EBP
SUB EDX,ECX
MOV dword[SS:EDX- 48],EAX
POP EDX
ADD ECX, 4
CMP ECX, 20
JL FOR_LOOP_2
MOV ECX, 6
FOR_LOOP_3 : MOV DWORD[EBP-12], ECX
MOV EDX, dword[EBP- 12]
PUSH ECX
MOV ECX,6
CMP EDX,ECX
JL END
MOV ECX,10
CMP EDX,ECX
JG END
POP ECX
MOV ECX, [dword EBP-20]
SUB EDX, 6
SHL EDX,2
ADD EDX,4
SUB ECX,EDX
MOV EAX, [dword SS: ECX]
PUSH EAX
MOV EDX, dword[EBP- 12]
PUSH ECX
MOV ECX,6
CMP EDX,ECX
JL END
MOV ECX,10
CMP EDX,ECX
JG END
POP ECX
MOV ECX, [dword EBP-44]
SUB EDX, 6
SHL EDX,2
ADD EDX,4
SUB ECX,EDX
MOV EAX, [dword SS: ECX]
PUSH EAX
POP EBX
POP EAX
ADD EAX, EBX
PUSH EAX
POP EAX
MOV ESI,EBP
SUB ESI,dword[dynOffset]
MOV [dword SS : ESI-4-12],EAX
mov EAX,dword[EBP - 16]
push eax
push men
call printf
pop eax
pop eax
MOV EAX, [dword EBP-4-12]
PUSH EAX
MOV EDX, dword[EBP- 12]
PUSH ECX
MOV ECX,6
CMP EDX,ECX
JL END
MOV ECX,10
CMP EDX,ECX
JG END
POP ECX
POP EAX
MOV ECX, [dword EBP-68]
SUB EDX, 6
SHL EDX,2
ADD EDX,4
SUB ECX,EDX
MOV [dword SS : ECX],EAX
MOV ECX, DWORD[EBP-12]
INC ECX
CMP ECX, 10
JLE FOR_LOOP_3
XOR ECX, ECX
PUSH EDX
FOR_LOOP_4 : MOV EDX, dword[EBP-68]
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
CMP ECX, 20
JL FOR_LOOP_4
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
