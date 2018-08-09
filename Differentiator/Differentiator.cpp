#include <assert.h>
#include <iostream>
#include <cstring>
#include <math.h>

#include "Differentiator.h"


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

int TreeChecker = 0;

int findDerivative(const parser *pars, calculator *calc)
{
    assert (pars);
    assert (calc);

    fprintf(calc->texFile, "\\documentclass{article}\n"
                           "\\begin{document} \\begin{center} "
                           "The original expression \n \\[");
    printExpressionTex(pars->tree.root, 0, calc->texFile);
    fprintf(calc->texFile, "\\]");

    calc->tree.root = makeDerivativeStep(pars->tree.root, calc);

    /*while (TreeChecker == 0)
    {
        treeVisitorInf(calc->tree.root, simplifyTree);

        if (TreeChecker == 0)
            TreeChecker = 1;
        else
            TreeChecker = 0;
    }*/

    fprintf(calc->texFile, "\n\nThe final expression\n \\[");
    printExpressionTex(calc->tree.root, 0, calc->texFile);
    fprintf(calc->texFile, " \\]\n\n\\end{center}\\end{document}\n"
                           "\\end");
    fclose(calc->texFile);
    calc->texFile = NULL;

    system("pdflatex -output-directory Tex/ Tex/Diff_Super_Test");
    system("sensible-browser Tex/Diff_Super_Test.pdf &");

    calc->tree.nodeAmount = 0;
    treeVisitorInf(calc->tree.root, nodeCount);

    return 0;
}


Node *makeDerivativeStep(const Node *node, calculator *calc)
{
    Node *result = NULL;
    switch (node->type)
    {
        case number:
        {
            result = createNode(number, "0", &calc->tree);
            break;
        }
        case curVariable:
        {
            result = createNode(number, "1", &calc->tree);
            break;
        }
        case charConst:
        {
            result = createNode(number, "0", &calc->tree);
            break;
        }
        case Add:
        {
            result = addDerivative(node, calc);
            texDerivativeStep(node, result, calc->texFile);
            return result;
        }
        case Sub:
        {
            result = subDerivative(node, calc);
            texDerivativeStep(node, result, calc->texFile);
            return result;
        }
        case Mul:
        {
            result = mulDerivative(node, calc);
            texDerivativeStep(node, result, calc->texFile);
            break;
        }
        case Div:
        {
            result = divDerivative(node, calc);
            texDerivativeStep(node, result, calc->texFile);
            break;
        }
        case Expo:
        {
            result = expDerivative(node, calc);
            texDerivativeStep(node, result, calc->texFile);
            break;
        }
        case Ln:
        {
            result = findComplexDerivative(node->left, calc, logDerivative);
            texDerivativeStep(node, result, calc->texFile);
            break;
        }
        case Sin:
        {
            result = findComplexDerivative(node->left, calc, sinDerivative);
            texDerivativeStep(node, result, calc->texFile);
            break;
        }
        case Cos:
        {
            result = findComplexDerivative(node->left, calc, cosDerivative);
            texDerivativeStep(node, result, calc->texFile);
            break;
        }
        default:
            break;
    }
    return result;
}


int texDerivativeStep(const Node *original, const Node *result, FILE *outFileName)
{
    assert(original);
    assert(result);
    assert(outFileName);

    static int commentNumber = 0;
    commentNumber = commentNumber % NUMBER_OF_COMMENTS;
    fprintf(outFileName, "\\[$$%s\n$$", comments[commentNumber++]);

    printExpressionTex(original, 0, outFileName);
    fprintf(outFileName, "=");
    printExpressionTex(result, 0, outFileName);
    fprintf(outFileName, "\\]\n");

    return 0;
}


