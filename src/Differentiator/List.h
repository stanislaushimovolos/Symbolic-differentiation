/*!
 * @file List.h
 * @brief Simple implementation of doubly connected list.
 * @author Stanislau Shimovolos
 * @date 2018-8-15
 */

#ifndef DIFF3_0_LIST_H
#define DIFF3_0_LIST_H


typedef struct List
{
    struct listNode *current;
    struct listNode *start;
    int nodeCounter;
} List;


typedef struct listNode
{
    struct listNode *next;
    struct listNode *prev;
    Node *data;
    List *parent;
} listNode;


int dumpList(List *lst);

int constructList(List *lst);

int eraseNode(listNode *node);

int push(Node *node, List *lst);

int destructList(listNode *node);




#endif //DIFF3_0_LIST_H
