////
//// Created by Evyatar Sivan on 24/11/2019.
////
//
//#include "linked_list.h"
//#include <stdlib.h>
//#include <stdio.h>
//
//void printNodes(void* head) {
//    Node ptr = head;
//    while(ptr) {
//        printf("%d",*(int*)(ptr->data));
//        ptr = ptr->next;
//    }
//}
//
//Node createNode(void* element, unsigned int id) {
//    Node newNode = (Node)malloc(sizeof(*newNode));
//    if(newNode == NULL) return NULL;
//
//    newNode->next = NULL;
//    newNode->id = id;
//    newNode->data = element;
//
//    return newNode;
//}
//
//void destroyNodeById(AmountSet set, unsigned int id) {
//    Node node = find(set, id);
//    free(node);
//}
//
//void destroyListFrom(Node head) {
//    while(head) {
//        Node toDelete = head;
//        head = head->next;
//        set->fr
//        free(toDelete);
//    }
//}
//
//Node registerNode(AmountSet set, Node node) {
//    node->next = set->elements;
//    set->elements = node;
//    return node;
//}
//
//
//static Node find(AmountSet set, unsigned int id) {
//    Node ptr = set->elements;
//    while(ptr) {
//        if(ptr->id==id) return ptr;
//        ptr = ptr->next;
//    }
//    return NULL;
//}
//
//void destroyNodeById(AmountSet set, unsigned int id) {
//    Node node = find(set, id);
//    free(node);
//}
//
//void destroyListFrom(Node head) {
//    while(head) {
//        Node toDelete = head;
//        head = head->next;
//        free(toDelete);
//    }
//}