//
// Created by Tom on 22.11.2017.
//

#pragma  once

#include "../Tree_t/Tree.h"

#define MAX_JOKE_SIZE 100
#define JOKE_AMOUNT 7

const char outFilename[] = "Tex/Diff_Super_Test";
const char outFilenameForDotDump[] = "picture/dotDump.txt";
const char outFilenameForDotPrint[] = "picture/dotPrint.txt";

const char Comments[][MAX_JOKE_SIZE] = {{"After a simple calculating in mind: "},
                                        {"It is well known that: "},
                                        {"Using  WolframAlpha we have: "},
                                        {"Every child knows that: "},
                                        {"For a detailed explanation you should to purchase the paid version  "},
                                        {"By simple mathematical calculations"},
                                        {"It's not a secret that"}};

void TexExp(const Node *const node, FILE *outBaseFile);

void TexStr(FILE *outBaseFile);

int printResultRec(const Node *const Node, int prior, FILE *outBaseFile1);

int printResultFile(const Tree *const tree, const char *outFileName);

int simpleTree(Node *node);

Node *diffRec(const Node *const node, const char *const currValue, Tree *FinalTree, FILE *outFileName);

Node *diffMain(const Tree *const BegTree, Tree *FinalTree, const char *const currValue);

Node *complicatedDiff(Node *mainNode, Tree *FinalTree, const char *const currValue,
                      Node *(diff)(Node *node, Tree *FilTree, const char *const curValue, FILE *OutFileName),
                      FILE *outFileName);

Node *lnDiff(Node *node, Tree *FinalTree, const char *const currValue, FILE *outFileName);

Node *sinDiff(Node *node, Tree *FinalTree, const char *const currValue, FILE *outFileName);

Node *cosDiff(Node *node, Tree *FinalTree, const char *const currValue, FILE *outFileName);

Node *addDiff(const Node *const node, Tree *FinalTree, const char *const currValue, FILE *outFileName);

Node *subDiff(const Node *const node, Tree *FinalTree, const char *const currValue, FILE *outFileName);

Node *mulDiff(const Node *const node, Tree *FinalTree, const char *const currValue, FILE *outFileName);

Node *divDiff(const Node *const node, Tree *FinalTree, const char *const currValue, FILE *outFileName);

Node *expoDiff(const Node *const node, Tree *FinalTree, const char *const currValue, FILE *outFileName);

