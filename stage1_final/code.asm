POP EAX
MOV dword[EBP-2-2] EAX
POP EAX
MOV EAX, dword[EBP-6-2]
PUSH EAX
MOV EAX, dword[EBP-0-2]
PUSH EAX
POP EBX
POP EAX
CMP EAX, EBX
JLE TRUE0
MOVE EAX, 0
JUMP FALSE0
TRUE0: MOV EAX,1
FALSE0:
PUSH EAX
MOV dword[EBP-8-2] EAX
POP EAX
MOV EAX, dword[EBP-2-2]
PUSH EAX
POP EBX
POP EAX
ADD EAX, EBX
PUSH EAX
MOV dword[EBP-2-2] EAX
POP EAX
MOV EAX, 1
PUSH EAX
MOV dword[EBP-6-2] EAX
POP EAX
