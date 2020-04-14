#include "codegen.h"

int num_relop = 0;
int num_conditional = 0;
int num_for = 0;
int num_while = 0;

void codeGenModuleDef(FILE *fp, tNode *head)
{
    //print label as id.
    tNode *child = head->node.n->child;
    fprintf(fp, "PROC %s:\n", child->node.l->ti->lexeme);
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
    fprintf(fp, "MOV [dword ESP-8], EBP\n");
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
        fprintf(fp, "MOV EDX, [dword EBP-%d-%d]\n", K, id_list_child->entry->offset);
        fprintf(fp, "MOV [dword ESP-%d-%d], EDX\n", K, ip_list_callee->offset);
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
            fprintf(fp, "MOV ECX, [dword EDX]\n");
            fprintf(fp, "MOV [dword EBP-%d-%d], ECX\n", op_list_element->entry->offset, K);
            op_list_element = op_list_element->node.l->sibling;
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
            fprintf(fp, "MOV ECX, [dword EBP-%d-%d]\n", head->entry->offset, K);
            fprintf(fp, "MOV [dword ECX-%d], EAX\n", offset);
        }
    }
    else
    {
        codeGenExpression(fp, head->node.l->sibling);
        fprintf(fp, "POP EAX\n");
        fprintf(fp, "MOV [dword EBP-%d-%d],EAX\n", head->entry->offset, K);
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
                    fprintf(fp, "MOV ECX, [dword EBP-%d-%d]\n", head->entry->offset, K);
                    fprintf(fp, "MOV EAX, [dword ECX-%d]\n", (offset+4));
                    fprintf(fp, "PUSH EAX\n");
                }
            }
            else
            {
                if (head->type->basic_type == INTEGER)
                {
                }
                fprintf(fp, "MOV EAX, [dword EBP-%d-%d]\n", head->entry->offset, K);
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
        else if (head->node.l->s.T == FALSE)
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
                fprintf(fp, "MOV EAX, 0\n");
                fprintf(fp, "JMP FALSE%d\n", num_relop);
                fprintf(fp, "TRUE%d: MOV EAX,1\n", num_relop);
                fprintf(fp, "FALSE%d:\n", num_relop);
                fprintf(fp, "PUSH EAX\n");
                num_relop++;
            }
            else if (head->node.n->s.T == LE)
            {
                fprintf(fp, "CMP EAX, EBX\n");
                fprintf(fp, "JLE TRUE%d\n", num_relop);
                fprintf(fp, "MOV EAX, 0\n");
                fprintf(fp, "JMP FALSE%d\n", num_relop);
                fprintf(fp, "TRUE%d: MOV EAX,1\n", num_relop);
                fprintf(fp, "FALSE%d:\n", num_relop);
                fprintf(fp, "PUSH EAX\n");
                num_relop++;
            }
            else if (head->node.n->s.T == GT)
            {
                fprintf(fp, "CMP EAX, EBX\n");
                fprintf(fp, "JG TRUE%d\n", num_relop);
                fprintf(fp, "MOV EAX, 0\n");
                fprintf(fp, "JMP FALSE%d\n", num_relop);
                fprintf(fp, "TRUE%d: MOV EAX,1\n", num_relop);
                fprintf(fp, "FALSE%d:\n", num_relop);
                fprintf(fp, "PUSH EAX\n");
                num_relop++;
            }
            else if (head->node.n->s.T == GE)
            {
                fprintf(fp, "CMP EAX, EBX\n");
                fprintf(fp, "JGE TRUE%d\n", num_relop);
                fprintf(fp, "MOV EAX, 0\n");
                fprintf(fp, "JMP FALSE%d\n", num_relop);
                fprintf(fp, "TRUE%d: MOV EAX,1\n", num_relop);
                fprintf(fp, "FALSE%d:\n", num_relop);
                fprintf(fp, "PUSH EAX\n");
                num_relop++;
            }
            else if (head->node.n->s.T == EQ)
            {
                fprintf(fp, "CMP EAX, EBX\n");
                fprintf(fp, "JE TRUE%d\n", num_relop);
                fprintf(fp, "MOV EAX, 0\n");
                fprintf(fp, "JMP FALSE%d\n", num_relop);
                fprintf(fp, "TRUE%d: MOV EAX,1\n", num_relop);
                fprintf(fp, "FALSE%d:\n", num_relop);
                fprintf(fp, "PUSH EAX\n");
                num_relop++;
            }
            else if (head->node.n->s.T == NE)
            {
                fprintf(fp, "CMP EAX, EBX\n");
                fprintf(fp, "JNE TRUE%d\n", num_relop);
                fprintf(fp, "MOV EAX, 0\n");
                fprintf(fp, "JMP FALSE%d\n", num_relop);
                fprintf(fp, "TRUE%d: MOV EAX,1\n", num_relop);
                fprintf(fp, "FALSE%d:\n", num_relop);
                fprintf(fp, "PUSH EAX\n");
                num_relop++;
            }
        }
    }
}

