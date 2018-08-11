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
    int eventFlag;
    int nodeAmount;
};

struct Node
{
    int type;
    Node *left;
    Node *right;
    Node *parent;
    Tree *myTree;
    char *content;
};

int countNodes(Node *node);

int constructNode(Node **node);

int treeConstruct(Tree *tree);

int nodeSetName(Node *node, const char *content);

int printTree(const Tree *tree, const char *outFileName);

int dumpTreePicture(const Tree *tree, const char *outFileName);

int dumpRecNode(const Node *n, int *nodeCounter, FILE *outPictureFile);

int printRecNode(const Node *n, int *NodeCounter, FILE *outPictureFile);

void destructNode(Node *node);

void destructTree(Tree *tree);

void destructNodeRec(Node *node);

void visitTree(Node *node, int func(Node *node));

void connectLeft(Node *mainNode, Node *leftNode);

void connectRight(Node *mainNode, Node *rightNode);

Node *copyTree(const Node *node, Tree *newTree);

Node *createNode(int type, const char *content, Tree *newTree);


