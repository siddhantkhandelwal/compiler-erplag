#include "codegen.h"

int num_relop = 0;
int num_conditional = 0;
int num_for = 0;
int num_while = 0;
int dynamic_offset = 0;
int num_print_bool = 0;


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
    fprintf(fp, "SUB ESP, %d\n", 4 * (count+1));
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
    fprintf(fp, "POP EBP\n");
    fprintf(fp, "ret\n");
}

void codeGenModuleReuse(FILE *fp, tNode *head)
{
    fprintf(fp, "PUSH EDX\n");
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
        if(id_list_child->entry->type->basic_type==ARRAY){

            if(id_list_child->entry->type->isStatic==0){
                fprintf(fp, "MOV EDX, dword[start+%d]\n",id_list_child->entry->type->dyn_index);
                fprintf(fp, "MOV dword[start+%d], EDX\n",ip_list_callee->type->dyn_index);
                fprintf(fp, "MOV EDX, dword[end+%d]\n",id_list_child->entry->type->dyn_index);
                fprintf(fp, "MOV dword[end+%d], EDX\n",ip_list_callee->type->dyn_index);
            }
        }

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

    fprintf(fp, "POP EDX\n");
}

void codeGenAssigment(FILE *fp, tNode *head)
{

    if (head->entry->type->basic_type == ARRAY)
    {
        if (head->type->isStatic == 0)
        {
            if(head->entry->type->isStatic==1){ // Dynamic Index Static Array


                codeGenExpression(fp, head->node.l->sibling->node.l->sibling);

                tNode* index = head->node.l->sibling;
                //fprintf(fp,"PUSH EDX\n");
                fprintf(fp,"MOV EDX, dword[EBP- %d]\n",(index->entry->offset+K));
                fprintf(fp,"PUSH ECX\n");
                fprintf(fp, "MOV ECX,%d\n",head->entry->type->start);
                fprintf(fp,"CMP EDX,ECX\n");
                fprintf(fp, "JL END\n");
                fprintf(fp, "MOV ECX,%d\n",head->entry->type->end);
                fprintf(fp,"CMP EDX,ECX\n");
                fprintf(fp, "JG END\n");
                fprintf(fp,"POP ECX\n");
                //fprintf(fp,"POP EDX\n");
                fprintf(fp, "POP EAX\n" );
                fprintf(fp,"MOV ECX, [dword EBP-%d]\n",(head->entry->offset+K));
                fprintf(fp,"SUB EDX, %d\n",head->entry->type->start);
                fprintf(fp,"SHL EDX,2\n");
                fprintf(fp,"ADD EDX,4\n");
                fprintf(fp,"SUB ECX,EDX\n");
                fprintf(fp, "MOV [dword SS : ECX],EAX\n");
               // fprintf(fp, "POP EDX\n");
            }
        }
        else
        {
            codeGenExpression(fp, head->node.l->sibling->node.l->sibling);
            fprintf(fp, "POP EAX\n");
            int offset = (head->node.l->sibling->node.l->ti->value.v1 - head->type->start) * 4;
            printf("%s %d\n",head->node.l->ti->lexeme,head->entry->offset);
            fprintf(fp, "MOV ECX, [dword EBP-%d-%d]\n", head->entry->offset, K);
            fprintf(fp, "MOV [dword ECX-%d], EAX\n", offset+4);
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

                    if(head->entry->type->isStatic==1){ // Dynamic Index Static Array

                        tNode* index = head->node.l->sibling;
                        //fprintf(fp,"PUSH EDX\n");
                        fprintf(fp,"MOV EDX, dword[EBP- %d]\n",(index->entry->offset+K));
                        fprintf(fp,"PUSH ECX\n");
                        fprintf(fp, "MOV ECX,%d\n",head->entry->type->start);
                        fprintf(fp,"CMP EDX,ECX\n");
                        fprintf(fp, "JL END\n");
                        fprintf(fp, "MOV ECX,%d\n",head->entry->type->end);
                        fprintf(fp,"CMP EDX,ECX\n");
                        fprintf(fp, "JG END\n");
                        fprintf(fp,"POP ECX\n");
                        //fprintf(fp,"POP EDX\n");

                        fprintf(fp,"MOV ECX, [dword EBP-%d]\n",(head->entry->offset+K));
                        fprintf(fp,"SUB EDX, %d\n",head->entry->type->start);
                        fprintf(fp,"SHL EDX,2\n");
                        fprintf(fp,"ADD EDX,4\n");
                        fprintf(fp,"SUB ECX,EDX\n");
                        fprintf(fp, "MOV EAX, [dword SS: ECX]\n");
                        fprintf(fp, "PUSH EAX\n");
                    }   
                    else{
                        //Dynamic Array
                        tNode* index = head->node.l->sibling;
                        if(index->node.l->s.T == NUM)
                        {
                            fprintf(fp, "MOV EDX, %d\n", index->node.l->ti->value.v1);
                            fprintf(fp, "CMP EDX, dword[start + %d]\n", head->entry->type->dyn_index * 4);
                            fprintf(fp, "JL END\n");
                            fprintf(fp, "CMP EDX, dword[end + %d]\n", head->entry->type->dyn_index * 4);
                            fprintf(fp, "JG END\n");
                            fprintf(fp, "MOV EDI, dword[EBP-%d]\n", head->entry->offset + K);
                            fprintf(fp, "ADD EDI, dyn_arrays\n");
                            fprintf(fp, "SUB EDX, dword[start + %d]\n", head->entry->type->dyn_index * 4);
                            fprintf(fp, "SHL EDX, 2\n");
                            fprintf(fp, "ADD EDI, EDX\n");
                            fprintf(fp, "MOV EAX, [dword DS: EDI]\n");
                            fprintf(fp, "PUSH EAX\n");
                        }
                        else
                        {
                            fprintf(fp, "MOV EDX, dword[EBP-%d]\n", index->entry->offset + K);
                            fprintf(fp, "CMP EDX, dword[start + %d]\n", head->entry->type->dyn_index * 4);
                            fprintf(fp, "JL END\n");
                            fprintf(fp, "CMP EDX, dword[end + %d]\n", head->entry->type->dyn_index * 4);
                            fprintf(fp, "JG END\n");
                            fprintf(fp, "MOV EDI, dword[EBP-%d]\n", head->entry->offset + K);
                            fprintf(fp, "ADD EDI, dyn_arrays\n");
                            fprintf(fp, "SUB EDX, dword[start + %d]\n", head->entry->type->dyn_index * 4);
                            fprintf(fp, "SHL EDX, 2\n");
                            fprintf(fp, "ADD EDI, EDX\n");
                            fprintf(fp, "MOV EAX, [dword DS: EDI]\n");
                            fprintf(fp, "PUSH EAX\n");
                        }
                        
                    }
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
                fprintf(fp, "MUL EBX\n");
                fprintf(fp, "PUSH EAX\n");
            }
            else if (head->node.n->s.T == DIV)
            {
                fprintf(fp, "DIV EBX\n");
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
                fprintf(code, "JNZ CASE_%d_%d\n", num_conditional, next_stmt->node.n->child->node.l->ti->value.v1);
            }
            else
            {
                fprintf(code, "JNZ DEFAULT_%d\n", num_conditional);
            }

            codeGen(code,curr_stmt->node.n->sibling);
            fprintf(code, "JMP SWITCH_END%d\n", num_conditional);
            if (next_stmt)
                fprintf(code, "CASE_%d_%d : ", num_conditional, next_stmt->node.n->child->node.l->ti->value.v1);

            tNode *temp = next_stmt;
            curr_stmt = temp;
            if (temp)
                next_stmt = next_stmt->node.n->sibling->node.n->sibling;
        }

        fprintf(code, "DEFAULT_%d : \n", num_conditional);
        //printf("%s\n", terminalDict[default_stmt->node.n->child->node.l->s.T]);
        codeGen(code,default_stmt->node.n->child->node.l->sibling);
        fprintf(code, "SWITCH_END%d : \n", num_conditional);
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
        fprintf(code, "SWITCH_END%d : \n", num_conditional);
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
            if(id->entry->type->isStatic == 1)
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
                fprintf(fp,"PUSH ECX\n");
                fprintf(fp,"PUSH EDX\n");
                fprintf(fp,"PUSH %d\n",width);
                fprintf(fp,"PUSH arr_msg\n");
                fprintf(fp,"call printf\n");
                fprintf(fp,"add esp,8\n");
                fprintf(fp,"POP EDX\n");
                fprintf(fp,"POP ECX\n");
                fprintf(fp, "XOR ECX, ECX\n");
                fprintf(fp, "FOR_LOOP_%d : ", num_for);
                if (id->entry->type->element_type == INTEGER || id->entry->type->element_type == BOOLEAN)
                {

                    fprintf(fp,"PUSH EDX\n");
                    fprintf(fp,"PUSH ECX\n");
                    fprintf(fp, "PUSH intvar+0\n");
                    fprintf(fp, "PUSH intinputFormat\n");
                    fprintf(fp, "call scanf\n");
                    fprintf(fp,"ADD ESP,8\n");
                    fprintf(fp, "POP ECX\n");
                    fprintf(fp,"POP EDX\n");
                    fprintf(fp, "MOV EAX,dword[intvar+0]\n");
                    fprintf(fp, "PUSH EDX\n");
                    fprintf(fp, "MOV EDX,EBP\n");
                    fprintf(fp, "SUB EDX,ECX\n");
                    fprintf(fp, "MOV dword[SS:EDX- %d],EAX\n",(K+id->entry->offset+4));
                    fprintf(fp, "POP EDX\n");
                }
            
                //}
                //else
                //{
            
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
                num_for++;
                fprintf(fp,"PUSH ECX\n");
                fprintf(fp,"PUSH EDX\n");
                fprintf(fp, "MOV EDI, dword[end + %d]\n", id->entry->type->dyn_index * 4);
                fprintf(fp, "SUB EDI, dword[start + %d]\n", id->entry->type->dyn_index * 4);
                fprintf(fp, "INC EDI\n");
                fprintf(fp, "PUSH EDI\n");
                fprintf(fp,"PUSH arr_msg\n");
                fprintf(fp,"call printf\n");
                fprintf(fp,"add esp,8\n");
                fprintf(fp,"POP EDX\n");
                fprintf(fp,"POP ECX\n");

                fprintf(fp, "XOR ECX, ECX\n");
                fprintf(fp, "FOR_LOOP_%d : ", num_for);
                if (id->entry->type->element_type == INTEGER || id->entry->type->element_type == BOOLEAN)
                {
                    fprintf(fp,"PUSH EDX\n");
                    fprintf(fp,"PUSH ECX\n");
                    fprintf(fp, "PUSH intvar+0\n");
                    fprintf(fp, "PUSH intinputFormat\n");
                    fprintf(fp, "call scanf\n");
                    fprintf(fp,"ADD ESP,8\n");
                    fprintf(fp, "POP ECX\n");
                    fprintf(fp,"POP EDX\n");
                    fprintf(fp, "MOV EAX,dword[intvar+0]\n");
                    fprintf(fp, "PUSH EDX\n");
                    fprintf(fp, "MOV EDX, dyn_arrays\n");
                    fprintf(fp, "ADD EDX, dword[EBP-%d]\n", (K+id->entry->offset));
                    fprintf(fp, "ADD EDX,ECX\n");
                    fprintf(fp, "MOV dword[DS:EDX],EAX\n");
                    fprintf(fp, "POP EDX\n");
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
                fprintf(fp, "MOV EDI, dword[end + %d]\n", id->entry->type->dyn_index * 4);
                fprintf(fp, "SUB EDI, dword[start + %d]\n", id->entry->type->dyn_index * 4);
                fprintf(fp, "INC EDI\n");
                fprintf(fp, "SHL EDI, 2\n");
                fprintf(fp, "CMP ECX, EDI\n");
                fprintf(fp, "JL FOR_LOOP_%d\n", num_for);
            }
            
        }
        else
        {
            if (id->entry->type->basic_type == INTEGER || id->entry->type->basic_type == BOOLEAN)
            {   

                fprintf(fp,"PUSH EDX\n");
                fprintf(fp,"PUSH int_msg\n");
                fprintf(fp,"call printf\n");
                fprintf(fp,"ADD ESP,4\n");
                fprintf(fp,"POP EDX\n");

                fprintf(fp,"PUSH EDX\n");
                fprintf(fp, "PUSH intvar+0\n");
                fprintf(fp, "PUSH intinputFormat\n");
                //fprintf(fp, "XOR EAX,EAX\n");
                fprintf(fp, "call scanf\n");
                fprintf(fp, "ADD ESP,8\n");
                fprintf(fp,"POP EDX\n");
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
                if(var->node.n->child->entry->type->isStatic==1){
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
                        fprintf(fp, "cmp ebx,%d\n",id->entry->type->start);
                        fprintf(fp, "JL END\n");
                        fprintf(fp, "cmp ebx,%d\n",id->entry->type->end);
                        fprintf(fp, "JG END\n");
                        fprintf(fp, "sub EBX,%d\n",id->entry->type->start);
                        fprintf(fp, "shl EBX,2\n");

                        fprintf(fp,"mov EDX, dword[EBP-%d]\n",(K+off));
                        fprintf(fp, "sub edx,ebx\n");
                        fprintf(fp, "sub edx,4\n");
                        fprintf(fp, "mov EAX, dword[EDX]\n");
                        fprintf(fp, "pop EBX\n");
                        fprintf(fp, "pop EDX\n");
                    }
                }else{
                    if (id->node.l->sibling->node.l->s.T == NUM)
                    {
                        //int disp = id->node.l->sibling->node.l->ti->value.v1 - id->entry->type->start;
                        fprintf(fp,"push EDX\n");
                        fprintf(fp,"mov EDX, dword[EBP-%d]\n",(K+off));
                        fprintf(fp, "cmp dword[start+%d],%d\n",id->entry->type->dyn_index*4,id->node.l->sibling->node.l->ti->value.v1);
                        fprintf(fp, "JG END\n");
                        fprintf(fp, "cmp dword[end+%d],%d\n",id->entry->type->dyn_index*4,id->node.l->sibling->node.l->ti->value.v1);
                        fprintf(fp, "JL END\n");
                        fprintf(fp, "MOV EDX,%d\n",id->node.l->sibling->node.l->ti->value.v1);
                        fprintf(fp, "SUB EDX,dword[start+%d]\n",id->entry->type->dyn_index*4);
                        fprintf(fp, "SHL EDX,2\n");
                        fprintf(fp, "mov EAX,dword[dyn_arrays+EDX]\n");
                        fprintf(fp, "pop EDX\n");
                    }
                    else if (id->node.l->sibling->node.l->s.T == ID)
                    {
                        fprintf(fp,"push EDX\n");
                        fprintf(fp,"push EBX\n");
                        fprintf(fp, "mov EBX, dword[EBP-%d]\n",(id->node.l->sibling->entry->offset+K));
                        fprintf(fp, "cmp ebx,dword[start+%d]\n",id->entry->type->dyn_index*4);
                        fprintf(fp, "JL END\n");
                        fprintf(fp, "cmp ebx,dword[end+%d]\n",id->entry->type->dyn_index*4);
                        fprintf(fp, "JG END\n");
                        fprintf(fp, "sub EBX,dword[start+%d]\n",id->entry->type->dyn_index*4);
                        fprintf(fp, "shl EBX,2\n");

                        fprintf(fp,"mov EDX, dword[EBP-%d]\n",(K+off));
                        fprintf(fp, "add edx,ebx\n");
                        fprintf(fp, "mov EAX, dword[dyn_arrays+EDX]\n");
                        fprintf(fp, "pop EBX\n");
                        fprintf(fp, "pop EDX\n");
                    }
                }

                if(var->node.n->child->entry->type->element_type == INTEGER){
                    fprintf(fp,"push edx\n");
                    fprintf(fp,"push eax\n");
                    fprintf(fp,"push men\n");
                    fprintf(fp, "call printf\n");
                    fprintf(fp, "add esp,8\n");
                    fprintf(fp, "pop edx\n");
                }else{
                    num_print_bool++;
                    fprintf(fp,"PUSH EDX\n");
                    fprintf(fp,"CMP EAX,1\n");
                    fprintf(fp,"JNZ PRINT_FALSE_%d\n",num_print_bool);
                    fprintf(fp,"PUSH true_msg\n");
                    fprintf(fp,"PUSH men_bool\n");
                    fprintf(fp,"call printf\n");
                    fprintf(fp, "JMP PRINT_FALSE_END_%d\n",num_print_bool);
                    fprintf(fp,"PRINT_FALSE_%d : \n",num_print_bool);
                    fprintf(fp,"PUSH false_msg\n");
                    fprintf(fp,"PUSH men_bool\n");
                    fprintf(fp,"call printf\n");
                    fprintf(fp,"PRINT_FALSE_END_%d : \n",num_print_bool);
                    fprintf(fp,"ADD ESP,8\n");
                    fprintf(fp,"POP EDX\n");
                }
            }
            else
            {
                // complete array
                if(id->entry->type->isStatic == 1)
                {
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
                    fprintf(fp, "PUSH EDX\n");
                    fprintf(fp,"PUSH array_output\n");
                    fprintf(fp,"call printf\n");
                    fprintf(fp,"add esp,4\n");
                    fprintf(fp, "POP EDX\n");
                    fprintf(fp, "XOR ECX, ECX\n");
                    //fprintf(fp, "PUSH EDX\n");
                    fprintf(fp,"PUSH EDX\n");
                    fprintf(fp, "FOR_LOOP_%d : ", num_for);
                    
                    fprintf(fp, "MOV EDX, dword[EBP-%d]\n",(id->entry->offset+K));
                    fprintf(fp, "SUB EDX,ECX\n");
                    fprintf(fp, "mov EAX,dword[SS:EDX-4]\n");
                    
                    fprintf(fp, "push edx\n");
                    fprintf(fp, "push ecx\n");
                    fprintf(fp, "push eax\n");
               
                    if(id->entry->type->element_type == BOOLEAN){
                        fprintf(fp,"PUSH EDX\n");
                        fprintf(fp,"CMP EAX,1\n");
                        fprintf(fp,"JNZ PRINT_FALSE_%d\n",num_print_bool);
                        fprintf(fp,"PUSH true_msg_arr\n");
                        //fprintf(fp,"PUSH men_bool_arr\n");
                        fprintf(fp,"call printf\n");
                        fprintf(fp, "JMP PRINT_FALSE_END_%d\n",num_print_bool);
                        fprintf(fp,"PRINT_FALSE_%d : \n",num_print_bool);
                        fprintf(fp,"PUSH false_msg_arr\n");
                        //fprintf(fp,"PUSH men_bool\n");
                        fprintf(fp,"call printf\n");
                        fprintf(fp,"PRINT_FALSE_END_%d : \n",num_print_bool);
                        fprintf(fp,"ADD ESP,4\n");
                        fprintf(fp,"POP EDX\n");
                    }else{
                        fprintf(fp, "push array_element\n");
                        fprintf(fp, "call printf\n");
                        fprintf(fp, "pop eax\n");
                    }

                    
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
                    fprintf(fp, "PUSH EDX\n");
                    fprintf(fp, "PUSH array_end\n");
                    fprintf(fp, "call printf\n");
                    fprintf(fp, "add esp,4\n");
                    fprintf(fp, "pop edx\n");
                    fprintf(fp, "POP EDX\n");
                }
                else
                {
                    num_for++;
                    fprintf(fp, "PUSH EDX\n");
                    fprintf(fp,"PUSH array_output\n");
                    fprintf(fp,"call printf\n");
                    fprintf(fp,"add esp,4\n");
                    fprintf(fp, "POP EDX\n");

                    fprintf(fp, "XOR ECX, ECX\n");
                    //fprintf(fp, "PUSH EDX\n");
                    fprintf(fp,"PUSH EDX\n");
                    fprintf(fp, "FOR_LOOP_%d : ", num_for);

                    fprintf(fp, "MOV EDX, dword[EBP-%d]\n",(id->entry->offset+K));
                    fprintf(fp, "ADD EDX, dyn_arrays\n");
                    fprintf(fp, "ADD EDX,ECX\n");
                    fprintf(fp, "mov EAX,dword[DS:EDX]\n");
                    
                    fprintf(fp, "push edx\n");
                    fprintf(fp, "push ecx\n");
                    fprintf(fp, "push eax\n");

                    if(id->entry->type->element_type == BOOLEAN){
                        fprintf(fp,"PUSH EDX\n");
                        fprintf(fp,"CMP EAX,1\n");
                        fprintf(fp,"JNZ PRINT_FALSE_%d\n",num_print_bool);
                        fprintf(fp,"PUSH true_msg_arr\n");
                        //fprintf(fp,"PUSH men_bool\n");
                        fprintf(fp,"call printf\n");
                        fprintf(fp, "JMP PRINT_FALSE_END_%d\n",num_print_bool);
                        fprintf(fp,"PRINT_FALSE_%d : \n",num_print_bool);
                        fprintf(fp,"PUSH false_msg_arr\n");
                        //fprintf(fp,"PUSH men_bool\n");
                        fprintf(fp,"call printf\n");
                        fprintf(fp,"PRINT_FALSE_END_%d : \n",num_print_bool);
                        fprintf(fp,"ADD ESP,4\n");
                        fprintf(fp,"POP EDX\n");
                    }else{
                        fprintf(fp, "push array_element\n");
                        fprintf(fp, "call printf\n");
                        fprintf(fp, "pop eax\n");
                    }

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
                    fprintf(fp, "MOV EDI, dword[end + %d]\n", id->entry->type->dyn_index * 4);
                    fprintf(fp, "SUB EDI, dword[start + %d]\n", id->entry->type->dyn_index * 4);
                    fprintf(fp, "INC EDI\n");
                    fprintf(fp, "SHL EDI, 2\n");
                    fprintf(fp, "CMP ECX, EDI\n");
                    fprintf(fp, "JL FOR_LOOP_%d\n", num_for);
                    fprintf(fp, "PUSH EDX\n");
                    fprintf(fp, "PUSH array_end\n");
                    fprintf(fp, "call printf\n");
                    fprintf(fp, "add esp,4\n");
                    fprintf(fp, "pop edx\n");
                    fprintf(fp,"POP EDX\n");
                }
                
            }
        }

        else
        {
           // if (var->node.n->child->node.n->s.N == BOOLCONSTT)
            //{
                if (var->node.n->child->node.l->s.T == TRUE)
                {
                    fprintf(fp,"PUSH EDX\n");
                    fprintf(fp, "push true_msg\n");
                    fprintf(fp, "push men_bool\n");
                    fprintf(fp, "call printf\n");
                    fprintf(fp, "pop eax\n");
                    fprintf(fp, "pop eax\n");
                    fprintf(fp, "POP EDX\n");
                    return;
                }
                else if(var->node.n->child->node.l->s.T == FALSE)
                {
                    fprintf(fp,"PUSH EDX\n");
                    fprintf(fp, "push false_msg\n");
                    fprintf(fp, "push men_bool\n");
                    fprintf(fp, "call printf\n");
                    fprintf(fp, "pop eax\n");
                    fprintf(fp, "pop eax\n");
                    fprintf(fp, "POP EDX\n");
                    return;
                }
            //}
            else if (var->node.n->child->node.l->s.T == NUM || var->node.n->child->node.l->s.T == RNUM)
            {
                fprintf(fp, "mov EAX,%s\n", var->node.n->child->node.l->ti->lexeme);
            }
            else if (var->node.n->child->node.l->s.T == ID && var->node.n->child->node.l->sibling == NULL)
            {
                fprintf(fp, "mov EAX,dword[EBP - %d]\n", (K + var->node.n->child->entry->offset));
            }
            fprintf(fp,"PUSH EDX\n");
            if(var->node.n->child->entry->type->basic_type==BOOLEAN){
                num_print_bool++;
                fprintf(fp,"PUSH EDX\n");
                fprintf(fp,"CMP EAX,1\n");
                fprintf(fp,"JNZ PRINT_FALSE_%d\n",num_print_bool);
                fprintf(fp,"PUSH true_msg\n");
                fprintf(fp,"PUSH men_bool\n");
                fprintf(fp,"call printf\n");
                fprintf(fp, "JMP PRINT_FALSE_END_%d\n",num_print_bool);
                fprintf(fp,"PRINT_FALSE_%d : \n",num_print_bool);
                fprintf(fp,"PUSH false_msg\n");
                fprintf(fp,"PUSH men_bool\n");
                fprintf(fp,"call printf\n");
                fprintf(fp,"PRINT_FALSE_END_%d : \n",num_print_bool);
                fprintf(fp,"ADD ESP,8\n");
                fprintf(fp,"POP EDX\n");
                return;
            }
            fprintf(fp, "push eax\n");
            fprintf(fp, "push men\n");
            fprintf(fp, "call printf\n");
            fprintf(fp, "pop eax\n");
            fprintf(fp, "pop eax\n");
            fprintf(fp, "POP EDX\n");
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

    fprintf(fp,"extern printf\nextern scanf\nextern exit\n");

    fprintf(fp,"section .data\n");
      
    fprintf(fp,"intinputFormat: db \"%%d\",0\nrealinputFormat: db \"%%f\",0\nintvar: times 100 dd 0\nrealvar: times 100 dd 0\nmen:  db \"Output: %%d \", 10,0\nerrorMsg : db \"Array Index Out of Bounds\",10,0\ndynOffset : dd 0\n");

    fprintf(fp, "errorMsg2: db \"Invalid array range for dynamic array\",10,0\n");
    fprintf(fp,"men_bool: db \"Output: %%s \",0\n");
    fprintf(fp,"int_msg: db \"Enter an Integer value : \",10,0 \n");
    fprintf(fp,"true_msg: db \"True\",10,0 \n");
    fprintf(fp,"false_msg: db \"False\",10,0 \n");
    fprintf(fp,"true_msg_arr: db \"True \",0 \n");
    fprintf(fp,"false_msg_arr: db \"False \",0 \n");
    fprintf(fp,"arr_msg: db \"Enter %%d array elements :\",10,0\n");
    fprintf(fp,"array_output: db \"Printing Array Elements: \",10,0\n");
    fprintf(fp,"array_element: db \"%%d \",0\n");
    fprintf(fp, "array_end: db 10,0\n");


    fprintf(fp, "start: times 100 dd 0\n end: times 100 dd 0\n");

    fprintf(fp, "dyn_arrays: times 100 dd 0\n");

    fprintf(fp,"section .text\nglobal main\n");

//    fprintf(fp, "mov ebp, esp\n");

//    fprintf(fp, "push EAX\npush EAX\n");    

    codeGen(fp,head);

    //fprintf(fp, "mov eax,1\nmov ebx,0\nint 80h\n");
    fprintf(fp, "call exit\n");

    fprintf(fp, "END : \n");
    fprintf(fp, "PUSH errorMsg\n");
    fprintf(fp, "call printf\n");
    //fprintf(fp, "mov eax,1\nmov ebx,0\nint 80h\n");
    fprintf(fp,"call exit\n");
    fprintf(fp, "END2 : \n");
    fprintf(fp, "PUSH errorMsg2\n");
    fprintf(fp, "call printf\n");
    //fprintf(fp, "mov eax,1\nmov ebx,0\nint 80h\n");
    fprintf(fp,"call exit\n");


    //fprintf(fp, "ret\n");


}

