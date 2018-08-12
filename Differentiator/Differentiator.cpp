#include <assert.h>
#include <iostream>
#include <cstring>
#include <math.h>

#include "Differentiator.h"


int findDerivative(const parser *pars, calculator *calc)
{
    assert (pars);
    assert (calc);

    fprintf(calc->texFile, "\\documentclass{article}\n"
                           "\\begin{document} \\begin{center} "
                           "The original expression \n \\[");
    printExpressionTex(pars->tree.root, calc->texFile);
    fprintf(calc->texFile, "\\]");

    calc->tree.root = makeDerivativeStep(pars->tree.root, calc);
    simplifyExpression(calc);

    fprintf(calc->texFile, "\n\nThe final expression\n \\[");
    printExpressionTex(calc->tree.root, calc->texFile);
    fprintf(calc->texFile, " \\]\n\n\\end{center}\\end{document}\n"
                           "\\end");
    fclose(calc->texFile);
    calc->texFile = NULL;

    system("pdflatex -output-directory Tex/ Tex/Diff_Super_Test");
    system("sensible-browser Tex/Diff_Super_Test.pdf &");

    calc->tree.nodeAmount = 0;
    visitTree(calc->tree.root, countNodes);

    return 0;
}


Node *makeDerivativeStep(const Node *node, calculator *calc)
{
    Node *result = NULL;
    switch (node->type)
    {
        case Number:
        {
            result = createNumericalNode(Number, 0, &calc->tree);
            return result;
        }
        case CurVariable:
        {
            result = createNumericalNode(Number, 1, &calc->tree);
            return result;
        }
        case CharConst:
        {
            result = createNumericalNode(Number, 0, &calc->tree);
            return result;
        }

#define DEF_CMD_UNARY(command, number, function)                        \
case number:{                                                           \
            result = findComplexDerivative(node->left, calc, function); \
            texDerivativeStep(node, result, calc->texFile);             \
            return result;                                              \
            }                                                           \

#define DEF_CMD(command, number, function)                              \
case number:{                                                           \
            result = function(node, calc);                              \
            texDerivativeStep(node, result, calc->texFile);             \
            return result;                                              \
            }                                                           \


#include "../Commands/MathFunc.h"

        default:
            break;
    }
    return result;
}

#undef DEF_CMD_UNARY
#undef DEF_CMD


Node *findComplexDerivative(const Node *node, calculator *calc,
                            Node *(func)(const Node *node, calculator *calc))
{
    assert(func);
    assert(node);

    Node *mul = createTypeNode(Mul, &calc->tree);
    connectLeft(mul, func(node, calc));
    connectRight(mul, makeDerivativeStep(node, calc));
    return mul;
}


Node *logDerivative(const Node *node, calculator *calc)
{
    assert(calc);
    assert(node);

    Node *denominator = copyTree(node, &calc->tree);
    Node *mainNode = createTypeNode(Div, &calc->tree);
    Node *leftNode = createNumericalNode(Number, 1, &calc->tree);
    connectLeft(mainNode, leftNode);
    connectRight(mainNode, denominator);
    return mainNode;
}


Node *sinDerivative(const Node *node, calculator *calc)
{
    assert(calc);
    assert(node);

    Node *cosContent = copyTree(node, &calc->tree);
    Node *mainNode = createTypeNode(Cos, &calc->tree);
    connectLeft(mainNode, cosContent);
    return mainNode;
}


Node *cosDerivative(const Node *node, calculator *calc)
{
    assert(calc);
    assert(node);

    Node *sinContent = copyTree(node, &calc->tree);
    Node *mainNode = createTypeNode(Mul, &calc->tree);
    Node *rightNode = createNumericalNode(Number, -1, &calc->tree);

    Node *leftNode = createTypeNode(Sin, &calc->tree);
    connectLeft(leftNode, sinContent);

    connectRight(mainNode, rightNode);
    connectLeft(mainNode, leftNode);
    return mainNode;
}


