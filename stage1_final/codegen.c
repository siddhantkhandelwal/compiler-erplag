#include "codegen.h"

int num_relop;

void codeGenModuleDef(FILE *fp, tNode *head)
{
    //print label as id.
    tNode *child = head->node.n->child;
    fprintf(fp, "%s:\n", child->node.l->ti->lexeme);
    child = child->node.l->sibling;
    tNode *child_child = child->node.n->child;
    int count = 0;
    while (child_child)
    {
        count++;
        child_child = child_child->node.l->sibling->node.n->sibling;
    }
    fprintf(fp, "SUB ESP, %d\n", 4 * count);
    fprintf(fp, "MOV EDX, ESP\n");
    tNode *outp_list = child->node.n->sibling;
    if (outp_list->node.n->s.N == OUTPUTPLIST)
    {
        child_child = outp_list->node.n->child;
        count = 0;
        while (child_child)
        {
            count++;
            child_child = child_child->node.l->sibling->node.n->sibling;
        }
        fprintf(fp, "SUB ESP, %d\n", 4 * count);
        codeGen(fp, outp_list->node.n->sibling);
    }
    else
    {
        codeGen(fp, outp_list);
    }

    fprintf(fp, "MOV ESP, EBP\n");
    fprintf(fp, "SUB ESP, 8\n");
    fprintf(fp, "POP EBP");
    fprintf(fp, "ret\n");
}

void codeGenModuleReuse(FILE *fp, tNode *head)
{
    fprintf(fp, "MOV dword[ESP-8], EBP\n");
    se *callee_entry = head->entry;
    scope *callee_scope = callee_entry->scope_info;
    tNode *id_list;
    tNode *func;
    if (head->node.n->child->leafTag == 0)
    {
        func = head->node.n->child;
        id_list = head->node.n->child->node.l->sibling;
    }
    else
    {
        func = head->node.n->child->node.n->sibling;
        id_list = head->node.n->child->node.n->sibling->node.l->sibling;
    }
    tNode *id_list_child = id_list->node.n->child;
    se *ip_list_callee = callee_scope->input_list;
    while (id_list_child)
    {
        fprintf(fp, "MOV EDX, dword[EBP-%d-%d]\n", K, id_list_child->entry->offset);
        fprintf(fp, "MOV dword[ESP-%d-%d], EDX\n", K, ip_list_callee->offset);
        id_list_child = id_list_child->node.l->sibling;
        ip_list_callee = ip_list_callee->next;
    }
    fprintf(fp, "MOV EBP, ESP\n");
    fprintf(fp, "call %s\n", func->node.l->ti->lexeme);
    if (head->node.n->child->leafTag)
    {
        tNode *op_list = head->node.n->child;
        tNode *op_list_element = op_list->node.n->child;
        while (op_list_element)
        {
            fprintf(fp, "SUB EDX, 4\n");
            fprintf(fp, "MOV ECX, dword[EDX]\n");
            fprintf(fp, "MOV dword[EBP-%d-%d], ECX", op_list_element->entry->offset, K);
        }
    }
}

void codeGenAssigment(FILE *fp, tNode *head)
{

    if (head->type->basic_type == ARRAY)
    {
        if (head->type->isStatic == 0)
        {
        }
        else
        {
            codeGenExpression(fp, head->node.l->sibling->node.l->sibling);
            fprintf(fp, "POP EAX\n");
            int offset = (head->node.l->sibling->node.l->ti->value.v1 - head->type->start) * 4;
            fprintf(fp, "MOV ECX, dword[EBP-%d-%d]\n", head->entry->offset, K);
            fprintf(fp, "MOV dword[ECX-%d], EAX\n", offset);
        }
    }
    else
    {
        codeGenExpression(fp, head->node.l->sibling);
        fprintf(fp, "POP EAX\n");
        fprintf(fp, "MOV dword[EBP-%d-%d] EAX\n", head->entry->offset, K);
    }
}

