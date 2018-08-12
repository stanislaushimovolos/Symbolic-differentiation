//
// Created by Tom on 22.11.2017.
//

#pragma  once

#include "../Tree_t/Tree.h"
#include "../Commands/Commands.h"


static const char outFileTex[] = "Tex/Diff_Super_Test";
static const char outFilenameForDotDump[] = "picture/dotDump.txt";
static const char outFilenameForDotPrint[] = "picture/dotPrint.txt";

#define NUMBER_OF_COMMENTS 7
static const char *comments[] = {"After mental calculation: ",
                                 "It is well known that: ",
                                 "Using  WolframAlpha we have: ",
                                 "Every child knows that: ",
                                 "For a detailed explanation you should purchase the paid version: ",
                                 "If you are familiar with calculus: ",
                                 "It's not a secret that: "};


int simplifyTreeRec(Node *node);

int simplifyExpression(calculator *cal);

Node *logDerivative(const Node *node, calculator *calc);

Node *sinDerivative(const Node *node, calculator *calc);

Node *cosDerivative(const Node *node, calculator *calc);

Node *addDerivative(const Node *node, calculator *calc);

Node *subDerivative(const Node *node, calculator *calc);

Node *mulDerivative(const Node *node, calculator *calc);

Node *divDerivative(const Node *node, calculator *calc);

Node *expDerivative(const Node *node, calculator *calc);

Node *makeDerivativeStep(const Node *node, calculator *calc);

Node *findComplexDerivative(const Node *node, calculator *calc,
                            Node *(func)(const Node *node, calculator *calc));

int printPlus(Node *node);

int destructCalculator(calculator *calc);

int foldConstants(Node *node, int operation);

int findDerivative(const parser *pars, calculator *calc);

int printExpressionTex(const Node *node, FILE *outFileName);

int constructCalculator(calculator *calc, const char *inputFileName);

int texDerivativeStep(const Node *original, const Node *result, FILE *outFileName);

