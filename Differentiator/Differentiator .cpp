#include <assert.h>
#include <iostream>
#include <cstring>
#include <math.h>

#include "Differentiator .h"


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

    destructTree(&calc->tree);
    return 0;
}

int TreeChecker = 0;

Node *diffMain(parser *pars, calculator *calc)
{
    assert (pars);
    assert (calc);

    fprintf(calc->texFile, "\\documentclass{article}\n"
                           "\\begin{document} \\begin{center} The original expression \n \\[");

    TexExp(pars->tree.root, calc->texFile);
    fprintf(calc->texFile, " \\] ");

    calc->tree.root = diffRec(pars->tree.root, calc);


    /* while (TreeChecker == 0)
     {
         treeVisitorInf(calc->tree.root, simpleTree);

         if (TreeChecker == 0)
             TreeChecker = 1;
         else
             TreeChecker = 0;
     }*/

    fprintf(calc->texFile, "\n\nThe final expression\n \\[");

    TexExp(calc->tree.root, calc->texFile);

    fprintf(calc->texFile, " \\]\n\n\\end{center}\\end{document}\n"
                           "\\end");

    fclose(calc->texFile);

    system("pdflatex -output-directory Tex/ Tex/Diff_Super_Test");
    system("sensible-browser Tex/Diff_Super_Test.pdf &");

    calc->tree.nodeAmount = 0;
    treeVisitorInf(calc->tree.root, nodeCount);
}


Node *diffRec(Node *node, calculator *calc)
{
    Node *retNode = NULL;
    switch (node->type)
    {
        case number:
        {
            retNode = createNode(number, "0", &calc->tree);
            break;
        }
        case curVariable:
        {
            retNode = createNode(number, "1", &calc->tree);
            break;
        }
        case charConst:
        {
            retNode = createNode(number, "0", &calc->tree);
            break;
        }
        case Add:
        {
            Node *nodeDif = addDiff(node, calc);
            fprintf(calc->texFile, " \\[ ");
            TexStr(calc->texFile);
            TexExp(node, calc->texFile);
            fprintf(calc->texFile, "~\\Rightarrow~");
            TexExp(nodeDif, calc->texFile);
            fprintf(calc->texFile, "\\]\n");
            return nodeDif;
        }
        case Sub:
        {
            Node *nodeDif = subDiff(node, calc);
            fprintf(calc->texFile, " \\[ ");
            TexStr(calc->texFile);
            TexExp(node, calc->texFile);
            fprintf(calc->texFile, "~\\Rightarrow~");
            TexExp(nodeDif, calc->texFile);
            fprintf(calc->texFile, "\\]\n");
            return nodeDif;
        }
        case Mul:
        {
            Node *nodeDif = mulDiff(node, calc);
            fprintf(calc->texFile, " \\[ ");
            TexStr(calc->texFile);
            TexExp(node, calc->texFile);
            fprintf(calc->texFile, "~\\Rightarrow~");
            TexExp(nodeDif, calc->texFile);
            fprintf(calc->texFile, "\\]\n");
            return nodeDif;
        }
        case Div:
        {
            Node *nodeDif = divDiff(node, calc);
            fprintf(calc->texFile, " \\[ ");
            TexStr(calc->texFile);
            TexExp(node, calc->texFile);
            fprintf(calc->texFile, "~\\Rightarrow~");
            TexExp(nodeDif, calc->texFile);
            fprintf(calc->texFile, "\\]\n");
            return nodeDif;
        }
        case Expo:
        {
            Node *nodeDif = expoDiff(node, calc);
            fprintf(calc->texFile, " \\[ ");
            TexStr(calc->texFile);
            TexExp(node, calc->texFile);
            fprintf(calc->texFile, "~\\Rightarrow~");
            TexExp(nodeDif, calc->texFile);
            fprintf(calc->texFile, "\\]\n");
            return nodeDif;
        }

        case Ln:
        {
            Node *nodeDif = complicatedDiff(node->Left, calc, lnDiff);
            fprintf(calc->texFile, " \\[ ");
            TexStr(calc->texFile);
            TexExp(node, calc->texFile);
            fprintf(calc->texFile, "~\\Rightarrow~");
            TexExp(nodeDif, calc->texFile);
            fprintf(calc->texFile, "\\]\n");
            return nodeDif;
        }

        case Sin:
        {
            Node *nodeDif = complicatedDiff(node->Left, calc, sinDiff);
            fprintf(calc->texFile, " \\[ ");
            TexStr(calc->texFile);
            TexExp(node, calc->texFile);
            fprintf(calc->texFile, "~\\Rightarrow~");
            TexExp(nodeDif, calc->texFile);
            fprintf(calc->texFile, "\\]\n");
            return nodeDif;
        }

        case Cos:
        {
            Node *nodeDif = complicatedDiff(node->Left, calc, cosDiff);
            fprintf(calc->texFile, " \\[ ");
            TexStr(calc->texFile);
            TexExp(node, calc->texFile);
            fprintf(calc->texFile, "~\\Rightarrow~");
            TexExp(nodeDif, calc->texFile);
            fprintf(calc->texFile, "\\]\n");
            return nodeDif;
        }

        default:
            break;

    }

    return retNode;
}


