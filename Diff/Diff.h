//
// Created by Tom on 22.11.2017.
//

#pragma  once

#include "../Tree_t/Tree.h"

enum
{
	number, operator_, curVariable, charConst

};

struct Elem
{
	char type;
	char *content;
};

const char MainVariable[] = "x";

const char inputFilename[] = "../base/base.txt";
const char outFilename[] = "../dumps/base.txt";
const char outFilenameForDotDump[] = "../picture/dotDump.txt";
const char outFilenameForDotPrint[] = "../picture/dotPrint.txt";

char *getBufferFromFileGetSzOfBuf (const char *_inputFileName, size_t *sizeOfBuffer = NULL);

char contentAnalyze (Node *node, const char *const currValue);

Node *diffRec (const Node *const node, const char *const currValue, Tree *FinalTree);

Node *diffMain (const Tree *const BegTree, Tree *FinalTree, const char *const currValue);

Node *createNode (char type, Tree *FinalTree);

Node *createNode (char type, char operator_, Node *dL, Node *dR, Tree *FinalTree);
