#include "tree.h"

tNode *head = NULL;

tNode *createtNode(rhsNode *rh, tokenInfo *ti)
{
    tNode *tn = malloc(sizeof(tNode));
    if (rh->tag == 0)
    {
        //printf("Terminal\n");
        lNode *ln = malloc(sizeof(lNode));
        ln->ti = ti;
        ln->sibling = NULL;
        (tn->node).l = ln;
        tn->leafTag = 0;
    }
    else
    {
        //printf("Before Non Term\n");
        nlNode *nln = malloc(sizeof(nlNode));
        nln->s = rh->S;
        nln->child = NULL;
        nln->sibling = NULL;
        (tn->node).n = nln;
        tn->leafTag = 1;
        //printf("After Non Term\n");
    }
    return tn;
}

void inserttNode(tNode *ptn, tNode *tn)
{
    if (head == NULL)
    {
        head = tn;
        return;
    }
    else
    {
        tNode *temptn;
        if (ptn->leafTag == 0)
        {
            return;
        }
        else
        {
            treeNode *tempnln;
            tempnln->n = (ptn->node).n;
            tNode *temptNode = tempnln->n->child;
            if (temptNode == NULL)
            {
                tempnln->n->child = tn;
            }
            else
            {
                if(temptNode->leafTag)
                    tempnln->n = (temptNode->node).n;
                else{
                    tempnln->l = (temptNode->node).l;
                }
                while (tempnln->n->sibling)
                {
                    temptNode = tempnln->n->sibling;
                    if (temptNode->leafTag == 0)
                    {
                        tempnln->l = (temptNode->node).l;
                    }
                    else
                    {
                        tempnln->n = (temptNode->node).n;
                    }
                }
                tempnln->n->sibling = tn;
            }
        }
    }
}



/*int main()
{
    Symbol s;
    s.N = PROGRAM;
    int tag = 1;
    rhsNode *rh = malloc(sizeof(rh));
    rh->S = s;
    rh->tag = tag;
    rh->next = NULL;
    tokenInfo *ti = createToken(PROGRAM, "program", 0);
    tNode *tn = createtNode(rh, ti);
    inserttNode(NULL, tn);
    rh->tag = 0;
    tNode *tn1 = createtNode(rh, ti);
    inserttNode(head, tn1);
    printf("%d\n", head->node.n->child->leafTag);
    return 0;
}*/