/*!
 * @file Differentiator.c
 * @brief Realization of @ref Differentiator.h functions.
 * @author Stanislau Shimovolos
 * @date 2018-8-15
 */


#include <math.h>
#include <stdlib.h>

#include "Differentiator.h"

int findDerivative(const Tree *tree, calculator *calc)
{
    assert (tree);
    assert (calc);

    fprintf(calc->texFile, "\\documentclass{article}\n"
                           "\\begin{document} \\begin{center} "
                           "The original expression \n \\[");
    texResult(tree->root, calc->texFile);
    fprintf(calc->texFile, "\\]");

    calc->tree.root = makeDerivativeStep(tree->root, calc);
    simplifyExpression(calc);

    fprintf(calc->texFile, "\n\nThe final expression\n \\[");
    texResult(calc->tree.root, calc->texFile);
    fprintf(calc->texFile, " \\]\n\n\\end{center}\\end{document}\n"
                           "\\end");
    fclose(calc->texFile);
    calc->texFile = NULL;

    system("pdflatex -output-directory Tex/ Tex/Diff_Result");
    system("sensible-browser Tex/Diff_Result.pdf &");

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
            return result;
    }
}

#undef DEF_CMD_UNARY
#undef DEF_CMD


Node *findComplexDerivative(const Node *node, calculator *calc,
                            Node *(func)(const Node *node, calculator *calc))
{
    assert(func);
    assert(node);

    Node *mulNode = createSimpleNode(Mul, &calc->tree);
    connectRight(mulNode, func(node, calc));
    connectLeft(mulNode, makeDerivativeStep(node, calc));
    return mulNode;
}


Node *logDerivative(const Node *node, calculator *calc)
{
    assert(calc);
    assert(node);

    Tree *tree = &calc->tree;

    Node *denominator = copyTree(node, tree);
    Node *mainNode = createSimpleNode(Div, tree);
    Node *leftNode = createNumericalNode(Number, 1, tree);

    connectLeft(mainNode, leftNode);
    connectRight(mainNode, denominator);
    return mainNode;
}


Node *sinDerivative(const Node *node, calculator *calc)
{
    assert(calc);
    assert(node);

    Tree *tree = &calc->tree;

    Node *cosContent = copyTree(node, tree);
    Node *mainNode = createSimpleNode(Cos, tree);

    connectLeft(mainNode, cosContent);
    return mainNode;
}


Node *cosDerivative(const Node *node, calculator *calc)
{
    assert(calc);
    assert(node);

    Tree *tree = &calc->tree;

    Node *sinContent = copyTree(node, tree);
    Node *mainNode = createSimpleNode(Mul, tree);
    Node *minusNode = createNumericalNode(Number, -1, tree);

    Node *sinNode = createSimpleNode(Sin, tree);
    connectLeft(sinNode, sinContent);

    connectRight(mainNode, sinNode);
    connectLeft(mainNode, minusNode);
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

    Node *addNode = createSimpleNode(Add, &calc->tree);
    connectLeft(addNode, dL);
    connectRight(addNode, dR);

    return addNode;
}


Node *subDerivative(const Node *node, calculator *calc)
{
    assert(calc);
    assert(node);
    assert(node->right);
    assert(node->left);

    Node *dL = makeDerivativeStep(node->left, calc);
    Node *dR = makeDerivativeStep(node->right, calc);
    Node *mainNode = createSimpleNode(Sub, &calc->tree);

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

    Tree *tree = &calc->tree;

    Node *LeftCopy = copyTree(node->left, tree);
    Node *RightCopy = copyTree(node->right, tree);

    Node *dL = makeDerivativeStep(node->left, calc);
    Node *dR = makeDerivativeStep(node->right, calc);

    Node *mainNode = createSimpleNode(Add, tree);
    Node *firstMulNode = createSimpleNode(Mul, tree);
    Node *secondMulNode = createSimpleNode(Mul, tree);

    connectRight(firstMulNode, dL);
    connectLeft(firstMulNode, RightCopy);

    connectRight(secondMulNode, dR);
    connectLeft(secondMulNode, LeftCopy);

    connectLeft(mainNode, firstMulNode);
    connectRight(mainNode, secondMulNode);

    return mainNode;
}


