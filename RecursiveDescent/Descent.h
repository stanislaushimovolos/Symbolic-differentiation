//
// Created by superstraz on 8/8/18.
//

#ifndef DIFF3_0_DESCENT_H
#define DIFF3_0_DESCENT_H

#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../Tree_t/Tree.h"
#include "../Commands/Commands.h"

static const char inputFilename[] = "base/base.txt";

enum
{
    FILE_OPENING_ERROR = 1,
    FORMAT_ERROR,
    UNKNOWN_FUNC,
    ARITHMETIC_OP,
    ARGUMENTS_ERROR,
    USER_ERROR,
    OUT_OF_ERR_RANGE_ERR,
};

static const char *errorList[] = {"",
                                  "",
                                  "Wrong format\nExpected d/d <variable> <expression>\n",
                                  "Unknown function:\n",
                                  "Expected arithmetic operation\n"
};

Node *GetAddSub(parser *tree);

Node *GetMulDiv(parser *tree);

Node *GetExpo(parser *tree);

Node *GetBranches(parser *tree);

Node *GetFunc(parser *tree);

Node *GetNumber(parser *tree);

int getData(parser *pars, const char *inputFileName);

int getBuf(parser *data, const char *inputFileName);

int destructParser(parser *pars);

int constructParser(parser *pars, const char *inputFileName);

int GetTree(parser *tree);


int contentAnalyze(Node *node, parser *pars);

int throw_error(unsigned int err_num, const char *usr_msg, const char *err_msg, const char *_func, int _line,
                const char *_file);

#endif //DIFF3_0_DESCENT_H