#undef DEF_CMD_BIN_OPERATOR


Node *complicatedDiff(Node *mainNode, calculator *calc,
                      Node *(diff)(Node *node, calculator *calc))
{
    Node *mul = createNode(Mul, "*", &calc->tree);
    connectLeft(mul, diff(mainNode, calc));
    connectRight(mul, diffRec(mainNode, calc));
    return mul;
}


Node *lnDiff(Node *node, calculator *calc)
{
    Node *denominator = TreeCopy(node, &calc->tree);
    Node *mainNode = createNode(Div, "/", &calc->tree);
    Node *leftNode = createNode(number, "1", &calc->tree);
    connectLeft(mainNode, leftNode);
    connectRight(mainNode, denominator);
    return mainNode;
}


Node *sinDiff(Node *node, calculator *calc)
{
    Node *cosContent = TreeCopy(node, &calc->tree);
    Node *mainNode = createNode(Cos, "cos", &calc->tree);
    connectLeft(mainNode, cosContent);
    return mainNode;
}


Node *cosDiff(Node *node, calculator *calc)
{
    Node *sinContent = TreeCopy(node, &calc->tree);
    Node *mainNode = createNode(Mul, "*", &calc->tree);
    Node *rightNode = createNode(number, "-1", &calc->tree);

    Node *leftNode = createNode(Sin, "sin", &calc->tree);
    connectLeft(leftNode, sinContent);

    connectRight(mainNode, rightNode);
    connectLeft(mainNode, leftNode);
    return mainNode;
}


Node *addDiff(Node *node, calculator *calc)
{
    assert(node);
    assert(node->Right);
    assert(node->Left);

    Node *dL = diffRec(node->Left, calc);
    Node *dR = diffRec(node->Right, calc);

    Node *mainNode = createNode(Add, node->content, &calc->tree);
    connectLeft(mainNode, dL);
    connectRight(mainNode, dR);

    return mainNode;
}


Node *subDiff(Node *node, calculator *calc)
{
    assert(node);
    assert(node->Right);
    assert(node->Left);

    Node *dL = diffRec(node->Left, calc);
    Node *dR = diffRec(node->Right, calc);
    Node *mainNode = createNode(Sub, node->content, &calc->tree);

    connectLeft(mainNode, dL);
    connectRight(mainNode, dR);

    return mainNode;
}


