/* LAVANYA SONI - 2017A7PS0151P
    KUSHAGRA AGRAWAL - 2017A7PS0107P
    SIDDHANT KHANDELWAL - 2017A7PS0127P
    GROUP - 11
*/

#include "stack.h"

stack st;

int isEmpty()
{
    if (st.size == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

stackNode *createStackNode(tNode *tn)
{
    stackNode *sn = malloc(sizeof(stackNode));
    sn->tn = tn;
    sn->next = NULL;
    return sn;
}

int initializeStack(tNode *tn)
{
    stackNode *sn = createStackNode(tn);
    st.top = sn;
    st.size = 1;
}

void push(tNode *tn)
{
    stackNode *sn = createStackNode(tn);
    sn->next = st.top;
    st.top = sn;
    (st.size)++;
}

stackNode *pop()
{
    stackNode *sn = st.top;
    st.top = sn->next;
    (st.size)--;
    return sn;
}

void print_stack()
{

    stackNode *temp = st.top;
    while (temp)
    {

        printf("%d\n", temp->tn->node.n->s.N);
        temp = temp->next;
    }
}
