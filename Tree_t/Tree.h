//
// Created by Tom on 12.11.2017.
//
#pragma once

#define SEPARATOR '\"'

#include <iostream>

struct Node;

struct Tree
{
    Node *root;
    int nodeAmount;
};

struct Node
{
    Tree *myTree;
    Node *Left;
    Node *Right;
    Node *Parent;
    int type;
    char *content;
};


int nodeConstruct(Node **node);

int nodeSetName(Node *node, char *content);

int treeConstruct(Tree *tree);

int printTreeFileRec(const Node *const Node, const int *const NodeAmount, int *NodeCounterRec, FILE *outBaseFile1);

int printTreeFile(const Tree *const tree, const char *outFileName);

int dumpRecNode(const Node *const n, int *nodeCounter, FILE *outPictureFile);

int printRecNode(const Node *const n, int *NodeCounter, FILE *outPictureFile);

void connectRight(Node *mainNode, Node *rightNode);

void connectLeft(Node *mainNode, Node *leftNode);

int dumpTreePicture(const Tree *const tree, const char *outFileName);

int printTree(const Tree *const tree, const char *outFileName);

void destructNode(Node *node);

void destructTree(Tree *tree);

void destructNodeRec(Node *node);

int nodeCount(Node *node);

void treeVisitorInf(Node *node, int
nodeFunc(Node
         *Node));

Node *TreeCopy(const Node *node, Tree *newTree);

Node *createNode(const char type, const char *content, Tree *FinalTree);
