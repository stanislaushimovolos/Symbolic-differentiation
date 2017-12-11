//
// Created by Tom on 22.11.2017.
//

#pragma  once

#include "../Tree_t/Tree.h"

enum
{
	number, curVariable, charConst, Add, Sub, Mul, Div, Expo, Ln, Sin, Cos

};

#define MAX_JOKE_SIZE 100
#define JOKE_AMOUNT 7

const char MainVariable[] = "x";

const char variableFlag = '$';

const char inputFilename[] = "../base/base.txt";
const char outFilename[] = "../Tex/tex.txt";
const char outFilenameForDotDump[] = "../picture/dotDump.txt";
const char outFilenameForDotPrint[] = "../picture/dotPrint.txt";

const char Comments[JOKE_AMOUNT + 1][MAX_JOKE_SIZE] = {{"After a simple calculating in mind: "},
                                                       {"It is well known that: "},
                                                       {"Using  WolframAlpha we have: "},
                                                       {"Every child knows that: "},
                                                       {"For a detailed explanation you should to purchase the paid version  "},
                                                       {"By simple mathematical calculations"},
                                                       {"It's not a secret that"}};

void TexExp (const Node *const node, FILE *outBaseFile);

void TexStr (FILE *outBaseFile);

void contentAnalyze (Node *node, const char *const currValue);

int printResultRec (const Node *const Node, int prior, FILE *outBaseFile1);

int printResultFile (const Tree *const tree, const char *outFileName);

int simpleTree (Node *node);

char *getBuff (const char *_inputFileName, char **curVar);

char *getBufferFromFileGetSzOfBuf (const char *_inputFileName, size_t *sizeOfBuffer = NULL);

Node *diffRec (const Node *const node, const char *const currValue, Tree *FinalTree, FILE *outFileName);

Node *diffMain (const Tree *const BegTree, Tree *FinalTree, const char *const currValue);

Node *complicatedDiff (Node *mainNode, Tree *FinalTree, const char *const currValue,
                       Node *(diff) (Node *node, Tree *FilTree, const char *const curValue, FILE *OutFileName),
                       FILE *outFileName);

Node *lnDiff (Node *node, Tree *FinalTree, const char *const currValue, FILE *outFileName);

Node *sinDiff (Node *node, Tree *FinalTree, const char *const currValue, FILE *outFileName);

Node *cosDiff (Node *node, Tree *FinalTree, const char *const currValue, FILE *outFileName);

Node *addDiff (const Node *const node, Tree *FinalTree, const char *const currValue, FILE *outFileName);

Node *subDiff (const Node *const node, Tree *FinalTree, const char *const currValue, FILE *outFileName);

Node *mulDiff (const Node *const node, Tree *FinalTree, const char *const currValue, FILE *outFileName);

Node *divDiff (const Node *const node, Tree *FinalTree, const char *const currValue, FILE *outFileName);

Node *expoDiff (const Node *const node, Tree *FinalTree, const char *const currValue, FILE *outFileName);


Node *GetTree (Tree *tree, char *content, const char *const currValue);

Node *GetAddSub (Tree *tree, const char *const currValue);

Node *GetMulDiv (Tree *tree, const char *const currValue);

Node *GetExpo (Tree *tree, const char *const currValue);

Node *GetBranches (Tree *tree, const char *const currValue);

Node *GetFunc (Tree *tree, const char *const currValue);

Node *GetNumber (Tree *tree, const char *const currValue);