int printExpressionTex(const Node *const node, int priority, FILE *outFileName)
{
    switch (node->type)
    {
        case Add :
        {
            if (!priority)
            {
                printExpressionTex(node->left, 0, outFileName);
                fprintf(outFileName, " + ");
                printExpressionTex(node->right, 0, outFileName);
            }
            else
            {
                fprintf(outFileName, "(");
                printExpressionTex(node->left, 0, outFileName);
                fprintf(outFileName, " + ");
                printExpressionTex(node->right, 0, outFileName);
                fprintf(outFileName, ")");
            }
            break;
        }
        case Ln :
        {
            fprintf(outFileName, "\\ln(");
            printExpressionTex(node->left, 0, outFileName);
            fprintf(outFileName, ")");
            break;
        }
        case Cos :
        {
            fprintf(outFileName, "\\cos(");
            printExpressionTex(node->left, 0, outFileName);
            fprintf(outFileName, ")");
            break;
        }
        case Sin :
        {
            fprintf(outFileName, "\\sin(");
            printExpressionTex(node->left, 0, outFileName);
            fprintf(outFileName, ")");
            break;
        }
        case Sub :
        {
            if (!priority)
            {
                printExpressionTex(node->left, 0, outFileName);
                fprintf(outFileName, " - ");
                printExpressionTex(node->right, 0, outFileName);
            }
            else
            {
                fprintf(outFileName, "(");
                printExpressionTex(node->left, 0, outFileName);
                fprintf(outFileName, " - ");
                printExpressionTex(node->right, 0, outFileName);
                fprintf(outFileName, ")");
            }
            break;
        }
        case number:
        {
            if (*(node->content) == '-')
                fprintf(outFileName, "(%s)", node->content);
            else
                fprintf(outFileName, "%s", node->content);
            break;
        }
        case charConst:
        {
            fprintf(outFileName, "%s", node->content);
            break;
        }
        case curVariable:
        {
            fprintf(outFileName, "%s", node->content);
            break;
        }
        case Mul :
        {
            printExpressionTex(node->left, 1, outFileName);
            fprintf(outFileName, "\\cdot ");
            printExpressionTex(node->right, 1, outFileName);
            break;
        }
        case Div :
        {
            fprintf(outFileName, "\\frac{");
            printExpressionTex(node->left, 0, outFileName);
            fprintf(outFileName, "}{");
            printExpressionTex(node->right, 0, outFileName);
            fprintf(outFileName, "}");
            break;
        }
        case Expo :
        {
            if (node->left->type == number ||
                node->left->type == charConst ||
                node->left->type == curVariable)
            {
                printExpressionTex(node->left, 0, outFileName);
                fprintf(outFileName, "^{");
                printExpressionTex(node->right, 0, outFileName);
                fprintf(outFileName, "}");
            }
            else
            {
                fprintf(outFileName, "(");
                printExpressionTex(node->left, 0, outFileName);
                fprintf(outFileName, ")^{");
                printExpressionTex(node->right, 0, outFileName);
                fprintf(outFileName, "}");
            }
            break;
        }
        default:
            return 1;
    }
    return 0;
}


#define STANDARD_SIMPLIFICATION_LEFT                    \
do {      if (node->parent)                             \
    {                                                   \
        destructNodeRec(node->left);                    \
        if (node == node->parent->right)                \
            connectRight (node->parent, node->right);   \
                                                        \
        if (node == node->parent->left)                 \
            connectLeft (node->parent, node->right);    \
                                                        \
        destructNode (node);                            \
    }                                                   \
    else                                                \
    {                                                   \
        node->myTree->root = node->right;               \
        node->right->parent = NULL;                     \
        destructNode (node->left);                      \
        destructNode (node);                            \
    }                                                   \
} while (0)


#define STANDARD_SIMPLIFICATION_RIGHT                   \
do {                                                    \
                                                        \
    if (node->parent)                                   \
    {                                                   \
        destructNodeRec(node->right);                   \
        if (node == node->parent->right)                \
            connectRight (node->parent, node->left);    \
                                                        \
        if (node == node->parent->left)                 \
            connectLeft (node->parent, node->left);     \
                                                        \
        destructNode (node);                            \
                                                        \
    }                                                   \
    else                                                \
    {                                                   \
        node->myTree->root = node->left;                \
        node->left->parent = NULL;                      \
        destructNode (node->right);                     \
        destructNode (node);                            \
                                                        \
    }                                                   \
} while (0)


#define  CONST_FOLD(operator_)                                                                          \
do {                                                                                                    \
    char **ptrEnd = NULL;                                                                               \
    double val = strtod (node->left->content, ptrEnd) operator_ strtod (node->right->content, ptrEnd);  \
                                                                                                        \
    destructNode (node->left);                                                                          \
    destructNode (node->right);                                                                         \
                                                                                                        \
    val *= 100;                                                                                         \
    val = round(val);                                                                                   \
    int numCounter = 0;                                                                                 \
                                                                                                        \
        int helper = (int) val;                                                                         \
        while (helper > 0)                                                                              \
        {                                                                                               \
            helper /= 10;                                                                               \
            numCounter++;                                                                               \
        }                                                                                               \
    val /= 100;                                                                                         \
                                                                                                        \
    node->content = (char *) calloc ((size_t) (numCounter + 2), sizeof (char));                         \
    sprintf (node->content, "%lg",  val);                                                               \
    node->type = number;                                                                                \
    node->left = NULL;                                                                                  \
    node->right = NULL;                                                                                 \
} while(0)


