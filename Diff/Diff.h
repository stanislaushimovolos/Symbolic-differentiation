//
// Created by Tom on 22.11.2017.
//

#pragma  once

#include "../Tree_t/Tree.h"

const char MainVariable[] = "x";

const char inputFilename[] = "../base/base.txt";
const char outFilename[] = "../dumps/base.txt";
const char outFilenameForDotDump[] = "../picture/dotDump.txt";
const char outFilenameForDotPrint[] = "../picture/dotPrint.txt";

int simpleTree (Node *node);

char *getBufferFromFileGetSzOfBuf (const char *_inputFileName, size_t *sizeOfBuffer = NULL);

char contentAnalyze (Node *node, const char *const currValue);

Node *diffRec (const Node *const node, const char *const currValue, Tree *FinalTree);

Node *diffMain (const Tree *const BegTree, Tree *FinalTree, const char *const currValue);

Node *createNode (char type, int value, Tree *FinalTree);

Node *createNode (char type, char operator__, Node *LeftCopy, Node *RightCopy, Tree *FinalTree);

Node *createNode (const char type, char operator__, Tree *FinalTree);

Node *createNode (const char type, const char *const func, Tree *FinalTree);

Node *complicatedDiff (Node *mainNode, Tree *FinalTree, const char *const currValue,
                       Node *(diff) (Node *node, Tree *FilTree, const char *const curValue));

Node *lnDiff (Node *node, Tree *FinalTree, const char *const currValue);

Node *sinDiff (Node *node, Tree *FinalTree, const char *const currValue);

Node *cosDiff (Node *node, Tree *FinalTree, const char *const currValue);

Node *addDiff (const Node *const node, Tree *FinalTree, const char *const currValue);

Node *subDiff (const Node *const node, Tree *FinalTree, const char *const currValue);

Node *mulDiff (const Node *const node, Tree *FinalTree, const char *const currValue);

Node *divDiff (const Node *const node, Tree *FinalTree, const char *const currValue);

Node *expoDiff (const Node *const node, Tree *FinalTree, const char *const currValue);