void codeGen(FILE *fp, tNode *head)
{
    if (head == NULL || head->leafTag == 0)
    {
        return;
    }

    if(head->node.n->s.N == DRIVERMODULE){
        fprintf(fp,"main:\n");

        fprintf(fp, "mov ebp, esp\n");

        fprintf(fp, "push EAX\npush EAX\n");
    }

    if (head->node.n->s.N == ASSIGNMENTSTMT)
    {   
        fprintf(fp,"PUSH EDX\n");
        codeGenAssigment(fp, head->node.n->child);
        fprintf(fp,"POP EDX\n");
    }
    else if (head->node.n->s.N == CONDITIONALSTMT)
    {
        fprintf(fp, "PUSH EDX\n");
        generate_conditional(fp,head);
        fprintf(fp, "POP EDX\n");
        return;
    }
    else if (head->node.n->s.N == ITERATIVESTMT)
    {
        fprintf(fp, "PUSH EDX\n");
        generate_iterative(fp,head);
        fprintf(fp, "POP EDX\n");
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


            while(id_child){
                se* temp = id_child->entry;
                if(temp->type->isStatic == 1){

                    fprintf(fp,"mov EDX, ESP\n");
                    fprintf(fp,"SUB EDX, 4\n");
                    fprintf(fp,"push EDX\n");
                    int s = temp->type->start;
                    int e = temp->type->end;
                    int w = e-s+1;
                    fprintf(fp,"SUB ESP, %d\n",4*w);
                }else{
                    fprintf(fp,"mov EDX, dword[dynOffset]\n");
                    //fprintf(fp,"mov dword[ebp-%d], edx\n", id_child->entry->offset+ K);
                   
                    //fprintf(fp, "SUB ESP, 4\n");
                    fprintf(fp, " push edx\n");
                    int s,e;
                    tNode* range_arrays = dt->node.n->child->node.l->sibling;
                    tNode* l = range_arrays->node.n->child;
                    tNode* r;
                    r = l->node.l->sibling;
                    if(l->node.l->s.T == NUM)
                    {
                        s = l->node.l->ti->value.v1;
                        fprintf(fp, "MOV dword[start + %d], %d\n", temp->type->dyn_index * 4, s);
                    }
                    else{
                        fprintf(fp, "PUSH EAX\n");
                        fprintf(fp, "MOV EAX, dword[EBP-%d]\n", (l->entry->offset + K));
                        fprintf(fp, "MOV dword[start + %d], EAX\n", temp->type->dyn_index * 4);
                        fprintf(fp, "POP EAX\n");
                    }
                    if(r->node.l->s.T == NUM)
                    {
                        e = r->node.l->ti->value.v1;
                        fprintf(fp, "MOV dword[end + %d], %d\n", temp->type->dyn_index * 4, e);
                    }
                    else
                    {
                        fprintf(fp, "PUSH EAX\n");
                        fprintf(fp, "MOV EAX, dword[EBP-%d]\n", (r->entry->offset + K));
                        fprintf(fp, "MOV dword[end + %d], EAX\n", temp->type->dyn_index * 4);
                        fprintf(fp, "POP EAX\n");
                    }
                    fprintf(fp, "PUSH EAX\n");
                    fprintf(fp, "MOV EAX, dword[end + %d]\n", temp->type->dyn_index * 4);
                    fprintf(fp, "SUB EAX, dword[start + %d]\n", temp->type->dyn_index * 4);
                    fprintf(fp, "CMP EAX, 0\n");
                    fprintf(fp, "JL END2\n");
                    fprintf(fp, "INC EAX\n");
                    fprintf(fp, "SHL EAX, 2\n");
                    fprintf(fp, "ADD dword[dynOffset], EAX\n");
                    fprintf(fp, "POP EAX\n");
                    
                }
                id_child = id_child->node.l->sibling;
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