int simplifyTree(Node *node)
{
    if (strcmp(node->content, "*") == 0)
    {
        if (strcmp(node->left->content, "0") == 0 || strcmp(node->right->content, "0") == 0)
        {
            destructNodeRec(node->left);
            destructNodeRec(node->right);
            *(node->content) = '0';
            node->type = number;
            node->right = NULL;
            node->left = NULL;
            TreeChecker++;
            return 0;
        }
        if (strcmp(node->right->content, "1") == 0)
        {
            STANDARD_SIMPLIFICATION_RIGHT;
            TreeChecker++;
            return 0;

        }
        if (strcmp(node->left->content, "1") == 0)
        {
            STANDARD_SIMPLIFICATION_LEFT;
            TreeChecker++;
            return 0;
        }

        if (node->right->type == number && node->left->type == number)
        {
            CONST_FOLD(*);
            TreeChecker++;
            return 0;
        }
        return 0;
    }
    if (strcmp(node->content, "+") == 0)
    {
        if (strcmp(node->left->content, "0") == 0)
        {
            STANDARD_SIMPLIFICATION_LEFT;
            TreeChecker++;
            return 0;
        }
        if (strcmp(node->right->content, "0") == 0)
        {
            STANDARD_SIMPLIFICATION_RIGHT;
            TreeChecker++;
            return 0;
        }
        if (node->right->type == number && node->left->type == number)
        {
            CONST_FOLD(+);
            TreeChecker++;
            return 0;
        }
        return 0;
    }
    if (strcmp(node->content, "-") == 0)
    {
        if (strcmp(node->right->content, "0") == 0)
        {
            STANDARD_SIMPLIFICATION_RIGHT;
            TreeChecker++;
            return 0;
        }
        if (strcmp(node->left->content, "0") == 0)
        {
            char str1[] = "-1";
            char str2[] = "*";
            nodeSetName(node->left, str1);
            node->left->type = number;
            nodeSetName(node, str2);
            node->type = Mul;
            TreeChecker++;
            return 0;
        }
        if (node->right->type == number && node->left->type == number)
        {
            CONST_FOLD(-);
            TreeChecker++;
            return 0;
        }
        return 0;
    }
    if (strcmp(node->content, "^") == 0)
    {
        if (strcmp(node->left->content, "0") == 0)
        {
            destructNodeRec(node->left);
            destructNodeRec(node->right);
            *(node->content) = '0';
            node->type = number;
            node->right = NULL;
            node->left = NULL;
            TreeChecker++;
            return 0;
        }
        if (strcmp(node->left->content, "1") == 0)
        {
            destructNodeRec(node->left);
            destructNodeRec(node->right);
            *(node->content) = '1';
            node->type = number;
            node->right = NULL;
            node->left = NULL;
            TreeChecker++;
            return 0;
        }
        if (strcmp(node->right->content, "0") == 0)
        {
            destructNodeRec(node->left);
            destructNodeRec(node->right);
            *(node->content) = '1';
            node->type = number;
            node->right = NULL;
            node->left = NULL;
            TreeChecker++;
            return 0;
        }
        if (strcmp(node->right->content, "1") == 0)
        {
            STANDARD_SIMPLIFICATION_RIGHT;
            TreeChecker++;
        }
        return 0;
    }
    if (strcmp(node->content, "/") == 0)
    {
        if (node->right->type == number && node->left->type == number)
        {
            CONST_FOLD(/);
            TreeChecker++;
            return 0;
        }
        if (strcmp(node->left->content, "0") == 0)
        {
            destructNodeRec(node->left);
            destructNodeRec(node->right);
            *(node->content) = '0';
            node->type = number;
            node->right = NULL;
            node->left = NULL;
            TreeChecker++;
            return 0;
        }
    }
}


Node *findComplexDerivative(const Node *mainNode, calculator *calc,
                            Node *(func)(const Node *node, calculator *calc))
{
    Node *mul = createNode(Mul, "*", &calc->tree);
    connectLeft(mul, func(mainNode, calc));
    connectRight(mul, makeDerivativeStep(mainNode, calc));
    return mul;
}


Node *logDerivative(const Node *node, calculator *calc)
{
    Node *denominator = copyTree(node, &calc->tree);
    Node *mainNode = createNode(Div, "/", &calc->tree);
    Node *leftNode = createNode(number, "1", &calc->tree);
    connectLeft(mainNode, leftNode);
    connectRight(mainNode, denominator);
    return mainNode;
}


Node *sinDerivative(const Node *node, calculator *calc)
{
    Node *cosContent = copyTree(node, &calc->tree);
    Node *mainNode = createNode(Cos, "cos", &calc->tree);
    connectLeft(mainNode, cosContent);
    return mainNode;
}


