#include "tree.h"

tNode *head = NULL;

tNode *createtNode(rhsNode *rh, tokenInfo *ti)
{
    tNode *tn = malloc(sizeof(tNode));
    if (rh->tag == 0)
    {
        lNode *ln = malloc(sizeof(lNode));
        ln->ti = ti;
        ln->sibling = NULL;
        (tn->node).l = ln;
        tn->leafTag = 0;
    }
    else
    {
        nlNode *nln = malloc(sizeof(nlNode));
        nln->s = rh->S;
        nln->child = NULL;
        nln->sibling = NULL;
        (tn->node).n = nln;
        tn->leafTag = 1;
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
                tempnln->n = (temptNode->node).n;
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

int main()
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
    printf("%d\n", head->leafTag);
    return 0;
}