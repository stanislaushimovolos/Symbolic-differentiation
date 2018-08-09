//
// Created by Tom on 02.12.2017.
//


//currValue = MainValue
#define  DEF_CMD_UNARY(operator_, number, func)                                                 \
    DEF_CMD(operator_, number,{                                                                 \
        Node *LeftCopy = TreeCopy (node->Left, FinalTree);                                      \
        Node *nodeDif = complicatedDiff (LeftCopy, FinalTree, currValue, func, outFileName);    \
        fprintf(outFileName, " \\[ ");                                                          \
        TexStr (outFileName);                                                                   \
        TexExp (node, outFileName);                                                             \
        fprintf(outFileName, "~\\Rightarrow~");                                                 \
        TexExp (nodeDif, outFileName);                                                          \
        fprintf(outFileName, "\\]\n");                                                          \
        return nodeDif;})                                                                       \


#define  DEF_CMD_BINARY(operator_, number, func)                                     \
    DEF_CMD(operator_, number, {                                                     \
        Node *nodeDif =   func(node, FinalTree, currValue, outFileName);             \
        fprintf(outFileName, " \\[ ");                                               \
        TexStr (outFileName);                                                        \
        TexExp (node, outFileName);                                                  \
        fprintf(outFileName, "~\\Rightarrow~");                                      \
        TexExp (nodeDif, outFileName);\
        fprintf(outFileName, "\\]\n");\
        return nodeDif;})


DEF_CMD_UNARY (ln, Ln, lnDiff)

DEF_CMD_UNARY (sin, Sin, sinDiff)

DEF_CMD_UNARY (cos, Cos, cosDiff)

DEF_CMD_BINARY (+, Add, addDiff)

DEF_CMD_BINARY (-, Sub, subDiff)

DEF_CMD_BINARY (*, Mul, mulDiff)

DEF_CMD_BINARY (/, Div, divDiff)

DEF_CMD_BINARY (^, Expo, expoDiff)


#undef DEF_CMD_UNARY

#undef DEF_CMD_BINARY