Node *addDerivative(const Node *node, calculator *calc)
{
    assert(calc);
    assert(node);
    assert(node->right);
    assert(node->left);

    Node *dL = makeDerivativeStep(node->left, calc);
    Node *dR = makeDerivativeStep(node->right, calc);

    Node *mainNode = createTypeNode(Add, &calc->tree);
    connectLeft(mainNode, dL);
    connectRight(mainNode, dR);

    return mainNode;
}


Node *subDerivative(const Node *node, calculator *calc)
{
    assert(calc);
    assert(node);
    assert(node->right);
    assert(node->left);

    Node *dL = makeDerivativeStep(node->left, calc);
    Node *dR = makeDerivativeStep(node->right, calc);
    Node *mainNode = createTypeNode(Sub, &calc->tree);

    connectLeft(mainNode, dL);
    connectRight(mainNode, dR);

    return mainNode;
}


Node *mulDerivative(const Node *node, calculator *calc)
{
    assert(calc);
    assert(node);
    assert(node->right);
    assert(node->left);

    Node *LeftCopy = copyTree(node->left, &calc->tree);
    Node *RightCopy = copyTree(node->right, &calc->tree);

    Node *dL = makeDerivativeStep(node->left, calc);
    Node *dR = makeDerivativeStep(node->right, calc);

    Node *mainNode = createTypeNode(Add, &calc->tree);
    Node *FirstProNode = createTypeNode(Mul, &calc->tree);
    Node *SecondProNode = createTypeNode(Mul, &calc->tree);

    connectRight(FirstProNode, dL);
    connectLeft(FirstProNode, RightCopy);

    connectRight(SecondProNode, dR);
    connectLeft(SecondProNode, LeftCopy);

    connectLeft(mainNode, FirstProNode);
    connectRight(mainNode, SecondProNode);

    return mainNode;
}


Node *divDerivative(const Node *node, calculator *calc)
{
    assert(calc);
    assert(node);
    assert(node->right);
    assert(node->left);

    Node *LeftCopy = copyTree(node->left, &calc->tree);
    Node *RightCopy = copyTree(node->right, &calc->tree);

    Node *dL = makeDerivativeStep(node->left, calc);
    Node *dR = makeDerivativeStep(node->right, calc);

    Node *minusNode = createTypeNode(Sub, &calc->tree);
    Node *FirstProNode = createTypeNode(Mul, &calc->tree);
    Node *SecondProNode = createTypeNode(Mul, &calc->tree);

    connectRight(FirstProNode, dL);
    connectLeft(FirstProNode, RightCopy);

    connectRight(SecondProNode, dR);
    connectLeft(SecondProNode, LeftCopy);

    connectLeft(minusNode, FirstProNode);
    connectRight(minusNode, SecondProNode);

    Node *denPro = createTypeNode(Mul, &calc->tree);

    Node *RightCopy1 = copyTree(node->right, &calc->tree);
    Node *RightCopy2 = copyTree(node->right, &calc->tree);

    connectRight(denPro, RightCopy1);
    connectLeft(denPro, RightCopy2);

    Node *MainNode = createTypeNode(Div, &calc->tree);

    connectLeft(MainNode, minusNode);
    connectRight(MainNode, denPro);

    return MainNode;
}


