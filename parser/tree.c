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
            nlNode *tempnln = (ptn->node).n;
            tNode *temptNode = tempnln->child;
            if (temptNode == NULL)
            {
                tempnln->child = tn;
            }
            else
            {
                tempnln = (temptNode->node).n;
                while (tempnln->sibling)
                {
                    temptNode = tempnln->sibling;
                    if (temptNode->leafTag == 0)
                    {
                        tempnln = (temptNode->node).l;
                    }
                    else
                    {
                        tempnln = (temptNode->node).n;
                    }
                }
                tempnln->sibling = tn;
            }
        }
    }
}