void codeGenExpression(FILE *fp, tNode *head)
{
    if (head == NULL)
    {
        return;
    }

    if (head->leafTag == 0)
    {
        if (head->node.l->s.T == ID)
        {
            //Add code for array.
            if (head->type->basic_type == ARRAY)
            {
                if (head->type->isStatic == 0)
                {
                }
                else
                {
                    int offset = (head->node.l->sibling->node.l->ti->value.v1 - head->type->start) * 4;
                    fprintf(fp, "MOV ECX, dword[EBP-%d-%d]\n", head->entry->offset, K);
                    fprintf(fp, "MOV EAX, dword[ECX-%d]", offset);
                    fprintf(fp, "PUSH EAX\n");
                }
            }
            else
            {
                if (head->type->basic_type == INTEGER)
                {
                }
                fprintf(fp, "MOV EAX, dword[EBP-%d-%d]\n", head->entry->offset, K);
                fprintf(fp, "PUSH EAX\n");
            }
        }
        else if (head->node.l->s.T == NUM)
        {
            fprintf(fp, "MOV EAX, %d\n", head->node.l->ti->value.v1);
            fprintf(fp, "PUSH EAX\n");
        }
        else if (head->node.l->s.T == TRUE)
        {
            fprintf(fp, "MOV EAX, 1\n");
            fprintf(fp, "PUSH EAX\n");
        }
        else if (head->node.l->s.T == TRUE)
        {
            fprintf(fp, "MOV EAX, 0\n");
            fprintf(fp, "PUSH EAX\n");
        }
    }
    else
    {
        tNode *temp = head->node.n->child;
        while (temp != NULL)
        {
            codeGenExpression(fp, temp);
            if (temp->leafTag)
                temp = temp->node.n->sibling;
            else
            {
                if (temp->type->basic_type == ARRAY && temp->node.l->sibling)
                {
                    temp = temp->node.l->sibling->node.l->sibling;
                }
                else
                {
                    temp = temp->node.l->sibling;
                }
            }
        }
        if (head->node.n->is_operator)
        {
            fprintf(fp, "POP EBX\n");
            fprintf(fp, "POP EAX\n");
            if (head->node.n->s.T == PLUS)
            {
                fprintf(fp, "ADD EAX, EBX\n");
                fprintf(fp, "PUSH EAX\n");
            }
            else if (head->node.n->s.T == MINUS)
            {
                fprintf(fp, "SUB EAX, EBX\n");
                fprintf(fp, "PUSH EAX\n");
            }
            else if (head->node.n->s.T == MUL)
            {
                fprintf(fp, "IMUL EBX\n");
                fprintf(fp, "PUSH EAX\n");
            }
            else if (head->node.n->s.T == MINUS)
            {
                fprintf(fp, "IDIV EBX\n");
                fprintf(fp, "PUSH EAX\n");
            }
            else if (head->node.n->s.T == AND)
            {
                fprintf(fp, "AND EAX, EBX\n");
                fprintf(fp, "PUSH EAX\n");
            }
            else if (head->node.n->s.T == OR)
            {
                fprintf(fp, "OR EAX, EBX\n");
                fprintf(fp, "PUSH EAX\n");
            }
            else if (head->node.n->s.T == LT)
            {
                fprintf(fp, "CMP EAX, EBX\n");
                fprintf(fp, "JL TRUE%d\n", num_relop);
                fprintf(fp, "MOVE EAX, 0\n");
                fprintf(fp, "JUMP FALSE%d\n", num_relop);
                fprintf(fp, "TRUE%d: MOV EAX,1\n", num_relop);
                fprintf(fp, "FALSE%d:\n", num_relop);
                fprintf(fp, "PUSH EAX\n");
                num_relop++;
            }
            else if (head->node.n->s.T == LE)
            {
                fprintf(fp, "CMP EAX, EBX\n");
                fprintf(fp, "JLE TRUE%d\n", num_relop);
                fprintf(fp, "MOVE EAX, 0\n");
                fprintf(fp, "JUMP FALSE%d\n", num_relop);
                fprintf(fp, "TRUE%d: MOV EAX,1\n", num_relop);
                fprintf(fp, "FALSE%d:\n", num_relop);
                fprintf(fp, "PUSH EAX\n");
                num_relop++;
            }
            else if (head->node.n->s.T == GT)
            {
                fprintf(fp, "CMP EAX, EBX\n");
                fprintf(fp, "JG TRUE%d\n", num_relop);
                fprintf(fp, "MOVE EAX, 0\n");
                fprintf(fp, "JUMP FALSE%d\n", num_relop);
                fprintf(fp, "TRUE%d: MOV EAX,1\n", num_relop);
                fprintf(fp, "FALSE%d:\n", num_relop);
                fprintf(fp, "PUSH EAX\n");
                num_relop++;
            }
            else if (head->node.n->s.T == GE)
            {
                fprintf(fp, "CMP EAX, EBX\n");
                fprintf(fp, "JGE TRUE%d\n", num_relop);
                fprintf(fp, "MOVE EAX, 0\n");
                fprintf(fp, "JUMP FALSE%d\n", num_relop);
                fprintf(fp, "TRUE%d: MOV EAX,1\n", num_relop);
                fprintf(fp, "FALSE%d:\n", num_relop);
                fprintf(fp, "PUSH EAX\n");
                num_relop++;
            }
            else if (head->node.n->s.T == EQ)
            {
                fprintf(fp, "CMP EAX, EBX\n");
                fprintf(fp, "JE TRUE%d\n", num_relop);
                fprintf(fp, "MOVE EAX, 0\n");
                fprintf(fp, "JUMP FALSE%d\n", num_relop);
                fprintf(fp, "TRUE%d: MOV EAX,1\n", num_relop);
                fprintf(fp, "FALSE%d:\n", num_relop);
                fprintf(fp, "PUSH EAX\n");
                num_relop++;
            }
            else if (head->node.n->s.T == NE)
            {
                fprintf(fp, "CMP EAX, EBX\n");
                fprintf(fp, "JNE TRUE%d\n", num_relop);
                fprintf(fp, "MOVE EAX, 0\n");
                fprintf(fp, "JUMP FALSE%d\n", num_relop);
                fprintf(fp, "TRUE%d: MOV EAX,1\n", num_relop);
                fprintf(fp, "FALSE%d:\n", num_relop);
                fprintf(fp, "PUSH EAX\n");
                num_relop++;
            }
        }
    }
}