Node *expDerivative(const Node *node, calculator *calc)
{
    assert(calc);
    assert(node);
    assert(node->right);
    assert(node->left);

    Node *base1 = copyTree(node->left, &calc->tree);
    Node *base2 = copyTree(node->left, &calc->tree);
    Node *base4 = copyTree(node->left, &calc->tree);

    Node *index2 = copyTree(node->right, &calc->tree);
    Node *index3 = copyTree(node->right, &calc->tree);
    Node *index4 = copyTree(node->right, &calc->tree);

    Node *mainMinus = createTypeNode(Sub, &calc->tree);
    Node *MainPlus = createTypeNode(Add, &calc->tree);
    Node *ProNode1 = createTypeNode(Mul, &calc->tree);
    Node *ProNode2 = createTypeNode(Mul, &calc->tree);
    Node *ProNode3 = createTypeNode(Mul, &calc->tree);
    Node *ProNode4 = createTypeNode(Mul, &calc->tree);

    Node *Cap1 = createTypeNode(Expo, &calc->tree);
    Node *Cap2 = createTypeNode(Expo, &calc->tree);

    Node *log = createTypeNode(Log, &calc->tree);

    connectLeft(ProNode1, log);
    connectLeft(log, base1);
    connectRight(ProNode1, makeDerivativeStep(node->right, calc));
    connectLeft(ProNode2, ProNode1);
    connectRight(ProNode2, Cap1);
    connectLeft(Cap1, base2);
    connectRight(Cap1, index2);
    connectRight(ProNode2, Cap1);
    connectLeft(MainPlus, ProNode2);

    connectRight(MainPlus, ProNode3);
    connectRight(ProNode3, Cap2);
    connectLeft(ProNode3, ProNode4);

    connectLeft(ProNode4, index3);
    connectRight(ProNode4, makeDerivativeStep(node->left, calc));
    connectLeft(Cap2, base4);
    connectRight(Cap2, mainMinus);

    connectRight(mainMinus, createNumericalNode(Number, 1, &calc->tree));
    connectLeft(mainMinus, index4);
    return MainPlus;
}


int foldConstants(Node *node, int operation)
{
    assert(node);
    double firstVal = node->left->value,
            secondVal = node->right->value;
    double result = 0;

    destructNode(node->left);
    destructNode(node->right);
    free(node->content);

    node->right = NULL;
    node->left = NULL;
    node->type = Number;

    switch (operation)
    {
        case Add:
        {
            result = firstVal + secondVal;
            break;
        }
        case Sub:
        {
            result = firstVal - secondVal;
            break;
        }
        case Mul:
        {
            result = firstVal * secondVal;
            break;
        }
        case Div:
        {
            result = firstVal / secondVal;
            break;
        }
        case Expo:
        {
            result = pow(firstVal, secondVal);
            break;
        }
        default:
            return 1;
    }

    node->value = result;
    return 0;
}


#define SIMPLIFY_LEFT_SIDE                              \
do {                                                    \
    destructNode(node->left);                           \
if (node->parent)                                       \
    {                                                   \
        if (node == node->parent->right)                \
            connectRight (node->parent, node->right);   \
        else                                            \
            connectLeft (node->parent, node->right);    \
    }                                                   \
    else                                                \
    {                                                   \
        node->myTree->root = node->right;               \
        node->right->parent = NULL;                     \
    }                                                   \
    destructNode (node);                                \
                                                        \
} while (0)


#define SIMPLIFY_RIGHT_SIDE                             \
do {                                                    \
    destructNode(node->right);                          \
    if (node->parent)                                   \
    {                                                   \
        if (node == node->parent->right)                \
            connectRight (node->parent, node->left);    \
        else                                            \
            connectLeft (node->parent, node->left);     \
    }                                                   \
    else                                                \
    {                                                   \
        node->myTree->root = node->left;                \
        node->left->parent = NULL;                      \
    }                                                   \
    destructNode (node);                                \
} while (0)


#define REPLACE_BY_VALUE(val)                           \
    destructNodeRec(node->left);                        \
    destructNodeRec(node->right);                       \
    node->right = NULL;                                 \
    node->left = NULL;                                  \
                                                        \
    node->value = val;                                  \
    node->type = Number;                                \


int simplifyExpression(calculator *calc)
{
    assert(calc);
    assert(calc->tree.root);

    Tree *tree = &calc->tree;
    tree->eventFlag = 1;

    while (tree->eventFlag)
    {
        tree->eventFlag = 0;
        visitTree(tree->root, simplifyTreeRec);
    }
    return 0;
}