Node *cosDerivative(const Node *node, calculator *calc)
{
    Node *sinContent = copyTree(node, &calc->tree);
    Node *mainNode = createNode(Mul, "*", &calc->tree);
    Node *rightNode = createNode(number, "-1", &calc->tree);

    Node *leftNode = createNode(Sin, "sin", &calc->tree);
    connectLeft(leftNode, sinContent);

    connectRight(mainNode, rightNode);
    connectLeft(mainNode, leftNode);
    return mainNode;
}


Node *addDerivative(const Node *node, calculator *calc)
{
    assert(node);
    assert(node->right);
    assert(node->left);

    Node *dL = makeDerivativeStep(node->left, calc);
    Node *dR = makeDerivativeStep(node->right, calc);

    Node *mainNode = createNode(Add, node->content, &calc->tree);
    connectLeft(mainNode, dL);
    connectRight(mainNode, dR);

    return mainNode;
}


Node *subDerivative(const Node *node, calculator *calc)
{
    assert(node);
    assert(node->right);
    assert(node->left);

    Node *dL = makeDerivativeStep(node->left, calc);
    Node *dR = makeDerivativeStep(node->right, calc);
    Node *mainNode = createNode(Sub, node->content, &calc->tree);

    connectLeft(mainNode, dL);
    connectRight(mainNode, dR);

    return mainNode;
}


Node *mulDerivative(const Node *node, calculator *calc)
{
    assert(node);
    assert(node->right);
    assert(node->left);

    Node *LeftCopy = copyTree(node->left, &calc->tree);
    Node *RightCopy = copyTree(node->right, &calc->tree);

    Node *dL = makeDerivativeStep(node->left, calc);
    Node *dR = makeDerivativeStep(node->right, calc);

    Node *mainNode = createNode(Add, "+", &calc->tree);
    Node *FirstProNode = createNode(Mul, node->content, &calc->tree);
    Node *SecondProNode = createNode(Mul, node->content, &calc->tree);

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
    assert(node);
    assert(node->right);
    assert(node->left);

    Node *LeftCopy = copyTree(node->left, &calc->tree);
    Node *RightCopy = copyTree(node->right, &calc->tree);

    Node *dL = makeDerivativeStep(node->left, calc);
    Node *dR = makeDerivativeStep(node->right, calc);

    Node *minusNode = createNode(Sub, "-", &calc->tree);
    Node *FirstProNode = createNode(Mul, "*", &calc->tree);
    Node *SecondProNode = createNode(Mul, "*", &calc->tree);

    connectRight(FirstProNode, dL);
    connectLeft(FirstProNode, RightCopy);

    connectRight(SecondProNode, dR);
    connectLeft(SecondProNode, LeftCopy);

    connectLeft(minusNode, FirstProNode);
    connectRight(minusNode, SecondProNode);

    Node *denPro = createNode(Mul, "*", &calc->tree);

    Node *RightCopy1 = copyTree(node->right, &calc->tree);
    Node *RightCopy2 = copyTree(node->right, &calc->tree);

    connectRight(denPro, RightCopy1);
    connectLeft(denPro, RightCopy2);

    Node *MainNode = createNode(Div, "/", &calc->tree);

    connectLeft(MainNode, minusNode);
    connectRight(MainNode, denPro);

    return MainNode;
}


Node *expDerivative(const Node *node, calculator *calc)
{
    Node *base1 = copyTree(node->left, &calc->tree);
    Node *base2 = copyTree(node->left, &calc->tree);
    Node *base4 = copyTree(node->left, &calc->tree);

    Node *index2 = copyTree(node->right, &calc->tree);
    Node *index3 = copyTree(node->right, &calc->tree);
    Node *index4 = copyTree(node->right, &calc->tree);

    Node *mainMinus = createNode(Sub, "-", &calc->tree);
    Node *MainPlus = createNode(Add, "+", &calc->tree);
    Node *ProNode1 = createNode(Mul, "*", &calc->tree);
    Node *ProNode2 = createNode(Mul, "*", &calc->tree);
    Node *ProNode3 = createNode(Mul, "*", &calc->tree);
    Node *ProNode4 = createNode(Mul, "*", &calc->tree);

    Node *Cap1 = createNode(Expo, "^", &calc->tree);
    Node *Cap2 = createNode(Expo, "^", &calc->tree);

    Node *log = createNode(Ln, "ln", &calc->tree);

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

    connectRight(mainMinus, createNode(number, "1", &calc->tree));
    connectLeft(mainMinus, index4);
    return MainPlus;
}
