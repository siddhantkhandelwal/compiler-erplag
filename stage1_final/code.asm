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
array_parameter_demo:
SUB ESP, 12
MOV EDX, ESP
SUB ESP, 12
PUSH EDX
PUSH array_output
call printf
add esp,4
POP EDX
XOR ECX, ECX
PUSH EDX
FOR_LOOP_1 : MOV EDX, dword[EBP-12]
SUB EDX,ECX
mov EAX,dword[SS:EDX-4]
push edx
push ecx
push eax
push array_element
call printf
pop eax
pop eax
pop ecx
pop edx
ADD ECX, 4
CMP ECX, 20
JL FOR_LOOP_1
PUSH EDX
PUSH array_end
call printf
add esp,4
pop edx
POP EDX
PUSH EDX
PUSH array_output
call printf
add esp,4
POP EDX
XOR ECX, ECX
PUSH EDX
FOR_LOOP_2 : MOV EDX, dword[EBP-16]
ADD EDX, dyn_arrays
ADD EDX,ECX
mov EAX,dword[DS:EDX]
push edx
push ecx
push eax
push array_element
call printf
pop eax
pop eax
pop ecx
pop edx
ADD ECX, 4
MOV EDI, dword[end + 0]
SUB EDI, dword[start + 0]
INC EDI
SHL EDI, 2
CMP ECX, EDI
JL FOR_LOOP_2
PUSH EDX
PUSH array_end
call printf
add esp,4
pop edx
POP EDX
PUSH EAX
PUSH EAX
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
MOV dword[EBP-44],EAX
mov EDI, ESP
SUB EDI, 4
push EDI
SUB ESP, 20
MOV EDX,EDI
mov EDI, ESP
SUB EDI, 4
push EDI
SUB ESP, 20
MOV EDX,EDI
mov EDI, dword[dynOffset]
 push edi