int simplifyTreeRec(Node *node)
{
    switch (node->type)
    {
        case Mul:
        {
            if (node->left->type == Number)
            {
                if (node->left->value == 1)
                {
                    node->myTree->eventFlag = 1;
                    SIMPLIFY_LEFT_SIDE;
                    return 1;
                }
                if (node->left->value == 0)
                {
                    node->myTree->eventFlag = 1;
                    REPLACE_BY_VALUE(0);
                    return 1;
                }
                if (node->right->type == Number)
                {
                    node->myTree->eventFlag = 1;
                    foldConstants(node, Mul);
                    return 1;
                }
                return 0;
            }
            if (node->right->type == Number)
            {
                if (node->right->value == 1)
                {
                    node->myTree->eventFlag = 1;
                    SIMPLIFY_RIGHT_SIDE;
                    return 1;
                }
                if (node->right->value == 0)
                {
                    node->myTree->eventFlag = 1;
                    REPLACE_BY_VALUE(0);
                    return 1;

                }
                return 0;
            }
            return 0;
        }
        case Add:
        {
            if (node->left->type == Number)
            {
                if (node->left->value == 0)
                {
                    node->myTree->eventFlag = 1;
                    SIMPLIFY_LEFT_SIDE;
                    return 1;
                }
                if (node->right->type == Number)
                {
                    node->myTree->eventFlag = 1;
                    foldConstants(node, Add);
                    return 1;
                }
                return 0;
            }
            if (node->right->type == Number)
            {
                if (node->right->value == 0)
                {
                    node->myTree->eventFlag = 1;
                    SIMPLIFY_RIGHT_SIDE;
                    return 1;
                }
                return 0;
            }
            return 0;
        }
        case Sub:
        {
            if (node->left->type == Number)
            {
                if (node->left->value == 0)
                {
                    node->myTree->eventFlag = 1;
                    node->left->value = -1;
                    node->type = Mul;
                    return 1;
                }
                if (node->right->type == Number)
                {
                    node->myTree->eventFlag = 1;
                    foldConstants(node, Sub);
                    return 1;
                }
                return 0;
            }
            if (node->right->type == Number)
            {
                if (node->right->value == 0)
                {
                    node->myTree->eventFlag = 1;
                    SIMPLIFY_RIGHT_SIDE;
                    return 1;
                }
            }
            return 0;
        }
        case Expo:
        {
            if (node->left->type == Number)
            {
                if (node->left->value == 0)
                {
                    node->myTree->eventFlag = 1;
                    REPLACE_BY_VALUE(0);
                    return 1;
                }
                if (node->left->value == 1)
                {
                    node->myTree->eventFlag = 1;
                    REPLACE_BY_VALUE(1);
                    return 1;
                }
                if (node->right->type == Number)
                {
                    node->myTree->eventFlag = 1;
                    foldConstants(node, Expo);
                    return 1;
                }
                return 0;
            }
            if (node->right->type == Number)
            {
                if (node->right->value == 0)
                {
                    node->myTree->eventFlag = 1;
                    REPLACE_BY_VALUE(1);
                    return 1;
                }
                if (node->right->value == 1)
                {
                    node->myTree->eventFlag = 1;
                    SIMPLIFY_RIGHT_SIDE;
                    return 1;
                }
                return 0;
            }
            return 0;
        }
        case Div:
        {
            if (node->right->type == Number)
            {
                if (node->right->value == 1)
                {
                    node->myTree->eventFlag = 1;
                    SIMPLIFY_RIGHT_SIDE;
                    return 1;
                }
                if (node->left->type == Number)
                {
                    foldConstants(node, Div);
                    node->myTree->eventFlag = 1;
                    return 1;
                }
                return 0;
            }
            if (node->left->type == Number)
            {
                if (node->left->value == 0)
                {
                    node->myTree->eventFlag = 1;
                    REPLACE_BY_VALUE(0);
                    return 1;
                }
                return 0;
            }
            return 0;
        }
        default:
            return 0;
    }
}


int constructCalculator(calculator *calc, const char *outputFileName)
{
    assert(calc);
    assert(outputFileName);

    FILE *output = fopen(outputFileName, "w");
    if (!output)
        return 1;

    calc->texFile = output;
    treeConstruct(&calc->tree);
    return 0;
}