void printTree(tNode *head)
{
    if (head == NULL)
    {
        return;
    }
    if (head->leafTag == 0)
    {
        if (head->node.l->s.T == ID)
            printf("%s %s", head->node.l->ti->lexeme, terminalDict[head->type->basic_type]);
        return;
    }
    tNode *temp = head->node.n->child;
    while (temp)
    {
        printTree(temp);
        if (temp->leafTag == 0)
        {
            temp = temp->node.l->sibling;
        }
        else
        {
            temp = temp->node.n->sibling;
        }
    }
}

void codeGeniostmt(FILE *fp, tNode *head)
{
    // add this to data section of the assembly code
    // MEM: times 1000 db 0
    // intinputFormat: db \"%%d\",10,0
    // realinputFormat db \"%%f\",10,0
    // intvar: times 8 db 0
    // realvar: times 8 db 0
    // men:  db \"Value is %%d \",10,0
    // arr:  times 2 db 0

    if (head->node.n->child->node.l->s.T == GET_VALUE)
    {
        tNode *id = head->node.n->child->node.l->sibling;
        if (id->entry->type->basic_type == INTEGER || id->entry->type->basic_type == BOOLEAN)
        {
            fprintf(fp, "MOV AX,word[intvar+0]\n");
            fprintf(fp, "MOV RSI,intvar+0\n");
            fprintf(fp, "MOV RDI,intinputFormat\n");
            fprintf(fp, "XOR RAX,RAX\n");
            fprintf(fp, "call scanf\n");
            fprintf(fp, "MOV AX,word[intvar+0]\n");
            if (id->entry->type->basic_type == INTEGER)
            {
                fprintf(fp, "MOV word[MEM+%d],AX\n", id->entry->offset);
            }
            else
            {
                fprintf(fp, "MOV byte[MEM+%d],AX\n", id->entry->offset);
            }
        }
        else
        {
            fprintf(fp, "MOV AX,word[realvar+0]\n");
            fprintf(fp, "MOV RSI,realvar+0\n");
            fprintf(fp, "MOV RDI,realinputFormat\n");
            fprintf(fp, "XOR RAX,RAX\n");
            fprintf(fp, "call scanf\n");
            fprintf(fp, "MOV AX,word[realvar+0]\n");
            fprintf(fp, "MOV word[MEM+%d],AX\n", id->entry->offset);
        }
    }
    else
    {
        tNode *var = head->node.n->child->node.l->sibling;
        if (var->node.n->child->node.n->s.N == BOOLCONSTT)
        {
            if (var->node.n->child->node.n->child->node.l->s.T == TRUE)
            {
                fprintf(fp, "mov AX,1\n");
            }
            else
            {
                fprintf(fp, "mov AX,0\n");
            }
        }
        else if (var->node.n->child->node.l->s.T == NUM || var->node.n->child->node.l->s.T == RNUM)
        {
            fprintf(fp, "mov AX,%s\n", var->node.n->child->node.l->ti->lexeme);
        }
        else if (var->node.n->child->node.l->s.T == ID && var->node.n->child->node.l->sibling == NULL)
        {
            fprintf(fp, "mov AX,word[MEM+%d]\n", var->node.n->child->entry->offset);
        }
        else
        {
            //array
        }
        fprintf(fp, "push rax\n");
        fprintf(fp, "push rcx\n");
        fprintf(fp, "mov rdi, men\n");
        fprintf(fp, "mov rsi, rax\n");
        fprintf(fp, "xor rax, rax\n");
        fprintf(fp, "call printf\n");
        fprintf(fp, "pop rcx\n");
        fprintf(fp, "pop rax\n");
    }
}

void codeGen(FILE *fp, tNode *head)
{
    if (head == NULL || head->leafTag == 0)
    {
        return;
    }

    if (head->node.n->s.N == ASSIGNMENTSTMT)
    {
        codeGenAssigment(fp, head->node.n->child);
    }
    tNode *temp = head->node.n->child;
    while (temp)
    {
        codeGen(fp, temp);
        if (temp->leafTag == 0)
        {
            temp = temp->node.l->sibling;
        }
        else
        {
            temp = temp->node.n->sibling;
        }
    }
    // printTree(head);
}