PUSH EAX
MOV EAX, dword[EBP-40]
MOV dword[start + 4], EAX
POP EAX
PUSH EAX
MOV EAX, dword[EBP-44]
MOV dword[end + 4], EAX
POP EAX
PUSH EAX
MOV EAX, dword[end + 4]
SUB EAX, dword[start + 4]
CMP EAX, 0
JL END2
INC EAX
SHL EAX, 2
ADD dword[dynOffset], EAX
POP EAX
MOV EDX,EDI
PUSH ECX
PUSH EDX
PUSH 5
PUSH arr_msg
call printf
add esp,8
POP EDX
POP ECX
XOR ECX, ECX
FOR_LOOP_3 : PUSH EDX
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
MOV dword[SS:EDX- 60],EAX
POP EDX
ADD ECX, 4
CMP ECX, 20
JL FOR_LOOP_3
PUSH ECX
PUSH EDX
MOV EDI, dword[end + 4]
SUB EDI, dword[start + 4]
INC EDI
PUSH EDI
PUSH arr_msg
call printf
add esp,8
POP EDX
POP ECX
XOR ECX, ECX
FOR_LOOP_4 : PUSH EDX
PUSH ECX
PUSH intvar+0
PUSH intinputFormat
call scanf
ADD ESP,8
POP ECX
POP EDX
MOV EAX,dword[intvar+0]
PUSH EDX
MOV EDX, dyn_arrays
ADD EDX, dword[EBP-104]
ADD EDX,ECX
MOV dword[DS:EDX],EAX
POP EDX
ADD ECX, 4
MOV EDI, dword[end + 4]
SUB EDI, dword[start + 4]
INC EDI
SHL EDI, 2
CMP ECX, EDI
JL FOR_LOOP_4
PUSH EDX
PUSH array_output
call printf
add esp,4
POP EDX
XOR ECX, ECX
PUSH EDX
FOR_LOOP_5 : MOV EDX, dword[EBP-56]
SUB EDX,ECX
mov EAX,dword[SS:EDX-4]
push edx
push ecx
push eax
push array_element
call printf
pop eax
pop eax
pop ecx
pop edx
ADD ECX, 4
CMP ECX, 20
JL FOR_LOOP_5
PUSH EDX
PUSH array_end
call printf
add esp,4
pop edx
POP EDX
PUSH EDX
MOV EAX, 0
PUSH EAX
POP EAX
MOV [dword EBP-8-12],EAX
POP EDX
PUSH EDX
MOV ECX, 10
FOR_LOOP_6 : MOV DWORD[EBP-32], ECX
push EDX
push EBX
mov EBX, dword[EBP-32]
cmp ebx,10
JL END
cmp ebx,14
JG END
sub EBX,10
shl EBX,2
mov EDX, dword[EBP-56]
sub edx,ebx
sub edx,4
mov EAX, dword[EDX]
pop EBX
pop EDX
push edx
push eax
push men
call printf
add esp,8
pop edx
PUSH EDX
MOV EAX, [dword EBP-8-12]
PUSH EAX
MOV EDX, dword[EBP- 32]
PUSH ECX
MOV ECX,10
CMP EDX,ECX
JL END
MOV ECX,14
CMP EDX,ECX
JG END
POP ECX
MOV ECX, [dword EBP-56]
SUB EDX, 10
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
MOV [dword EBP-8-12],EAX
POP EDX
mov EAX,dword[EBP - 20]
PUSH EDX
push eax
push men
call printf
pop eax
pop eax
POP EDX
MOV ECX, DWORD[EBP-32]
INC ECX
CMP ECX, 14
JLE FOR_LOOP_6
POP EDX
PUSH EDX
PUSH array_output
call printf
add esp,4
POP EDX
XOR ECX, ECX
PUSH EDX
FOR_LOOP_7 : MOV EDX, dword[EBP-56]
SUB EDX,ECX
mov EAX,dword[SS:EDX-4]
push edx
push ecx
push eax
push array_element
call printf
pop eax
pop eax
pop ecx
pop edx
ADD ECX, 4
CMP ECX, 20
JL FOR_LOOP_7
PUSH EDX
PUSH array_end
call printf
add esp,4
pop edx
POP EDX
mov EAX,dword[EBP - 20]
PUSH EDX
push eax
push men
call printf
pop eax
pop eax
POP EDX
PUSH EDX
MOV EDX, 6
CMP EDX, dword[start + 4]
JL END
CMP EDX, dword[end + 4]
JG END
MOV EDI, dword[EBP-104]
ADD EDI, dyn_arrays
SUB EDX, dword[start + 4]
SHL EDX, 2
ADD EDI, EDX
MOV EAX, [dword DS: EDI]
PUSH EAX
MOV EDX, 7
CMP EDX, dword[start + 4]
JL END
CMP EDX, dword[end + 4]
JG END
MOV EDI, dword[EBP-104]
ADD EDI, dyn_arrays
SUB EDX, dword[start + 4]
SHL EDX, 2
ADD EDI, EDX
MOV EAX, [dword DS: EDI]
PUSH EAX
POP EBX
POP EAX
ADD EAX, EBX
PUSH EAX
MOV EDX, 5
CMP EDX, dword[start + 4]
JL END
CMP EDX, dword[end + 4]
JG END
MOV EDI, dword[EBP-104]
ADD EDI, dyn_arrays
SUB EDX, dword[start + 4]
SHL EDX, 2
ADD EDI, EDX
POP EAX
MOV [dword DS: EDI],EAX
POP EDX
PUSH EDX
PUSH array_output
call printf
add esp,4
POP EDX
XOR ECX, ECX
PUSH EDX
FOR_LOOP_8 : MOV EDX, dword[EBP-104]
ADD EDX, dyn_arrays
ADD EDX,ECX
mov EAX,dword[DS:EDX]
push edx
push ecx
push eax
push array_element
call printf
pop eax
pop eax
pop ecx
pop edx
ADD ECX, 4
MOV EDI, dword[end + 4]
SUB EDI, dword[start + 4]
INC EDI
SHL EDI, 2
CMP ECX, EDI
JL FOR_LOOP_8
PUSH EDX
PUSH array_end
call printf
add esp,4
pop edx
POP EDX
PUSH EDX
MOV EAX, 0
PUSH EAX
POP EAX
MOV [dword EBP-12-12],EAX
POP EDX
PUSH EDX
MOV EDX,dword[EBP-12]
MOV dword[EBP-80],EDX
POP EDX
PUSH EDX
PUSH array_output
call printf
add esp,4
POP EDX
XOR ECX, ECX
PUSH EDX
FOR_LOOP_9 : MOV EDX, dword[EBP-80]
SUB EDX,ECX
mov EAX,dword[SS:EDX-4]
push edx
push ecx
push eax
push array_element
call printf
pop eax
pop eax
pop ecx
pop edx
ADD ECX, 4
CMP ECX, 20
JL FOR_LOOP_9
PUSH EDX
PUSH array_end
call printf
add esp,4
pop edx
POP EDX
PUSH EDX
MOV ECX, 10
FOR_LOOP_10 : MOV DWORD[EBP-36], ECX
push EDX
push EBX
mov EBX, dword[EBP-36]
cmp ebx,10
JL END
cmp ebx,14
JG END
sub EBX,10
shl EBX,2
mov EDX, dword[EBP-80]
sub edx,ebx
sub edx,4
mov EAX, dword[EDX]
pop EBX
pop EDX
push edx
push eax
push men
call printf
add esp,8
pop edx
PUSH EDX
MOV EAX, [dword EBP-12-12]
PUSH EAX
MOV EDX, dword[EBP- 36]
PUSH ECX
MOV ECX,10
CMP EDX,ECX
JL END
MOV ECX,14
CMP EDX,ECX
JG END
POP ECX
MOV ECX, [dword EBP-80]
SUB EDX, 10
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
MOV [dword EBP-12-12],EAX
POP EDX
mov EAX,dword[EBP - 24]
PUSH EDX
push eax
push men
call printf
pop eax
pop eax
POP EDX
MOV ECX, DWORD[EBP-36]
INC ECX
CMP ECX, 14
JLE FOR_LOOP_10
POP EDX
PUSH EDX
PUSH array_output
call printf
add esp,4
POP EDX
XOR ECX, ECX
PUSH EDX
FOR_LOOP_11 : MOV EDX, dword[EBP-80]
SUB EDX,ECX
mov EAX,dword[SS:EDX-4]
push edx
push ecx
push eax
push array_element
call printf
pop eax
pop eax
pop ecx
pop edx
ADD ECX, 4
CMP ECX, 20
JL FOR_LOOP_11
PUSH EDX
PUSH array_end
call printf
add esp,4
pop edx
POP EDX
mov EAX,dword[EBP - 24]
PUSH EDX
push eax
push men
call printf
pop eax
pop eax
POP EDX
PUSH EDX
MOV EAX, 0
PUSH EAX
POP EAX
MOV [dword EBP-16-12],EAX
POP EDX
PUSH EDX
PUSH array_output
call printf
add esp,4
POP EDX
XOR ECX, ECX
PUSH EDX
FOR_LOOP_12 : MOV EDX, dword[EBP-16]
ADD EDX, dyn_arrays
ADD EDX,ECX
mov EAX,dword[DS:EDX]
push edx
push ecx
push eax
push array_element
call printf
pop eax
pop eax
pop ecx
pop edx
ADD ECX, 4
MOV EDI, dword[end + 0]
SUB EDI, dword[start + 0]
INC EDI
SHL EDI, 2
CMP ECX, EDI
JL FOR_LOOP_12
PUSH EDX
PUSH array_end
call printf
add esp,4
pop edx
POP EDX
PUSH EDX
MOV ECX, 1
FOR_LOOP_13 : MOV DWORD[EBP-52], ECX
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
MOV dword[EBP-48],EAX
PUSH EDX
MOV EAX, [dword EBP-16-12]
PUSH EAX
MOV EDX, dword[EBP-48]
CMP EDX, dword[start + 0]
JL END
CMP EDX, dword[end + 0]
JG END
MOV EDI, dword[EBP-16]
ADD EDI, dyn_arrays
SUB EDX, dword[start + 0]
SHL EDX, 2
ADD EDI, EDX
MOV EAX, [dword DS: EDI]
PUSH EAX
POP EBX
POP EAX
ADD EAX, EBX
PUSH EAX
POP EAX
MOV [dword EBP-16-12],EAX
POP EDX
mov EAX,dword[EBP - 28]
PUSH EDX
push eax
push men
call printf
pop eax
pop eax
POP EDX
MOV ECX, DWORD[EBP-52]
INC ECX
CMP ECX, 4
JLE FOR_LOOP_13
POP EDX
mov EAX,dword[EBP - 28]
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
main:
mov ebp, esp
push EAX
push EAX
PUSH EAX
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
mov EDI, ESP
SUB EDI, 4
push EDI
SUB ESP, 20
MOV EDX,EDI
mov EDI, dword[dynOffset]
 push edi