void generate_conditional(FILE* code , tNode *cond_stmt)
{

    tNode *id = cond_stmt->node.n->child;
    tNode *case_stmts = id->node.l->sibling;
    tNode *actual_stmts = case_stmts->node.n->child;
    tNode *default_stmt = case_stmts->node.n->sibling;

    num_conditional++;

    if (id->entry->type->basic_type == INTEGER)
    {

        fprintf(code, "MOV ECX, [dword EBP-%d]\n", (K+id->entry->offset));

        tNode *curr_stmt = actual_stmts;
        tNode *next_stmt = actual_stmts->node.n->sibling->node.n->sibling;

        while (curr_stmt)
        {

            fprintf(code, "CMP ECX, %d\n", curr_stmt->node.n->child->node.l->ti->value.v1);
            if (next_stmt)
            {
                fprintf(code, "JNZ CASE_%d-%d\n", num_conditional, next_stmt->node.n->child->node.l->ti->value.v1);
            }
            else
            {
                fprintf(code, "JNZ DEFAULT_%d\n", num_conditional);
            }

            codeGen(code,curr_stmt->node.n->sibling);
            fprintf(code, "JMP SWITCH_END%d\n", num_conditional);
            if (next_stmt)
                fprintf(code, "CASE_%d-%d : ", num_conditional, next_stmt->node.n->child->node.l->ti->value.v1);

            tNode *temp = next_stmt;
            curr_stmt = temp;
            if (temp)
                next_stmt = next_stmt->node.n->sibling->node.n->sibling;
        }

        fprintf(code, "DEFAULT_%d : ", num_conditional);
        codeGen(code,default_stmt->node.n->child);
        fprintf(code, "SWITCH_END%d : ", num_conditional);
    }
    else
    {

        fprintf(code, "MOV ECX, DWORD[EBP-%d]\n", (id->entry->offset+K));
        tNode *fst_stmt = actual_stmts;
        tNode *sec_stmt = actual_stmts->node.n->sibling->node.n->sibling;

        fprintf(code, "CMP ECX, 1\n");
        fprintf(code, "JNZ FALSE_%d\n", num_conditional);
        if (fst_stmt->node.n->child->node.l->ti->t == TRUE)
        {
            fprintf(code, "TRUE_%d : ", num_conditional);
            codeGen(code,fst_stmt->node.n->sibling);
            fprintf(code, "JMP SWITCH_END%d\n", num_conditional);
            fprintf(code, "FALSE_%d : \n", num_conditional);
            if (sec_stmt)
                codeGen(code,sec_stmt->node.n->sibling);
            fprintf(code, "JMP SWITCH_END%d\n", num_conditional);
        }
        else
        {

            fprintf(code, "TRUE_%d : ", num_conditional);
            if (sec_stmt)
                codeGen(code,sec_stmt->node.n->sibling);
            fprintf(code, "JMP SWITCH_END%d\n", num_conditional);
            fprintf(code, "FALSE_%d : \n", num_conditional);
            codeGen(code,fst_stmt->node.n->sibling);
            fprintf(code, "JMP SWITCH_END%d\n", num_conditional);
        }
    }
}

