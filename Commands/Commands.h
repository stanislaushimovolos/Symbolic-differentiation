//
// Created by superstraz on 8/9/18.
//

#ifndef DIFF3_0_COMMANDS_H
#define DIFF3_0_COMMANDS_H

enum
{
    Number,
    CurVariable,
    CharConst,
    Add,
    Sub,
    Mul,
    Div,
    Expo,
    Log,
    Sin,
    Cos
};

struct parser
{
    Tree tree;
    size_t sizeOfCode;
    int curCodePos;
    char *curVar;
    char *code;
};

struct calculator
{
    Tree tree;
    FILE *texFile;
};


#endif //DIFF3_0_COMMANDS_H