Node *divDerivative(const Node *node, calculator *calc)
{
    assert(calc);
    assert(node);
    assert(node->right);
    assert(node->left);

    Tree *tree = &calc->tree;

    Node *leftCopy = copyTree(node->left, tree);
    Node *rightCopy = copyTree(node->right, tree);

    Node *dL = makeDerivativeStep(node->left, calc);
    Node *dR = makeDerivativeStep(node->right, calc);

    Node *minusNode = createSimpleNode(Sub, tree);
    Node *firstMulNode = createSimpleNode(Mul, tree);
    Node *secondMulNode = createSimpleNode(Mul, tree);

    connectRight(firstMulNode, dL);
    connectLeft(firstMulNode, rightCopy);

    connectRight(secondMulNode, dR);
    connectLeft(secondMulNode, leftCopy);

    connectLeft(minusNode, firstMulNode);
    connectRight(minusNode, secondMulNode);

    Node *bottomMul = createSimpleNode(Mul, tree);

    Node *rightCopy1 = copyTree(node->right, tree);
    Node *rightCopy2 = copyTree(node->right, tree);

    connectRight(bottomMul, rightCopy1);
    connectLeft(bottomMul, rightCopy2);

    Node *mainNode = createSimpleNode(Div, tree);

    connectLeft(mainNode, minusNode);
    connectRight(mainNode, bottomMul);

    return mainNode;
}


Node *expDerivative(const Node *node, calculator *calc)
{
    assert(calc);
    assert(node);
    assert(node->right);
    assert(node->left);

    Tree *tree = &calc->tree;

    Node *base1 = copyTree(node->left, tree);
    Node *base2 = copyTree(node->left, tree);
    Node *base4 = copyTree(node->left, tree);

    Node *index2 = copyTree(node->right, tree);
    Node *index3 = copyTree(node->right, tree);
    Node *index4 = copyTree(node->right, tree);

    Node *mainMinus = createSimpleNode(Sub, tree);
    Node *mainPlus = createSimpleNode(Add, tree);
    Node *mulNode1 = createSimpleNode(Mul, tree);
    Node *mulNode2 = createSimpleNode(Mul, tree);
    Node *mulNode3 = createSimpleNode(Mul, tree);
    Node *mulNode4 = createSimpleNode(Mul, tree);

    Node *cap1 = createSimpleNode(Expo, tree);
    Node *cap2 = createSimpleNode(Expo, tree);

    Node *log = createSimpleNode(Log, tree);

    connectLeft(mulNode1, log);
    connectLeft(log, base1);
    connectRight(mulNode1, makeDerivativeStep(node->right, calc));
    connectLeft(mulNode2, mulNode1);
    connectRight(mulNode2, cap1);
    connectLeft(cap1, base2);
    connectRight(cap1, index2);
    connectRight(mulNode2, cap1);
    connectLeft(mainPlus, mulNode2);

    connectRight(mainPlus, mulNode3);
    connectRight(mulNode3, cap2);
    connectLeft(mulNode3, mulNode4);

    connectLeft(mulNode4, index3);
    connectRight(mulNode4, makeDerivativeStep(node->left, calc));
    connectLeft(cap2, base4);
    connectRight(cap2, mainMinus);

    connectRight(mainMinus, createNumericalNode(Number, 1, tree));
    connectLeft(mainMinus, index4);


    return mainPlus;
}


int foldConstants(Node *node)
{
    assert(node);

    double firstVal = node->left->value,
            secondVal = node->right->value;
    double result = 0;

    int operation = node->type;
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
            return 0;
    }

    node->value = result;
    return 1;
}


int simplifyExpression(calculator *calc)
{
    assert(calc);
    assert(calc->tree.root);

    Tree *tree = &calc->tree;

    visitTreePost(tree->root, simplifyTreeNumerical);
    visitTreePost(tree->root, foldConstantsGlobal);

    tree->eventFlag = 1;
    while (tree->eventFlag)
    {
        tree->eventFlag = 0;
        visitTreePost(tree->root, prepareTreeForSimplification);
        visitTreePost(tree->root, simplifyTreeNumerical);
        visitTreePref(tree->root, simplifyTreeMultiplication);
        visitTreePref(tree->root, simplifyTreeAddition);
    }
    visitTreePost(tree->root, prepareTreeForPrinting);

    return 0;
}


