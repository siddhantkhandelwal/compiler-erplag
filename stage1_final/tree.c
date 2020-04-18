/* LAVANYA SONI - 2017A7PS0151P
    KUSHAGRA AGRAWAL - 2017A7PS0107P
    SIDDHANT KHANDELWAL - 2017A7PS0127P
    GROUP - 11
*/

#include "tree.h"
#include <stdlib.h>

tNode *head = NULL;

tNode *createtNode(rhsNode *rh, tokenInfo *ti)
{
    tNode *tn = malloc(sizeof(tNode));
    if (rh->tag == 0)
    {
        lNode *ln = malloc(sizeof(lNode));
        ln->ti = ti;
        ln->sibling = NULL;
        ln->s = rh->S;
        (tn->node).l = ln;
        tn->leafTag = 0;
        tn->scope_start_line = -1;
        tn->scope_end_line = -1;
        tn->scope_marker = 0;
    }
    else
    {
        nlNode *nln = malloc(sizeof(nlNode));
        nln->s = rh->S;
        nln->child = NULL;
        nln->sibling = NULL;
        (tn->node).n = nln;
        tn->leafTag = 1;
        tn->node.n->is_operator = 0;
        tn->scope_start_line = -1;
        tn->scope_end_line = -1;
        tn->scope_marker = 0;
    }
    return tn;
}

void inserttNode(tNode *ptn, tNode *tn)
{
    if (head == NULL)
    {
        head = tn;
        head->parent = NULL;
        return;
    }
    else
    {
        tn->parent = ptn;
        tNode *temptn = NULL;
        if (ptn->leafTag == 0)
        {
            return;
        }
        else
        {

            tNode *prev = NULL;

            if (ptn->leafTag == 0)
            {

                return;
            }
            else
            {
                temptn = ptn->node.n->child;

                if (temptn == NULL)
                {
                    ptn->node.n->child = tn;
                    return;
                }
            }

            prev = temptn;

            int tg = temptn->leafTag;

            while (1)
            {

                if (tg == 0)
                {
                    if (temptn->node.l->sibling == NULL)
                    {
                        temptn->node.l->sibling = tn;
                        return;
                    }

                    temptn = temptn->node.l->sibling;
                    tg = temptn->leafTag;
                    continue;
                }
                else
                {

                    if (temptn->node.n->sibling == NULL)
                    {
                        temptn->node.n->sibling = tn;
                        return;
                    }

                    temptn = temptn->node.n->sibling;
                    tg = temptn->leafTag;
                    continue;
                }
            }
        }
    }
}
