//
// Created by Evyatar Sivan on 24/11/2019.
//

#ifndef HW1_LINKED_LIST_H
#define HW1_LINKED_LIST_H


typedef struct node {
    void* data;
    unsigned int id;
    struct node* next;
} *Node;

static Node find(AmountSet set, unsigned int id);

Node createNode(ASElement element, unsigned int id);

Node registerNode(AmountSet set, Node node);


void destroyNode(Node node);

void destroyListFrom(Node node);

void printNodes(AmountSet set);

#endif //HW1_LINKED_LIST_H
