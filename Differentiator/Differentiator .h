//
// Created by Tom on 22.11.2017.
//

#pragma  once

#include "../Tree_t/Tree.h"
#include "../Commands/Commands.h"

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

int constructCalculator(calculator *calc, const char *inputFileName);

int printResultRec(const Node *const Node, int prior, FILE *outBaseFile1);

int printResultFile(const Tree *const tree, const char *outFileName);

int destructCalculator(calculator *calc);

int simpleTree(Node *node);

Node *diffRec(Node *node, calculator *calc);

Node *diffMain(parser *pars, calculator *calc);

Node *complicatedDiff(Node *mainNode, calculator *calc, Node *(diff)(Node *node, calculator *calc));

Node *lnDiff(Node *node, calculator *calc);

Node *sinDiff(Node *node, calculator *calc);

Node *cosDiff(Node *node, calculator *calc);

Node *addDiff(Node *node, calculator *calc);

Node *subDiff(Node *node, calculator *calc);

Node *mulDiff(Node *node, calculator *calc);

Node *divDiff(Node *node, calculator *calc);

Node *expoDiff(Node *node, calculator *calc);