int prepareTreeForSimplification(Node *node)
{
    assert(node);

    switch (node->type)
    {
        case Sub:
        {
            node->type = Add;
            if (node->right->type != Number)
            {
                Node *multiplyNode = createSimpleNode(Mul, node->myTree);

                connectRight(multiplyNode, node->right);
                connectLeft(multiplyNode, createNumericalNode(Number, -1, node->myTree));
                connectRight(node, multiplyNode);
            }
            else
                node->right->value = -node->right->value;

            return 1;
        }
        case Mul:
        {
            if (node->left->type == Number && node->right->type == Add)
            {
                double multiplier = node->left->value;
                Node *firstMulNode = createSimpleNode(Mul, node->myTree);
                Node *secondMulNode = createSimpleNode(Mul, node->myTree);

                connectLeft(firstMulNode, createNumericalNode(Number, multiplier, node->myTree));
                connectLeft(secondMulNode, createNumericalNode(Number, multiplier, node->myTree));

                connectRight(firstMulNode, node->right->left);
                connectRight(secondMulNode, node->right->right);

                connectLeft(node->right, firstMulNode);
                connectRight(node->right, secondMulNode);

                if (node == node->parent->left)
                    connectLeft(node->parent, node->right);
                else
                    connectRight(node->parent, node->right);

                destructNode(node->left);
                destructNode(node);

                return 1;
            }
            if (node->right->type == Number
                && node->left->type != Number)
            {
                Node *keeper = node->left;
                connectLeft(node, node->right);
                connectRight(node, keeper);

                return 1;
            }

            int result = compareTrees(node->right, node->left);
            if (!result)
                return 0;

            node->type = Expo;
            destructNodeRec(node->right);
            connectRight(node, createNumericalNode(Number, 2, node->myTree));
        }
        default:
            return 0;
    }
}


int findSimilarTerms(const Node *node, List *lst)
{
    assert(node);
    assert(lst);

    switch (node->type)
    {
        case Add:
        {
            if (node->left->type != Add)
                push(node->left, lst);
            else
                findSimilarTerms(node->left, lst);

            if (node->right->type != Add)
                push(node->right, lst);
            else
                findSimilarTerms(node->right, lst);
            return 1;
        }
        default:
            return 1;
    }
}


int findNumericalMultipliers(const Node *node, List *lst)
{
    switch (node->type)
    {
        case Mul:
        {
            if (node->left->type == Number)
                push(node->left, lst);
            else
                findNumericalMultipliers(node->left, lst);

            if (node->right->type == Number)
                push(node->right, lst);
            else
                findNumericalMultipliers(node->right, lst);
            return 1;
        }
        default:
            return 0;
    }
}


double findNumericalSums(Node *node)
{
    double value = 0;

    if (node->type == Add)
    {
        if (node->left->type == Number)
        {
            value += node->left->value;
            node->left->value = 0;
        }
        else
            value += findNumericalSums(node->left);

        if (node->right->type == Number)
        {
            value += node->right->value;
            node->right->value = 0;
        }
        else
            value += findNumericalSums(node->right);
    }
    return value;
}


int foldConstantsGlobal(Node *node)
{
    if (node->type == Add && (!node->parent || (node->parent && node->parent->type != Add)))
    {
        double sum = findNumericalSums(node);
        if (sum != 0)
        {
            Node *mainPlus = createSimpleNode(Add, node->myTree);
            connectRight(mainPlus, createNumericalNode(Number, sum, node->myTree));

            if (node->parent)
                if (node->parent->left == node)
                    connectLeft(node->parent, mainPlus);
                else
                    connectRight(node->parent, mainPlus);
            else
                node->myTree->root = mainPlus;

            connectLeft(mainPlus, node);
            return 1;
        }
    }
    return 0;

}


