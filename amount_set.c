//
// Created by Evyatar Sivan on 24/11/2019.
//

#include "amount_set.h"
//#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define NULL_POINTER -1

typedef struct node {
    ASElement data;
    int amount; //!!!can be double
    struct node *next;
} *Node;

static AmountSetResult addAllOrDestroy(AmountSet target, AmountSet source);

static AmountSetResult addOrDestroy(AmountSet target, ASElement element);

static Node find(AmountSet set, ASElement element);

/*The struct that will hold the AmountSet fields and specific functions,
 * as well as the pointer to first element in linked list*/
struct AmountSet_t {
    Node elements;
    int nextElement;
    int size;
//    int iterator; ?????
    CopyASElement copyElm;
    FreeASElement freeElm;
    CompareASElements compareElm;
};


int main() {

    AmountSet set = malloc(sizeof(*set));
    if (set == NULL) {
        return 0;
    }

    Node headNode = malloc(sizeof(*headNode));
    if (headNode == NULL) {
        free(set);
        return 0;
    }
    headNode->amount=4;
    set->elements=headNode;


    free(headNode);
    free(set);
    return 0;
}


AmountSet asCreate(CopyASElement copyElement,
                   FreeASElement freeElement,
                   CompareASElements compareElements) {
    if (copyElement == NULL || freeElement == NULL || compareElements == NULL) {
        return NULL;
    }

    AmountSet set = malloc(sizeof(*set));
    if (set == NULL) {
        return NULL;
    }

    Node headNode = malloc(sizeof(*headNode));
    if (headNode == NULL) {
        free(set);
        return NULL;
    }
    headNode->next = NULL;
    headNode->amount=0;
    set->elements = headNode;
//    set->iterator = 0;
    set->nextElement = 0;
    set->size = 0;
    set->compareElm = compareElements;
    set->copyElm = copyElement;
    set->freeElm = freeElement;

    return set;
}

void asDestroy(AmountSet set) {
    if (set == NULL) return;

    asClear(set);
    free(set);
}

AmountSetResult asClear(AmountSet set) {

    if (set == NULL) return AS_NULL_ARGUMENT;

    Node nextNode = set->elements;

    while (nextNode) {
        Node toDelete = nextNode;
        nextNode = nextNode->next;
        set->freeElm(toDelete->data);
        free(toDelete);
    }

    return AS_SUCCESS;
}

AmountSet asCopy(AmountSet set) {
    if (set == NULL) return NULL;

    AmountSet newSet = asCreate(set->copyElm, set->freeElm, set->compareElm);
    if (newSet == NULL) return NULL;
    /*=======================add amount and size===========================*/
    if (addAllOrDestroy(newSet, set) == AS_OUT_OF_MEMORY) return NULL;

//    newSet->iterator = set->iterator ??????
    return newSet;
}

static AmountSetResult addAllOrDestroy(AmountSet target, AmountSet source) {
    Node nextNode = source->elements;
    printf("%p %p", nextNode, source->elements);
    while (nextNode) {
        if (addOrDestroy(target, nextNode->data) == AS_OUT_OF_MEMORY)
            return AS_OUT_OF_MEMORY;

        nextNode = nextNode->next;
    }

    return AS_SUCCESS;
}

static AmountSetResult addOrDestroy(AmountSet set, ASElement element) {

    AmountSetResult result = asRegister(set, element);
    if (result == AS_OUT_OF_MEMORY) asDestroy(set);
    return result;

}

AmountSetResult asRegister(AmountSet set, ASElement element) {
    if (set == NULL || element == NULL) return AS_NULL_ARGUMENT;

    if (asContains(set, element)) return AS_SUCCESS;

    Node newNode = malloc(sizeof(*newNode));
    if (newNode == NULL) {
        return AS_OUT_OF_MEMORY;
    }

    newNode->data = set->copyElm(element);
    if (newNode->data == NULL) return AS_OUT_OF_MEMORY;
    /*=======================add order by compare===========================*/
    newNode->next = set->elements;
    set->elements = newNode;
    set->size++;

}

bool asContains(AmountSet set, ASElement element) {
    Node foundNode = find(set, element);
    if(foundNode == NULL) return false;
    return true;
}

static Node find(AmountSet set, ASElement element) {
    assert(set != NULL && element != NULL);

    Node nextNode = set->elements;
    while (nextNode) {
        if (!set->compareElm(nextNode->data, element)) return nextNode;
    }

    return NULL;
}

int asGetSize(AmountSet set) {
    if (set == NULL) return NULL_POINTER;

    return set->size;
}

AmountSetResult asGetAmount
        (AmountSet set, ASElement element, double *outAmount) {
    if(set == NULL || element == NULL || outAmount == NULL)
        return AS_NULL_ARGUMENT;

    Node foundNode = find(set, element);
    if(foundNode == NULL) return AS_ITEM_DOES_NOT_EXIST;

    outAmount = foundNode->amount;
    return AS_SUCCESS;

}

AmountSetResult asChangeAmount(AmountSet set, ASElement element, const double amount) {

}


//ASElement asGetFirst(AmountSet set) {
//    return set->elements;
//}
//
//ASElement asGetNext(AmountSet set) {
//    return
//}

