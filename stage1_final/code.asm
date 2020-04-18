extern printf
extern scanf
extern exit
section .data
intinputFormat: db "%d",0
realinputFormat: db "%f",0
intvar: times 100 dd 0
realvar: times 100 dd 0
men:  db "Output: %d ", 10,0
errorMsg : db "Array Index Out of Bounds",10,0
dynOffset : dd 0
errorMsg2: db "Invalid array range for dynamic array",10,0
men_bool: db "Output: %s ",0
int_msg: db "Enter an Integer value : ",10,0 
true_msg: db "True",10,0 
false_msg: db "False",10,0 
true_msg_arr: db "True ",0 
false_msg_arr: db "False ",0 
arr_msg: db "Enter %d array elements :",10,0
array_output: db "Printing Array Elements: ",10,0
array_element: db "%d ",0
array_end: db 10,0
start: times 100 dd 0
 end: times 100 dd 0
dyn_arrays: times 100 dd 0
section .text
global main
compute_expr:
SUB ESP, 16
MOV EDX, ESP
SUB ESP, 8
PUSH EAX
PUSH EAX
PUSH EAX
PUSH EAX
PUSH EDX
PUSH int_msg
call printf
ADD ESP,4
POP EDX
PUSH EDX
PUSH intvar+0
PUSH intinputFormat
call scanf
ADD ESP,8
POP EDX
MOV EAX,dword[intvar+0]
MOV dword[EBP-40],EAX
PUSH EDX
PUSH int_msg
call printf
ADD ESP,4
POP EDX
PUSH EDX
PUSH intvar+0
PUSH intinputFormat
call scanf
ADD ESP,8
POP EDX
MOV EAX,dword[intvar+0]
MOV dword[EBP-36],EAX
PUSH EDX
MOV EAX, [dword EBP-0-12]
PUSH EAX
MOV EAX, [dword EBP-4-12]
PUSH EAX
POP EBX
POP EAX
MUL EBX
PUSH EAX
MOV EAX, [dword EBP-28-12]
PUSH EAX
MOV EAX, 2
PUSH EAX
POP EBX
POP EAX
MUL EBX
PUSH EAX
MOV EAX, 3
PUSH EAX
POP EBX
POP EAX
SUB EAX, EBX
PUSH EAX
POP EBX
POP EAX
ADD EAX, EBX
PUSH EAX
POP EAX
MOV [dword EBP-20-12],EAX
POP EDX
PUSH EDX
MOV EAX, [dword EBP-8-12]
PUSH EAX
MOV EAX, [dword EBP-0-12]
PUSH EAX
MOV EAX, [dword EBP-4-12]
PUSH EAX
POP EBX
POP EAX
CMP EAX, EBX
JLE TRUE0
MOV EAX, 0
JMP FALSE0
TRUE0: MOV EAX,1
FALSE0:
PUSH EAX
POP EBX
POP EAX
AND EAX, EBX
PUSH EAX
POP EAX
MOV [dword EBP-32-12],EAX
POP EDX
PUSH EDX
MOV EAX, [dword EBP-20-12]
PUSH EAX
MOV EAX, [dword EBP-24-12]
PUSH EAX
POP EBX
POP EAX
ADD EAX, EBX
PUSH EAX
POP EAX
MOV [dword EBP-12-12],EAX
POP EDX
PUSH EDX
MOV EAX, 1
PUSH EAX
POP EAX
MOV [dword EBP-16-12],EAX
POP EDX
mov EAX,dword[EBP - 12]
PUSH EDX
push eax
push men
call printf
pop eax
pop eax
POP EDX
mov EAX,dword[EBP - 16]
PUSH EDX
push eax
push men
call printf
pop eax
pop eax
POP EDX
mov EAX,dword[EBP - 20]
PUSH EDX
PUSH EDX
CMP EAX,1
JNZ PRINT_FALSE_1
PUSH true_msg
PUSH men_bool
call printf
JMP PRINT_FALSE_END_1
PRINT_FALSE_1 : 
PUSH false_msg
PUSH men_bool
call printf
PRINT_FALSE_END_1 : 
ADD ESP,8
POP EDX
mov EAX,dword[EBP - 40]
PUSH EDX
push eax
push men
call printf
pop eax
pop eax
POP EDX
mov EAX,dword[EBP - 32]
PUSH EDX
push eax
push men
call printf
pop eax
pop eax
POP EDX
mov EAX,dword[EBP - 36]
PUSH EDX
push eax
push men
call printf
pop eax
pop eax
POP EDX
mov EAX,dword[EBP - 24]
PUSH EDX
push eax
push men
call printf
pop eax
pop eax
POP EDX
mov EAX,dword[EBP - 28]
PUSH EDX
PUSH EDX
CMP EAX,1
JNZ PRINT_FALSE_2
PUSH true_msg
PUSH men_bool
call printf
JMP PRINT_FALSE_END_2
PRINT_FALSE_2 : 
PUSH false_msg
PUSH men_bool
call printf
PRINT_FALSE_END_2 : 
ADD ESP,8
POP EDX
MOV ESP, EBP
SUB ESP, 8
POP EBP
ret
main:
mov ebp, esp
push EAX
push EAX
PUSH EAX
PUSH EAX
PUSH EAX
PUSH EDX
PUSH int_msg
call printf
ADD ESP,4
POP EDX
PUSH EDX
PUSH intvar+0
PUSH intinputFormat
call scanf
ADD ESP,8
POP EDX
MOV EAX,dword[intvar+0]
MOV dword[EBP-12],EAX
PUSH EDX
PUSH int_msg
call printf
ADD ESP,4
POP EDX
PUSH EDX
PUSH intvar+0
PUSH intinputFormat
call scanf
ADD ESP,8
POP EDX
MOV EAX,dword[intvar+0]
MOV dword[EBP-16],EAX
PUSH EAX
PUSH EAX
PUSH EDX
MOV EAX, 0
PUSH EAX
POP EAX
MOV [dword EBP-16-12],EAX
POP EDX
PUSH EDX
MOV [dword ESP-8], EBP
MOV EDX, [dword EBP-12-0]
MOV [dword ESP-12-0], EDX
MOV EDX, [dword EBP-12-4]
MOV [dword ESP-12-4], EDX
MOV EDX, [dword EBP-12-16]
MOV [dword ESP-12-8], EDX
MOV EBP, ESP
call compute_expr
SUB EDX, 4
MOV ECX, [dword EDX]
MOV [dword EBP-8-12], ECX
SUB EDX, 4
MOV ECX, [dword EDX]
MOV [dword EBP-12-12], ECX
POP EDX
mov EAX,dword[EBP - 12]
PUSH EDX
push eax
push men
call printf
pop eax
pop eax
POP EDX
mov EAX,dword[EBP - 16]
PUSH EDX
push eax
push men
call printf
pop eax
pop eax
POP EDX
mov EAX,dword[EBP - 20]
PUSH EDX
push eax
push men
call printf
pop eax
pop eax
POP EDX
mov EAX,dword[EBP - 24]
PUSH EDX
PUSH EDX
CMP EAX,1
JNZ PRINT_FALSE_3
PUSH true_msg
PUSH men_bool
call printf
JMP PRINT_FALSE_END_3
PRINT_FALSE_3 : 
PUSH false_msg
PUSH men_bool
call printf
PRINT_FALSE_END_3 : 
ADD ESP,8
POP EDX
call exit
END : 
PUSH errorMsg
call printf
call exit
END2 : 
PUSH errorMsg2
call printf
call exit