int simplifyTreeMultiplication(Node *node)
{
    if (node->type == Mul)
    {
        List lst = {};
        constructList(&lst);
        findNumericalMultipliers(node, &lst);

        Tree *tree = node->myTree;
        if (lst.nodeCounter == 1)
        {
            if (lst.nodeCounter == 1 && lst.start->data != node->left)
            {
                double multiplier = lst.start->data->value;
                lst.start->data->value = 1;

                Node *mainNode = createSimpleNode(Mul, tree);
                connectLeft(mainNode, createNumericalNode(Number, multiplier, tree));

                if (node->parent)
                    if (node->parent->left == node)
                        connectLeft(node->parent, mainNode);
                    else
                        connectRight(node->parent, mainNode);
                else
                    node->myTree->root = mainNode;

                connectRight(mainNode, node);

                visitTreePost(mainNode, simplifyTreeNumerical);
            }
        }
        else if (lst.nodeCounter > 1)
        {
            tree->eventFlag = 1;

            listNode *iterator = lst.start;
            double multiplier = 1;

            while (iterator->next)
            {
                multiplier *= iterator->data->value;
                iterator->data->value = 1;
                iterator = iterator->next;
            }

            Node *mainNode = createSimpleNode(Mul, tree);
            connectLeft(mainNode, createNumericalNode(Number, multiplier, tree));

            if (node->parent)
                if (node->parent->left == node)
                    connectLeft(node->parent, mainNode);
                else
                    connectRight(node->parent, mainNode);
            else
                node->myTree->root = mainNode;

            connectRight(mainNode, node);

            visitTreePost(mainNode, simplifyTreeNumerical);
        }
        destructList(lst.start);
    }
    return 0;
}


int prepareTreeForPrinting(Node *node)
{
    assert(node);

    if (node->type == Add)
    {
        if (node->right->type == Number && node->right->value < 0)
        {
            node->type = Sub;
            node->right->value = -node->right->value;
            return 1;
        }
        if (node->right->type == Mul && node->right->left->value == -1)
        {
            node->type = Sub;
            Node *keeper = node->right;
            connectRight(node, keeper->right);
            destructNode(keeper->left);
            destructNode(keeper);
            return 1;
        }
    }
    return 0;
}


int simplifyTreeAddition(Node *node)
{
    if (node->type == Add)
    {
        List lst = {};
        constructList(&lst);
        findSimilarTerms(node, &lst);

        if (lst.nodeCounter > 1)
        {
            listNode *mainIterator = lst.start;
            listNode *secondIterator = NULL;

            Node *primarySubTree = NULL;
            Node *primaryComparableNode = NULL;

            while (mainIterator->next)
            {
                primarySubTree = mainIterator->data;
                primaryComparableNode = primarySubTree;
                secondIterator = mainIterator->next;

                Node *secondComparableNode = NULL;
                Node *secondSubTree = NULL;

                double primaryMultiplier = 0;
                double secondMultiplier = 0;

                while (secondIterator->next)
                {
                    primaryMultiplier = 1;
                    secondMultiplier = 1;

                    secondComparableNode = secondIterator->data;
                    secondSubTree = secondComparableNode;

                    if (primarySubTree->type == Mul)
                    {
                        if (primarySubTree->left->type == Number)
                        {
                            primaryMultiplier = primarySubTree->left->value;
                            primaryComparableNode = primarySubTree->right;
                        }
                    }
                    if (secondSubTree->type == Mul)
                    {
                        if (secondSubTree->left->type == Number)
                        {
                            secondMultiplier = secondSubTree->left->value;
                            secondComparableNode = secondSubTree->right;
                        }
                    }
                    int result = compareTrees(primaryComparableNode, secondComparableNode);

                    if (result)
                    {
                        node->myTree->eventFlag = 1;

                        if (primaryMultiplier != 1)
                        {
                            destructNode(primarySubTree->left);

                            if (primarySubTree == primarySubTree->parent->left)
                                connectLeft(primarySubTree->parent, primaryComparableNode);
                            else
                                connectRight(primarySubTree->parent, primaryComparableNode);

                            destructNode(primarySubTree);
                            primarySubTree = primaryComparableNode;
                        }
                        if (secondMultiplier != 1)
                        {
                            destructNode(secondSubTree->left);

                            if (secondSubTree == secondSubTree->parent->left)
                                connectLeft(secondSubTree->parent, secondComparableNode);
                            else
                                connectRight(secondSubTree->parent, secondComparableNode);

                            destructNode(secondSubTree);
                            secondSubTree = secondComparableNode;
                        }

                        double coreMultiplier = primaryMultiplier + secondMultiplier;
                        listNode *secondIteratorKeeper = secondIterator->next;

                        primarySubTree = mergeTrees(primarySubTree, secondSubTree, coreMultiplier);
                        eraseNode(secondIterator);

                        secondIterator = secondIteratorKeeper;
                        continue;
                    }
                    else
                        secondIterator = secondIterator->next;
                }
                mainIterator = mainIterator->next;
            }
        }
        destructList(lst.start);
    }
    return 0;
}