int destructCalculator(calculator *calc)
{
    assert(calc);
    if (calc->texFile)
        fclose(calc->texFile);

    destructTree(&calc->tree);
    return 0;
}


int texDerivativeStep(const Node *original, const Node *result, FILE *outFileName)
{
    assert(original);
    assert(result);
    assert(outFileName);

    static int commentNumber = 0;
    commentNumber = commentNumber % NUMBER_OF_COMMENTS;
    fprintf(outFileName, "\\[$$%s\n$$", comments[commentNumber++]);

    fprintf(outFileName, "(");
    printExpressionTex(original, outFileName);
    fprintf(outFileName, ")' = ");
    printExpressionTex(result, outFileName);
    fprintf(outFileName, "\\]\n");

    return 0;
}


int printExpressionTex(const Node *node, FILE *outFileName)
{
    switch (node->type)
    {
        case Add :
        {
            if (node->parent && node->parent->type == Mul)
            {
                fprintf(outFileName, "(");
                printExpressionTex(node->left, outFileName);
                fprintf(outFileName, " + ");
                printExpressionTex(node->right, outFileName);
                fprintf(outFileName, ")");
            }
            else
            {
                printExpressionTex(node->left, outFileName);
                fprintf(outFileName, " + ");
                printExpressionTex(node->right, outFileName);
            }
            return 0;
        }
        case Sub :
        {
            if (node->parent && node->parent->type == Mul)
            {
                fprintf(outFileName, "(");
                printExpressionTex(node->left, outFileName);
                fprintf(outFileName, " - ");
                printExpressionTex(node->right, outFileName);
                fprintf(outFileName, ")");
            }
            else
            {
                printExpressionTex(node->left, outFileName);
                fprintf(outFileName, " - ");
                printExpressionTex(node->right, outFileName);
            }
            return 0;
        }
        case Log :
        {
            fprintf(outFileName, "\\log(");
            printExpressionTex(node->left, outFileName);
            fprintf(outFileName, ")");
            return 0;
        }
        case Cos :
        {
            fprintf(outFileName, "\\cos(");
            printExpressionTex(node->left, outFileName);
            fprintf(outFileName, ")");
            return 0;
        }
        case Sin :
        {
            fprintf(outFileName, "\\sin(");
            printExpressionTex(node->left, outFileName);
            fprintf(outFileName, ")");
            return 0;
        }
        case Number:
        {
            if (node->value < 0)
                fprintf(outFileName, "(%lg)", node->value);
            else
                fprintf(outFileName, "%lg", node->value);
            return 0;
        }
        case CharConst:
        {
            fprintf(outFileName, "%s", node->content);
            return 0;
        }
        case CurVariable:
        {
            fprintf(outFileName, "%s", node->content);
            return 0;
        }
        case Mul :
        {
            printExpressionTex(node->left, outFileName);
            fprintf(outFileName, "\\cdot ");
            printExpressionTex(node->right, outFileName);
            return 0;
        }
        case Div :
        {
            fprintf(outFileName, "\\frac{");
            printExpressionTex(node->left, outFileName);
            fprintf(outFileName, "}{");
            printExpressionTex(node->right, outFileName);
            fprintf(outFileName, "}");
            return 0;
        }
        case Expo :
        {
            if (node->left->type == Number ||
                node->left->type == CharConst ||
                node->left->type == CurVariable)
            {
                printExpressionTex(node->left, outFileName);
                fprintf(outFileName, "^{");
                printExpressionTex(node->right, outFileName);
                fprintf(outFileName, "}");
            }
            else
            {
                fprintf(outFileName, "(");
                printExpressionTex(node->left, outFileName);
                fprintf(outFileName, ")^{");
                printExpressionTex(node->right, outFileName);
                fprintf(outFileName, "}");
            }
            return 0;
        }
        default:
            return 1;
    }
}