void generate_iterative(FILE* code ,tNode *head)
{

    tNode *loop_type = head->node.n->child;

    if (loop_type->node.l->ti->t == FOR)
    {

        num_for++;

        tNode *id = loop_type->node.l->sibling;
        tNode *range = id->node.l->sibling;
        tNode *lower = range->node.n->child;
        tNode *upper = lower->node.l->sibling;
        fprintf(code, "MOV ECX, %d\n", lower->node.l->ti->value.v1);
        fprintf(code, "FOR_LOOP_%d : ", num_for);
        fprintf(code, "MOV DWORD[EBP-%d], ECX\n", (id->entry->offset)+K);
        codeGen(code,range->node.n->sibling);
        fprintf(code, "MOV ECX, DWORD[EBP-%d]\n", (id->entry->offset+K)); // Assuming that loop variable is not changed inside the loop
        fprintf(code, "INC ECX\n");
        fprintf(code, "CMP ECX, %d\n", upper->node.l->ti->value.v1);
        fprintf(code, "JLE FOR_LOOP_%d\n", num_for);
    }
    else
    {

        num_while++;
        tNode *loop_expr = loop_type->node.l->sibling;
        fprintf(code, "WHILE_LOOP_CHECK_%d : \n", num_while);
        codeGenExpression(code, loop_expr);
        fprintf(code, "POP EAX\n");
        fprintf(code, "CMP EAX, 1\n");
        fprintf(code, "JNZ WHILE_LOOP_END%d\n", num_while);
        fprintf(code, "WHILE_LOOP_%d : ", num_while);
        codeGen(code,loop_expr->node.n->sibling);
        fprintf(code, "JMP WHILE_LOOP_CHECK_%d\n", num_while);
        fprintf(code, "WHILE_LOOP_END%d : ", num_while);
    }
}