Node *mergeTrees(Node *main, Node *filial, double value)
{
    assert(main);
    assert(filial);

    Node *fakeFilial = createNumericalNode(Number, 0, filial->myTree);

    if (filial->parent->right == filial)
        connectRight(filial->parent, fakeFilial);
    else
        connectLeft(filial->parent, fakeFilial);

    Node *multiplyNode = createSimpleNode(Mul, main->myTree);

    if (!main->parent)
        main->myTree->root = multiplyNode;
    else
    {
        if (main->parent->right == main)
            connectRight(main->parent, multiplyNode);
        else
            connectLeft(main->parent, multiplyNode);
    }

    connectRight(multiplyNode, main);
    connectLeft(multiplyNode, createNumericalNode(Number, value, filial->myTree));

    destructNodeRec(filial);
    return multiplyNode;
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


int simplifyTreeNumerical(Node *node)
{
    switch (node->type)
    {
        case Mul:
        {
            if (node->left->type == Number)
            {
                if (node->left->value == 1)
                {
                    SIMPLIFY_LEFT_SIDE;
                    return 1;
                }

                if (node->left->value == 0)
                {
                    REPLACE_BY_VALUE(0);
                    return 1;
                }
                if (node->right->type == Number)
                {
                    foldConstants(node);
                    return 1;
                }
                return 0;
            }
            if (node->right->type == Number)
            {
                if (node->right->value == 1)
                {
                    SIMPLIFY_RIGHT_SIDE;
                    return 1;
                }
                if (node->right->value == 0)
                {
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
                    SIMPLIFY_LEFT_SIDE;
                    return 1;
                }
                if (node->right->type == Number)
                {
                    foldConstants(node);
                    return 1;
                }
                return 0;
            }
            if (node->right->type == Number)
            {
                if (node->right->value == 0)
                {
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
                    node->left->value = -1;
                    node->type = Mul;
                    return 1;
                }
                if (node->right->type == Number)
                {
                    foldConstants(node);
                    return 1;
                }
                return 0;
            }
            if (node->right->type == Number)
            {
                if (node->right->value == 0)
                {
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
                    REPLACE_BY_VALUE(0);
                    return 1;
                }
                if (node->left->value == 1)
                {
                    REPLACE_BY_VALUE(1);
                    return 1;
                }
                if (node->right->type == Number)
                {
                    foldConstants(node);
                    return 1;
                }
                return 0;
            }
            if (node->right->type == Number)
            {
                if (node->right->value == 0)
                {
                    REPLACE_BY_VALUE(1);
                    return 1;
                }
                if (node->right->value == 1)
                {
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
                    SIMPLIFY_RIGHT_SIDE;
                    return 1;
                }
                if (node->left->type == Number)
                {
                    foldConstants(node);
                    return 1;
                }
                return 0;
            }
            if (node->left->type == Number)
            {
                if (node->left->value == 0)
                {
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
    texResult(original, outFileName);
    fprintf(outFileName, ")' = ");
    texResult(result, outFileName);
    fprintf(outFileName, "\\]\n");

    return 0;
}


int texResult(const Node *node, FILE *outFileName)
{
    switch (node->type)
    {
        case Add :
        {
            if (node->parent && node->parent->type == Mul)
            {
                fprintf(outFileName, "(");
                if (node->right->type == Sub)
                {
                    texResult(node->left, outFileName);
                    fprintf(outFileName, " + (");
                    texResult(node->right, outFileName);
                    fprintf(outFileName, ")");
                }
                else
                {
                    texResult(node->left, outFileName);
                    fprintf(outFileName, " + ");
                    texResult(node->right, outFileName);
                }
                fprintf(outFileName, ")");
            }
            else
            {
                if (node->right->type == Sub)
                {
                    texResult(node->left, outFileName);
                    fprintf(outFileName, " + (");
                    texResult(node->right, outFileName);
                    fprintf(outFileName, ")");
                }
                else
                {
                    texResult(node->left, outFileName);
                    fprintf(outFileName, " + ");
                    texResult(node->right, outFileName);
                }
            }
            return 0;
        }
        case Sub :
        {
            if (node->parent && node->parent->type == Mul)
            {
                fprintf(outFileName, "(");
                if (node->right->type == Add)
                {
                    texResult(node->left, outFileName);
                    fprintf(outFileName, " - (");
                    texResult(node->right, outFileName);
                    fprintf(outFileName, ")");
                }
                else
                {
                    texResult(node->left, outFileName);
                    fprintf(outFileName, " - ");
                    texResult(node->right, outFileName);
                }
                fprintf(outFileName, ")");
            }
            else
            {
                if (node->right->type == Add)
                {
                    texResult(node->left, outFileName);
                    fprintf(outFileName, " - (");
                    texResult(node->right, outFileName);
                    fprintf(outFileName, ")");
                }
                else
                {
                    texResult(node->left, outFileName);
                    fprintf(outFileName, " - ");
                    texResult(node->right, outFileName);
                }
            }
            return 0;
        }
        case Log :
        {
            fprintf(outFileName, "\\log(");
            texResult(node->left, outFileName);
            fprintf(outFileName, ")");
            return 0;
        }
        case Cos :
        {
            fprintf(outFileName, "\\cos(");
            texResult(node->left, outFileName);
            fprintf(outFileName, ")");
            return 0;
        }
        case Sin :
        {
            fprintf(outFileName, "\\sin(");
            texResult(node->left, outFileName);
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
            texResult(node->left, outFileName);
            fprintf(outFileName, "\\cdot ");
            texResult(node->right, outFileName);
            return 0;
        }
        case Div :
        {
            fprintf(outFileName, "\\frac{");
            texResult(node->left, outFileName);
            fprintf(outFileName, "}{");
            texResult(node->right, outFileName);
            fprintf(outFileName, "}");
            return 0;
        }
        case Expo :
        {
            if (node->left->type == Number ||
                node->left->type == CharConst ||
                node->left->type == CurVariable)
            {
                texResult(node->left, outFileName);
                fprintf(outFileName, "^{");
                texResult(node->right, outFileName);
                fprintf(outFileName, "}");
            }
            else
            {
                fprintf(outFileName, "(");
                texResult(node->left, outFileName);
                fprintf(outFileName, ")^{");
                texResult(node->right, outFileName);
                fprintf(outFileName, "}");
            }
            return 0;
        }
        default:
            return 1;
    }
}


int eraseNode(listNode *node)
{
    assert(node);
    if (node->prev)
        node->prev->next = node->next;
    if (node->next)
        node->next->prev = node->prev;

    node->parent->nodeCounter--;
    free(node);
    return 0;
}


int constructList(List *lst)
{
    assert(lst);
    lst->current = (listNode *) calloc(1, sizeof(listNode));
    lst->start = lst->current;
    lst->current->parent = lst;
    lst->current->next = NULL;
    lst->current->prev = NULL;
    lst->current->data = NULL;

    lst->nodeCounter = 0;
    return 0;
}


int push(Node *node, List *lst)
{
    assert(node);
    assert(lst);

    listNode *new = (listNode *) calloc(1, sizeof(listNode));
    new->parent = lst;
    new->prev = lst->current;
    new->next = NULL;
    new->data = NULL;

    lst->nodeCounter++;
    lst->current->data = node;
    lst->current->next = new;
    lst->current = new;
    return 0;
}


int dumpList(List *lst)
{
    assert(lst);

    listNode *current = lst->start;
    while (current->next)
    {
        printf("%p\n", current->data);
        current = current->next;
    }

    return 0;
}


int destructList(listNode *node)
{
    if (node->next)
        destructList(node->next);

    free(node);
    return 0;
}
