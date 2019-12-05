//
// Created by Yoav on 28/11/2019.
//
#ifndef AMOUNT_SET2_C_
#define AMOUNT_SET2_C_
#include "amount_set.h"
#include <stdlib.h>
#endif

typedef struct node{
    ASElement Element;
    double amount;
    struct node* next;
    struct node* previous;
} *Node;

 struct AmountSet_t{
    Node header;
    Node itr;
    int size;
    CopyASElement copyElement;
    FreeASElement freeElement;
    CompareASElements compareElement;
};

AmountSet asCreate(CopyASElement copyElement, FreeASElement freeElement, CompareASElements compareElements){
    if (copyElement==NULL||freeElement==NULL||compareElements==NULL)
        return NULL;
    AmountSet set = malloc(sizeof(*set));
    if (set==NULL)
        return NULL;
    set->size=0;
    set->header=NULL;
    set->itr=NULL;
    set->freeElement=freeElement;
    set->compareElement=compareElements;
    set->copyElement=copyElement;
    return set;
}
void asDestroy(AmountSet set){
    if (set==NULL)
        return;
    /** set->itr=set->header;
     if(set->header!=NULL) {
         while (set->header->next != NULL) {
             set->itr = set->header;
             set->header = set->header->next;
             set->header->previous=NULL;
             set->freeElement(set->itr->Element);
             Node x=set->itr;
             free(set->itr);
         }
         set->freeElement(set->header->Element);
         free(set->header);
     }*/ asClear(set);
    free(set);
}
AmountSet asCopy(AmountSet set) {
    if(set==NULL)
        return NULL;
    AmountSet new_set = asCreate(set->copyElement, set->freeElement, set->compareElement);
    if(new_set==NULL)
        return AS_OUT_OF_MEMORY;
    set->itr = set->header;
    while (set->itr!=NULL){
        if(asRegister(new_set,set->itr->Element)==AS_OUT_OF_MEMORY){
            asDestroy(new_set);
            return NULL;
        }
        asChangeAmount(new_set,set->itr->Element,set->itr->amount);
        set->itr=set->itr->next;
    }
    return new_set;
}
bool asContains(AmountSet set, ASElement element) {
    if(set==NULL||element==NULL)
        return false;
    set->itr = set->header;
    while(set->itr!=NULL) {
        if (set->compareElement(element, set->itr->Element )==0)
            return true;
        set->itr=set->itr->next;
    }
    return false;
}
static AmountSetResult addInitialNode(AmountSet set,Node newNode){
    set->header=newNode;
    return AS_SUCCESS;
}
static AmountSetResult addFirst(AmountSet set, Node newNode){
    Node temp;
    temp=set->header;
    set->header=newNode;
    set->header->next=temp;
    temp->previous=set->header;
    return AS_SUCCESS;
}
static  AmountSetResult addInTheMiddle(AmountSet set,Node newNode){
    set->itr=set->header;
    while (set->itr->next!=NULL){
        if (set->compareElement(set->itr->next->Element,newNode->Element)>0){
            newNode->next=set->itr->next;
            newNode->previous=set->itr;
            newNode->next->previous=newNode;
            set->itr->next=newNode;
            return AS_SUCCESS;
        }
        set->itr=set->itr->next;
    }
    set->itr->next=newNode;
    newNode->previous=set->itr;
    return AS_SUCCESS;
}
AmountSetResult asRegister(AmountSet set, ASElement element) {
    if (set == NULL || element == NULL)
        return AS_NULL_ARGUMENT;
    if (asContains(set, element))
        return AS_ITEM_ALREADY_EXISTS;
    Node newNode=malloc(sizeof(struct node));
    if(newNode==NULL)
        return AS_OUT_OF_MEMORY;
    newNode->Element=set->copyElement(element);
    newNode->amount=0;
    newNode->next=NULL;
    newNode->previous=NULL;
    set->size++;
    if (set->header==NULL)
        return addInitialNode(set,newNode);
    if(set->compareElement(set->header->Element,newNode->Element)>0)
        return addFirst(set,newNode);
    else
        return addInTheMiddle(set,newNode);
  /**  if (set->header==NULL){
        set->header=newNode;
        return AS_SUCCESS;
    }
    Node temp;
    if (set->compareElement(set->header->Element,newNode->Element)>0){
        temp=set->header;
        set->header=newNode;
        set->header->next=temp;
        temp->previous=set->header;
        return AS_SUCCESS;
    }
    set->itr=set->header;
    while (set->itr->next!=NULL){
        if (set->compareElement(set->itr->next->Element,newNode->Element)>0){
            newNode->next=set->itr->next;
            newNode->previous=set->itr;
            newNode->next->previous=newNode;
            set->itr->next=newNode;
            return AS_SUCCESS;
        }
        set->itr=set->itr->next;
    }
    set->itr->next=newNode;
    newNode->previous=set->itr;
    return AS_SUCCESS; */
}




AmountSetResult asGetAmount(AmountSet set, ASElement element, double *outAmount){
    if(set==NULL||element==NULL||outAmount==NULL)
        return AS_NULL_ARGUMENT;
    if(!asContains(set,element))
        return  AS_ITEM_DOES_NOT_EXIST;
    *outAmount=set->itr->amount;
    return AS_SUCCESS;
}
int asGetSize(AmountSet set){
    if (set==NULL)
        return -1;
    return set->size;
}
AmountSetResult asChangeAmount(AmountSet set, ASElement element, const double amount){
    if(set==NULL||element==NULL)
        return AS_NULL_ARGUMENT;
    if (!asContains(set,element))
        return AS_ITEM_DOES_NOT_EXIST;
    if(set->itr->amount+amount<0)
        return AS_INSUFFICIENT_AMOUNT;
    /** iterator is placed on the correct node by asContains*/
    set->itr->amount+=amount;
    return  AS_SUCCESS;
}
AmountSetResult asDelete(AmountSet set, ASElement element){
    if (set==NULL||element==NULL)
        return AS_NULL_ARGUMENT;
    if(!asContains(set,element))
        return AS_ITEM_DOES_NOT_EXIST;
    set->freeElement(set->itr->Element);
    if (set->itr->previous==NULL){
        set->header=set->header->next;
        free(set->itr);
        set->size--;
        return AS_SUCCESS;
    }
    if (set->itr->next==NULL){
        set->itr->previous->next=NULL;
        free(set->itr);
        set->size--;
        return AS_SUCCESS;
    }
    set->itr->previous->next=set->itr->next;
    set->itr->next->previous=set->itr->previous;
    free(set->itr);
    set->size--;
    return AS_SUCCESS;
}
AmountSetResult asClear(AmountSet set){
    if (set==NULL)
        return AS_NULL_ARGUMENT;
    while(set->itr){
        Node toDelete=set->itr;
        set->itr=set->itr->next;
        set->freeElement(toDelete);
        free(toDelete);
    }

/**    for (int i = 1; i <= set->size; ++i) {
        set->itr=set->header;
        set->header=set->header->next;
        set->freeElement(set->itr->Element);
        free(set->itr);
    }*/
    set->header=NULL;
    set->itr=NULL;/** 5555555555555555555555555555555555555555555555555555555555555*/
    set->size=0;
    return AS_SUCCESS;
}
ASElement asGetFirst(AmountSet set){
    if(set==NULL || set->header==NULL)
        return NULL;
    set->itr=set->header;
    return set->itr->Element;
}
ASElement asGetNext(AmountSet set){
    if(set==NULL||set->itr==NULL||set->itr->next==NULL)
        return NULL;
    set->itr=set->itr->next;
    return set->itr->Element;

}