PUSH EAX
MOV EAX, dword[EBP-12]
MOV dword[start + 8], EAX
POP EAX
PUSH EAX
MOV EAX, dword[EBP-16]
MOV dword[end + 8], EAX
POP EAX
PUSH EAX
MOV EAX, dword[end + 8]
SUB EAX, dword[start + 8]
CMP EAX, 0
JL END2
INC EAX
SHL EAX, 2
ADD dword[dynOffset], EAX
POP EAX
MOV EDX,EDI
PUSH ECX
PUSH EDX
PUSH 5
PUSH arr_msg
call printf
add esp,8
POP EDX
POP ECX
XOR ECX, ECX
FOR_LOOP_14 : PUSH EDX
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
MOV dword[SS:EDX- 36],EAX
POP EDX
ADD ECX, 4
CMP ECX, 20
JL FOR_LOOP_14
PUSH ECX
PUSH EDX
MOV EDI, dword[end + 8]
SUB EDI, dword[start + 8]
INC EDI
PUSH EDI
PUSH arr_msg
call printf
add esp,8
POP EDX
POP ECX
XOR ECX, ECX
FOR_LOOP_15 : PUSH EDX
PUSH ECX
PUSH intvar+0
PUSH intinputFormat
call scanf
ADD ESP,8
POP ECX
POP EDX
MOV EAX,dword[intvar+0]
PUSH EDX
MOV EDX, dyn_arrays
ADD EDX, dword[EBP-56]
ADD EDX,ECX
MOV dword[DS:EDX],EAX
POP EDX
ADD ECX, 4
MOV EDI, dword[end + 8]
SUB EDI, dword[start + 8]
INC EDI
SHL EDI, 2
CMP ECX, EDI
JL FOR_LOOP_15
PUSH EDX
MOV [dword ESP-8], EBP
MOV EDX, [dword EBP-12-20]
MOV [dword ESP-12-0], EDX
MOV EDX, [dword EBP-12-44]
MOV [dword ESP-12-4], EDX
MOV EDX, dword[start+8]
MOV dword[start+0], EDX
MOV EDX, dword[end+8]
MOV dword[end+0], EDX
MOV EBP, ESP
call array_parameter_demo
SUB EDX, 4
MOV ECX, [dword EDX]
MOV [dword EBP-8-12], ECX
SUB EDX, 4
MOV ECX, [dword EDX]
MOV [dword EBP-12-12], ECX
SUB EDX, 4
MOV ECX, [dword EDX]
MOV [dword EBP-16-12], ECX
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
push eax
push men
call printf
pop eax
pop eax
POP EDX
mov EAX,dword[EBP - 28]
PUSH EDX
push eax
push men
call printf
pop eax
pop eax
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
