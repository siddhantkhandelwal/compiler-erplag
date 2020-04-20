#include "tree.h"
#include <stdlib.h>

tNode* head = NULL;

tNode *createtNode(rhsNode *rh, tokenInfo *ti)
{

    tNode *tn = malloc(sizeof(tNode));
    if (rh->tag == 0)
    {
        //printf("Terminal\n");
        lNode *ln = malloc(sizeof(lNode));
        ln->ti = ti;
        ln->sibling = NULL;
        ln->s = rh->S;
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

            tNode* prev = NULL;


            if(ptn->leafTag == 0){

                return;
            }else{
                temptn = ptn->node.n->child;

                if(temptn==NULL){
                    ptn->node.n->child = tn;
                    return;
                }
            }

            prev = temptn;

            int tg = temptn->leafTag;

            while(1){

                if(tg==0){
                    if(temptn->node.l->sibling==NULL){
                        temptn->node.l->sibling = tn;
                        return;
                    }

                    temptn = temptn->node.l->sibling;
                    tg = temptn->leafTag;
                    continue;

                }else{

                     if(temptn->node.n->sibling==NULL){
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