void codeGeniostmt(FILE *fp, tNode *head)
{
    // add this to data section of the assembly code
    // intinputFormat: db \"%d\", 10
    // realinputFormat db \"%f\", 10
    // intvar: times 8 db 0
    // realvar: times 8 db 0    Change this to accomodate arrays
    // men:  db \"Output: %d \", 10

    if (head->node.n->child->node.l->s.T == GET_VALUE)
    {
        // Get_VALUE
        tNode *id = head->node.n->child->node.l->sibling;
        if (id->entry->is_array == 1)
        {
            int width = (id->entry->type->end - id->entry->type->start)+1;
            int array_size;
            if (id->entry->type->element_type == INTEGER)
            {
                array_size = (width) * 4;
            }
            else if (id->entry->type->element_type == BOOLEAN)
            {
                array_size = (width) * 4;
            }
            else
            {
                array_size = (width) * 4;
            }
            num_for++;
            fprintf(fp, "XOR ECX, ECX\n");
            fprintf(fp, "FOR_LOOP_%d : ", num_for);
            if (id->entry->type->element_type == INTEGER || id->entry->type->element_type == BOOLEAN)
            {
        /*        fprintf(fp, "MOV RSI,intvar+0\n");
                fprintf(fp, "MOV RDI,intinputFormat\n");
                fprintf(fp, "XOR RAX,RAX\n");
                fprintf(fp, "call scanf\n");
                fprintf(fp, "MOV EAX,dword[intvar+0]\n");
              //  if (id->entry->type->element_type == INTEGER)
                //{
        */      fprintf(fp,"PUSH ECX\n");
                fprintf(fp, "PUSH intvar+0\n");
                fprintf(fp, "PUSH intinputFormat\n");
                fprintf(fp, "call scanf\n");
                fprintf(fp,"ADD ESP,8\n");
                fprintf(fp, "POP ECX\n");
                fprintf(fp, "MOV EAX,dword[intvar+0]\n");
                fprintf(fp, "PUSH EDX\n");
                fprintf(fp, "MOV EDX,EBP\n");
                fprintf(fp, "SUB EDX,ECX\n");
                fprintf(fp, "MOV dword[SS:EDX- %d],EAX\n",(K+id->entry->offset+4));
                fprintf(fp, "POP EDX\n");
                //}
                //else
                //{
            }
            else
            {
                fprintf(fp, "PUSH realvar+0\n");
                fprintf(fp, "PUSH realinputFormat\n");
                //fprintf(fp, "XOR EAX,EAX\n");
                fprintf(fp, "call scanf\n");
                fprintf(fp, "MOV EAX, dword[realvar+0]\n");
                fprintf(fp, "PUSH EDX\n");
                fprintf(fp, "MOV EDX,EBP\n");
                fprintf(fp, "SUB EDX,ECX\n");
                fprintf(fp, "MOV dword[SS:EDX- %d],EAX\n",(K+id->entry->offset+4));
                fprintf(fp, "PUSH EDX\n");
            }
            if (id->entry->type->element_type == INTEGER)
            {
                fprintf(fp, "ADD ECX, 4\n");
            }
            else if (id->entry->type->element_type == BOOLEAN)
            {
                fprintf(fp, "ADD ECX, 4\n");
            }
            else
            {
                fprintf(fp, "ADD ECX, 4\n");
            }
            fprintf(fp, "CMP ECX, %d\n", array_size);
            fprintf(fp, "JL FOR_LOOP_%d\n", num_for);
        }
        else
        {
            if (id->entry->type->basic_type == INTEGER || id->entry->type->basic_type == BOOLEAN)
            {
                fprintf(fp, "PUSH intvar+0\n");
                fprintf(fp, "PUSH intinputFormat\n");
                //fprintf(fp, "XOR EAX,EAX\n");
                fprintf(fp, "call scanf\n");
                fprintf(fp, "ADD ESP,8\n");
                fprintf(fp, "MOV EAX,dword[intvar+0]\n");
                if (id->entry->type->basic_type == INTEGER)
                {
                    fprintf(fp, "MOV dword[EBP-%d],EAX\n", (K+id->entry->offset));
                }
                else
                {
                    fprintf(fp, "MOV dword[EBP-%d],EAX\n", (K+id->entry->offset));
                }
            }
            else
            {
                fprintf(fp, "PUSH realvar+0\n");
                fprintf(fp, "PUSH realinputFormat\n");
                fprintf(fp, "call scanf\n");
                fprintf(fp, "MOV EAX,dword[realvar+0]\n");
                fprintf(fp, "MOV dword[EBP-%d],EAX\n", (K+id->entry->offset));
            }
        }
    }
    else
    {
        // print
        tNode *var = head->node.n->child->node.l->sibling;
        tNode *id = var->node.n->child;
        if (var->node.n->child->node.l->s.T == ID && var->node.n->child->entry->is_array == 1)
        {

            int width = (id->entry->type->end - id->entry->type->start)+1;
            if (id->node.l->sibling != NULL)
            {
                // array element
                int off = id->entry->offset;
                if (id->node.l->sibling->node.l->s.T == NUM)
                {
                    int disp = id->node.l->sibling->node.l->ti->value.v1 - id->entry->type->start;
                    fprintf(fp,"push EDX\n");
                    fprintf(fp,"mov EDX, dword[EBP-%d]\n",(K+off));
                    fprintf(fp, "mov EAX,dword[EDX-%d]\n", ((disp*4)+4));
                    fprintf(fp, "pop EDX\n");
                }
                else if (id->node.l->sibling->node.l->s.T == ID)
                {
                    fprintf(fp,"push EDX\n");
                    fprintf(fp,"push EBX\n");
                    fprintf(fp, "mov EBX, dword[EBP-%d]\n",(id->node.l->sibling->entry->offset+K));
                    fprintf(fp,"mov EDX, dword[EBP-%d]\n",(K+off));
                    fprintf(fp, "mov EAX, dword[EDX-EBX*4-4]\n");
                    fprintf(fp, "pop EBX\n");
                    fprintf(fp, "pop EDX\n");
                }
                fprintf(fp, "push eax\n");
                fprintf(fp, "push ecx\n");
                fprintf(fp, "mov edi, men\n");
                fprintf(fp, "mov esi, eax\n");
                fprintf(fp, "xor eax, eax\n");
                fprintf(fp, "call printf\n");
                fprintf(fp, "pop ecx\n");
                fprintf(fp, "pop eax\n");
            }
            else
            {
                // complete array
                int array_size;
                if (id->entry->type->element_type == INTEGER)
                {
                    array_size = (width) * 4;
                }
                else if (id->entry->type->element_type == BOOLEAN)
                {
                    array_size = (width) * 4;
                }
                else
                {
                    array_size = (width) * 4;
                }
                num_for++;
                fprintf(fp, "XOR ECX, ECX\n");
                fprintf(fp, "PUSH EDX\n");
                
                fprintf(fp, "FOR_LOOP_%d : ", num_for);
                fprintf(fp, "MOV EDX, dword[EBP-%d]\n",(id->entry->offset+K));
                fprintf(fp, "SUB EDX,ECX\n");
                fprintf(fp, "mov EAX,dword[SS:EDX-4]\n");
                fprintf(fp, "push edx\n");
                fprintf(fp, "push ecx\n");
                fprintf(fp, "push eax\n");
               // fprintf(fp, "push ecx\n");
                fprintf(fp, "push men\n");
               // fprintf(fp, "mov esi, eax\n");
                //fprintf(fp, "xor eax, eax\n");
                fprintf(fp, "call printf\n");
              //  fprintf(fp, "pop ecx\n");
                fprintf(fp, "pop eax\n");
                fprintf(fp, "pop eax\n");
                fprintf(fp, "pop ecx\n");
                fprintf(fp, "pop edx\n");
                if (id->entry->type->element_type == INTEGER)
                {
                    fprintf(fp, "ADD ECX, 4\n");
                }
                else if (id->entry->type->element_type == BOOLEAN)
                {
                    fprintf(fp, "ADD ECX,4\n");
                }
                else
                {
                    fprintf(fp, "ADD ECX, 4\n");
                }
                fprintf(fp, "CMP ECX, %d\n", array_size);
                fprintf(fp, "JL FOR_LOOP_%d\n", num_for);
                fprintf(fp, "POP EDX\n");
            }
        }
        else
        {
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
                fprintf(fp, "mov EAX,%s\n", var->node.n->child->node.l->ti->lexeme);
            }
            else if (var->node.n->child->node.l->s.T == ID && var->node.n->child->node.l->sibling == NULL)
            {
                fprintf(fp, "mov EAX,dword[EBP - %d]\n", (K + var->node.n->child->entry->offset));
            }
            fprintf(fp, "push eax\n");
           //  fprintf(fp, "push ecx\n");
           // fprintf(fp, "MOV EAX, dword[EBP-%d]\n", );
            fprintf(fp, "push men\n");
            //fprintf(fp, "mov esi, eax\n");
            //fprintf(fp, "xor eax, eax\n");
            fprintf(fp, "call printf\n");
           // fprintf(fp, "pop ecx\n");
            fprintf(fp, "pop eax\n");
            fprintf(fp, "pop eax\n");
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

void codeGenInit(FILE* fp, tNode* head){

    fprintf(fp,"extern printf\nextern scanf\n");

    fprintf(fp,"section .data\n");
      
    fprintf(fp,"intinputFormat: db \"%%d\",0\nrealinputFormat: db \"%%f\",0\nintvar: times 100 dd 0\nrealvar: times 100 dd 0\nmen:  db \"Output: %%d \", 10,0\n");

    fprintf(fp,"section .text\nglobal main\nmain:\n");

    fprintf(fp, "mov ebp, esp\n");

    fprintf(fp, "push EAX\npush EAX\n");    

    codeGen(fp,head);

    fprintf(fp, "mov eax,1\nmov ebx,0\nint 80h\n");

    //fprintf(fp, "ret\n");


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
    else if (head->node.n->s.N == CONDITIONALSTMT)
    {
        generate_conditional(fp,head);
        return;
    }
    else if (head->node.n->s.N == ITERATIVESTMT)
    {

        generate_iterative(fp,head);
        return;
    }
    else if(head->node.n->s.N == MODULE_NT){

        codeGenModuleDef(fp,head);
        return;

    }else if(head->node.n->s.N == MODULEREUSESTMT){
        
        codeGenModuleReuse(fp,head);
        return;
    
    }
    else if (head->node.n->s.N == IOSTMT){
     
        codeGeniostmt(fp,head);   
    
    }
    else if(head->node.n->s.N == DECLARESTMT){
        tNode* id_list = head->node.n->child;
        tNode* dt = id_list->node.n->sibling;
        tNode* id_child = id_list->node.n->child;

        if(dt->node.n->child->node.l->s.T != ARRAY){

            while(id_child){

                fprintf(fp,"PUSH EAX\n");
                id_child = id_child->node.l->sibling;
            }
        }else{

            se* temp = id_child->entry;
            if(temp->type->isStatic == 1){

                fprintf(fp,"mov EDX, ESP\n");
                fprintf(fp,"SUB EDX, 4\n");
                fprintf(fp,"push EDX\n");
                int s = temp->type->start;
                int e = temp->type->end;
                int w = e-s+1;
                fprintf(fp,"SUB ESP, %d\n",4*w);
            }
        }
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