Node *mulDiff(Node *node, calculator *calc)
{
    assert(node);
    assert(node->Right);
    assert(node->Left);

    Node *LeftCopy = TreeCopy(node->Left, &calc->tree);
    Node *RightCopy = TreeCopy(node->Right, &calc->tree);

    Node *dL = diffRec(node->Left, calc);
    Node *dR = diffRec(node->Right, calc);

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


Node *divDiff(Node *node, calculator *calc)
{
    assert(node);
    assert(node->Right);
    assert(node->Left);

    Node *LeftCopy = TreeCopy(node->Left, &calc->tree);
    Node *RightCopy = TreeCopy(node->Right, &calc->tree);

    Node *dL = diffRec(node->Left, calc);
    Node *dR = diffRec(node->Right, calc);

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

    Node *RightCopy1 = TreeCopy(node->Right, &calc->tree);
    Node *RightCopy2 = TreeCopy(node->Right, &calc->tree);

    connectRight(denPro, RightCopy1);
    connectLeft(denPro, RightCopy2);

    Node *MainNode = createNode(Div, "/", &calc->tree);

    connectLeft(MainNode, minusNode);
    connectRight(MainNode, denPro);

    return MainNode;
}


Node *expoDiff(Node *node, calculator *calc)
{
    char ln[] = "ln";

    Node *base1 = TreeCopy(node->Left, &calc->tree);
    Node *base2 = TreeCopy(node->Left, &calc->tree);
    //Node *base3 = TreeCopy(node->Left, FinalTree);
    Node *base4 = TreeCopy(node->Left, &calc->tree);

    //Node *index1 = TreeCopy(node->Right, FinalTree);
    Node *index2 = TreeCopy(node->Right, &calc->tree);
    Node *index3 = TreeCopy(node->Right, &calc->tree);
    Node *index4 = TreeCopy(node->Right, &calc->tree);

    Node *mainMinus = createNode(Sub, "-", &calc->tree);
    Node *MainPlus = createNode(Add, "+", &calc->tree);
    Node *ProNode1 = createNode(Mul, "*", &calc->tree);
    Node *ProNode2 = createNode(Mul, "*", &calc->tree);
    Node *ProNode3 = createNode(Mul, "*", &calc->tree);
    Node *ProNode4 = createNode(Mul, "*", &calc->tree);

    Node *Cap1 = createNode(Expo, "^", &calc->tree);
    Node *Cap2 = createNode(Expo, "^", &calc->tree);

    Node *LN = {};
    nodeConstruct(&LN);
    nodeSetName(LN, ln);
    LN->type = Ln;

    connectLeft(ProNode1, LN);
    connectLeft(LN, base1);
    connectRight(ProNode1, diffRec(node->Right, calc));
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
    connectRight(ProNode4, diffRec(node->Left, calc));
    connectLeft(Cap2, base4);
    connectRight(Cap2, mainMinus);

    connectRight(mainMinus, createNode(number, "1", &calc->tree));
    connectLeft(mainMinus, index4);
    return MainPlus;
}


void TexExp(const Node *const node, FILE *outBaseFile)
{
    int priorityHelper = 0;
    printResultRec(node, priorityHelper, outBaseFile);
}

int strCounter = 0;

void TexStr(FILE *outBaseFile)
{
    strCounter++;
    strCounter = strCounter % JOKE_AMOUNT;

    fprintf(outBaseFile, "$$");
    fprintf(outBaseFile, "%s\n", Comments[strCounter]);
    fprintf(outBaseFile, "$$");
}


int printResultRec(const Node *const Node, int prior, FILE *outBaseFile1)
{
    switch (Node->type)
    {
        case Add :
        {
            if (prior == 0)
            {
                printResultRec(Node->Left, 0, outBaseFile1);
                fprintf(outBaseFile1, " + ");
                printResultRec(Node->Right, 0, outBaseFile1);
            }

            else
            {
                fprintf(outBaseFile1, "(");
                printResultRec(Node->Left, 0, outBaseFile1);
                fprintf(outBaseFile1, " + ");
                printResultRec(Node->Right, 0, outBaseFile1);
                fprintf(outBaseFile1, ")");
            }

            break;
        }

        case Ln :
        {
            fprintf(outBaseFile1, "\\ln(");
            printResultRec(Node->Left, 0, outBaseFile1);
            fprintf(outBaseFile1, ")");

            break;
        }

        case Cos :
        {
            fprintf(outBaseFile1, "\\cos(");
            printResultRec(Node->Left, 0, outBaseFile1);
            fprintf(outBaseFile1, ")");

            break;
        }

        case Sin :
        {
            fprintf(outBaseFile1, "\\sin(");
            printResultRec(Node->Left, 0, outBaseFile1);
            fprintf(outBaseFile1, ")");

            break;
        }

        case Sub :
        {
            if (prior == 0)
            {
                printResultRec(Node->Left, 0, outBaseFile1);
                fprintf(outBaseFile1, " - ");
                printResultRec(Node->Right, 0, outBaseFile1);
            }

            else
            {
                fprintf(outBaseFile1, "(");
                printResultRec(Node->Left, 0, outBaseFile1);
                fprintf(outBaseFile1, " - ");
                printResultRec(Node->Right, 0, outBaseFile1);
                fprintf(outBaseFile1, ")");
            }

            break;
        }


        case number:
        {
            if (*(Node->content) == '-')
            {
                fprintf(outBaseFile1, "(%s)", Node->content);
            }

            else
                fprintf(outBaseFile1, "%s", Node->content);

            break;
        }

        case charConst:
        {
            fprintf(outBaseFile1, "%s", Node->content);
            break;
        }

        case curVariable:
        {
            fprintf(outBaseFile1, "%s", Node->content);
            break;
        }


        case Mul :
        {

            printResultRec(Node->Left, 1, outBaseFile1);
            fprintf(outBaseFile1, "\\cdot ");
            printResultRec(Node->Right, 1, outBaseFile1);


            break;
        }

        case Div :
        {
            fprintf(outBaseFile1, "\\frac{");
            printResultRec(Node->Left, 0, outBaseFile1);
            fprintf(outBaseFile1, "}{");
            printResultRec(Node->Right, 0, outBaseFile1);
            fprintf(outBaseFile1, "}");

            break;
        }

        case Expo :
        {
            if (Node->Left->type == charConst || Node->Left->type == number || Node->Left->type == curVariable)
            {
                printResultRec(Node->Left, 0, outBaseFile1);
                fprintf(outBaseFile1, "^{");
                printResultRec(Node->Right, 0, outBaseFile1);
                fprintf(outBaseFile1, "}");
            }

            else
            {
                fprintf(outBaseFile1, "(");
                printResultRec(Node->Left, 0, outBaseFile1);
                fprintf(outBaseFile1, ")");
                fprintf(outBaseFile1, "^{");
                printResultRec(Node->Right, 0, outBaseFile1);
                fprintf(outBaseFile1, "}");
            }

            break;
        }

        default:
            break;
    }
}


#define STANDARD_SIMPLIFICATION_LEFT                    \
do {      if (node->Parent)                             \
    {                                                   \
        destructNodeRec(node->Left);                    \
        if (node == node->Parent->Right)                \
            connectRight (node->Parent, node->Right);   \
                                                        \
        if (node == node->Parent->Left)                 \
            connectLeft (node->Parent, node->Right);    \
                                                        \
        destructNode (node);                            \
    }                                                   \
    else                                                \
    {                                                   \
        node->myTree->root = node->Right;               \
        node->Right->Parent = NULL;                     \
        destructNode (node->Left);                      \
        destructNode (node);                            \
    }                                                   \
} while (0)


#define STANDARD_SIMPLIFICATION_RIGHT                   \
do {                                                    \
                                                        \
    if (node->Parent)                                   \
    {                                                   \
        destructNodeRec(node->Right);                   \
        if (node == node->Parent->Right)                \
            connectRight (node->Parent, node->Left);    \
                                                        \
        if (node == node->Parent->Left)                 \
            connectLeft (node->Parent, node->Left);     \
                                                        \
        destructNode (node);                            \
                                                        \
    }                                                   \
    else                                                \
    {                                                   \
        node->myTree->root = node->Left;                \
        node->Left->Parent = NULL;                      \
        destructNode (node->Right);                     \
        destructNode (node);                            \
                                                        \
    }                                                   \
} while (0)


#define  CONST_FOLD(operator_)                                                                          \
do {                                                                                                    \
    char **ptrEnd = NULL;                                                                               \
    double val = strtod (node->Left->content, ptrEnd) operator_ strtod (node->Right->content, ptrEnd);  \
                                                                                                        \
    destructNode (node->Left);                                                                          \
    destructNode (node->Right);                                                                         \
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
    node->Left = NULL;                                                                                  \
    node->Right = NULL;                                                                                 \
} while(0)


int simpleTree(Node *node)
{
    if (strcmp(node->content, "*") == 0)
    {

        if (strcmp(node->Left->content, "0") == 0 || strcmp(node->Right->content, "0") == 0)
        {
            destructNodeRec(node->Left);
            destructNodeRec(node->Right);
            *(node->content) = '0';
            node->type = number;
            node->Right = NULL;
            node->Left = NULL;
            TreeChecker++;
            return 0;
        }

        if (strcmp(node->Right->content, "1") == 0)
        {
            STANDARD_SIMPLIFICATION_RIGHT;
            TreeChecker++;
            return 0;

        }

        if (strcmp(node->Left->content, "1") == 0)
        {
            STANDARD_SIMPLIFICATION_LEFT;
            TreeChecker++;
            return 0;
        }

        if (node->Right->type == number && node->Left->type == number)
        {
            CONST_FOLD(*);
            TreeChecker++;
            return 0;
        }

        return 0;
    }

    if (strcmp(node->content, "+") == 0)
    {


        if (strcmp(node->Left->content, "0") == 0)
        {
            STANDARD_SIMPLIFICATION_LEFT;
            TreeChecker++;
            return 0;
        }

        if (strcmp(node->Right->content, "0") == 0)
        {
            STANDARD_SIMPLIFICATION_RIGHT;
            TreeChecker++;
            return 0;
        }

        if (node->Right->type == number && node->Left->type == number)
        {
            CONST_FOLD(+);
            TreeChecker++;
            return 0;
        }
        return 0;
    }

    if (strcmp(node->content, "-") == 0)
    {


        if (strcmp(node->Right->content, "0") == 0)
        {
            STANDARD_SIMPLIFICATION_RIGHT;
            TreeChecker++;
            return 0;
        }

        if (strcmp(node->Left->content, "0") == 0)
        {
            char str1[] = "-1";
            char str2[] = "*";
            nodeSetName(node->Left, str1);
            node->Left->type = number;
            nodeSetName(node, str2);
            node->type = Mul;
            TreeChecker++;
            return 0;
        }

        if (node->Right->type == number && node->Left->type == number)
        {
            CONST_FOLD(-);
            TreeChecker++;
            return 0;
        }
        return 0;
    }

    if (strcmp(node->content, "^") == 0)
    {


        if (strcmp(node->Left->content, "0") == 0)
        {
            destructNodeRec(node->Left);
            destructNodeRec(node->Right);
            *(node->content) = '0';
            node->type = number;
            node->Right = NULL;
            node->Left = NULL;
            TreeChecker++;
            return 0;
        }

        if (strcmp(node->Left->content, "1") == 0)
        {
            destructNodeRec(node->Left);
            destructNodeRec(node->Right);
            *(node->content) = '1';
            node->type = number;
            node->Right = NULL;
            node->Left = NULL;
            TreeChecker++;
            return 0;
        }

        if (strcmp(node->Right->content, "0") == 0)
        {
            destructNodeRec(node->Left);
            destructNodeRec(node->Right);
            *(node->content) = '1';
            node->type = number;
            node->Right = NULL;
            node->Left = NULL;
            TreeChecker++;
            return 0;
        }

        if (strcmp(node->Right->content, "1") == 0)
        {
            STANDARD_SIMPLIFICATION_RIGHT;
            TreeChecker++;
        }
        return 0;
    }

    if (strcmp(node->content, "/") == 0)
    {
        if (node->Right->type == number && node->Left->type == number)
        {
            CONST_FOLD(/);
            TreeChecker++;
            return 0;
        }

        if (strcmp(node->Left->content, "0") == 0)
        {
            destructNodeRec(node->Left);
            destructNodeRec(node->Right);
            *(node->content) = '0';
            node->type = number;
            node->Right = NULL;
            node->Left = NULL;
            TreeChecker++;
            return 0;
        }
    }

}