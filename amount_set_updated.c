////
//// Created by Evyatar Sivan on 05/12/2019.
////
//
////
//// Created by Yoav on 28/11/2019.
////
//#ifndef AMOUNT_SET2_C_
//#define AMOUNT_SET2_C_
//#include "amount_set.h"
//#include <stdlib.h>
//#endif
//
////Consider ASElement as node
//typedef struct node{
//    ASElement Element;
//    double amount;
//    struct node* next;
//    struct node* previous;
//} ASElement;
//
//typedef struct AmountSet_t{
//    Node header;
//    //use iterator not itr
//    Node itr;
//    int size;
//    CopyASElement copyElement;
//    FreeASElement freeElement;
//    CompareASElements compareElement;
//} *AmountSet;
//
//AmountSet asCreate(CopyASElement copyElement, FreeASElement freeElement, CompareASElements compareElements){
//    if (copyElement==NULL||freeElement==NULL||compareElements==NULL)
//        return NULL;
//    AmountSet set = malloc(sizeof(*set));
//    if (set==NULL)
//        return NULL;
//    set->size=0;
//    set->header=NULL;
//    set->itr=NULL;
//    set->freeElement=freeElement;
//    set->compareElement=compareElements;
//    set->copyElement=copyElement;
//    return set;
//}
//
///*
// * void asDestroy(AmountSet set) {
//    if (set == NULL) return;
//
//    asClear(set);
//    free(set);
//}
// * */
//void asDestroy(AmountSet set){
//    if (set==NULL)
//        return;
//    set->itr=set->header;
//    if(set->header!=NULL) {
//        //duplicated code - use asClear
//        while (set->header->next != NULL) {
//            //duplicated line
//            set->itr = set->header;
//            set->header = set->header->next;
//            set->freeElement(set->itr->Element);
//            //whats this line for?
//            Node x=set->itr;
//            free(set->itr);
//        }
//        set->freeElement(set->header->Element);
//        free(set->header);
//    }
//    free(set);
//}
//
//
//AmountSet asCopy(AmountSet set) {
//    if(set==NULL)
//        return NULL;
//    AmountSet new_set = asCreate(set->copyElement, set->freeElement, set->compareElement);
//    set->itr = set->header;
//    new_set->itr = new_set->header;
//    while (set->itr!=NULL){
//        asRegister(new_set,set->itr->Element);
//        asChangeAmount(new_set,set->itr->Element,set->itr->amount);
//        set->itr=set->itr->next;
//    }
//    return new_set;
//}
//bool asContains(AmountSet set, ASElement element) {
//    if(set==NULL||element==NULL)
//        return false;
//    set->itr = set->header;
//    while(set->itr!=NULL) {
//        if (set->compareElement(element, set->itr->Element )==0)
//            return true;
//        set->itr=set->itr->next;
//    }
//    return false;
//}
//AmountSetResult asRegister(AmountSet set, ASElement element) {
//    if (set == NULL || element == NULL)
//        return AS_NULL_ARGUMENT;
//    if (asContains(set, element))
//        return AS_ITEM_ALREADY_EXISTS;
//    Node newNode=malloc(sizeof(struct node));
//    newNode->Element=set->copyElement(element);
//    newNode->amount=0;
//    newNode->next=NULL;
//    newNode->previous=NULL;
//    if (set->header==NULL){
//        set->header=newNode;
//        set->size++;
//        return AS_SUCCESS;
//    }
//    Node temp;
//    if (set->compareElement(set->header->Element,newNode->Element)>0){
//        temp=set->header;
//        set->header=newNode;
//        set->header->next=temp;
//        temp->previous=set->header;
//        set->size++;
//        return AS_SUCCESS;
//    }
//    set->itr=set->header;
//    while (set->itr->next!=NULL){
//        if (set->compareElement(set->itr->next->Element,newNode->Element)>0){
//            newNode->next=set->itr->next;
//            newNode->previous=set->itr;
//            newNode->next->previous=newNode;
//            set->itr->next=newNode;
//            /**  temp=set->itr.pre;
//              temp->next=newNode;
//              newNode->previous=temp;
//              newNode->next=set->itr;
//              set->itr->previous=newNode; */
//            set->size++;
//            return AS_SUCCESS;
//        }
//        set->itr=set->itr->next;
//    }
//    set->itr->next=newNode;
//    newNode->previous=set->itr;
//    set->size++;
//    return AS_SUCCESS;
//}
//AmountSetResult asGetAmount(AmountSet set, ASElement element, double *outAmount){
//    if(set==NULL||element==NULL||outAmount==NULL)
//        return AS_NULL_ARGUMENT;
//    if(!asContains(set,element))
//        return  AS_ITEM_DOES_NOT_EXIST;
//    *outAmount=set->itr->amount;
//    return AS_SUCCESS;
//}
//int asGetSize(AmountSet set){
//    if (set==NULL)
//        return -1;
//    return set->size;
//}
//AmountSetResult asChangeAmount(AmountSet set, ASElement element, const double amount){
//    if(set==NULL||element==NULL)
//        return AS_NULL_ARGUMENT;
//    if (!asContains(set,element))
//        return AS_ITEM_DOES_NOT_EXIST;
//    if(set->itr->amount+amount<0)
//        return AS_INSUFFICIENT_AMOUNT;
//    set->itr->amount+=amount;
//    return  AS_SUCCESS;
//}
//AmountSetResult asDelete(AmountSet set, ASElement element){
//    if (set==NULL||element==NULL)
//        return AS_NULL_ARGUMENT;
//    if(!asContains(set,element))
//        return AS_ITEM_DOES_NOT_EXIST;
//    set->freeElement(set->itr->Element);
//    if (set->itr->previous==NULL){
//        set->header=set->header->next;
//        free(set->itr);
//        set->size--;
//        return AS_SUCCESS;
//    }
//    if (set->itr->next==NULL){
//        set->itr->previous->next=NULL;
//        free(set->itr);
//        set->size--;
//        return AS_SUCCESS;
//    }
//    set->itr->previous->next=set->itr->next;
//    set->itr->next->previous=set->itr->previous;
//    set->size--;
//    return AS_SUCCESS;
//}
//
////why using a for loop? consider this:
///*
// * AmountSetResult asClear(AmountSet set) {
//
//    if (set == NULL) return AS_NULL_ARGUMENT;
//
//    set->iterator = set->header;
//
//    while (iterator) {
//        Node toDelete = iterator;
//        iterator = iterator->next;
//        set->freeElm(toDelete->data);
//        free(toDelete);
//    }
//
//    set->header=NULL;
//    set->size=0;
//
//    return AS_SUCCESS;
//}
// * */
//AmountSetResult asClear(AmountSet set){
//    if (set==NULL)
//        return AS_NULL_ARGUMENT;
//    for (int i = 1; i <= set->size; ++i) {
//        set->itr=set->header;
//        set->header=set->header->next;
//        set->freeElement(set->itr->Element);
//        free(set->itr);
//    }
//    //unnecessary line
//    set->header=NULL;
//    set->itr=NULL;
//    set->size=0;
//    return AS_SUCCESS;
//}
//
//
//ASElement asGetFirst(AmountSet set){
//    if(set==NULL || set->header==NULL)
//        return NULL;
//    set->itr=set->header;
//    return set->itr->Element;
//}
//ASElement asGetNext(AmountSet set){
//    if(set==NULL||set->itr==NULL||set->itr->next==NULL)
//        return NULL;
//    set->itr=set->itr->next;
//    return set->itr->Element;
//
//}