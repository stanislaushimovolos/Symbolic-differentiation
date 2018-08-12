#include <iostream>
#include <assert.h>
#include <cstring>
#include <string.h>

#include "Tree.h"

void connectRight(Node *mainNode, Node *rightNode)
{
    assert (rightNode);
    assert (mainNode);

    mainNode->right = rightNode;
    rightNode->myTree = mainNode->myTree;
    rightNode->parent = mainNode;
    mainNode->myTree->nodeAmount++;
}


void connectLeft(Node *mainNode, Node *leftNode)
{
    assert (leftNode);
    assert (mainNode);

    mainNode->left = leftNode;
    leftNode->myTree = mainNode->myTree;
    leftNode->parent = mainNode;
    mainNode->myTree->nodeAmount++;
}

void visitTree(Node *node, int func(Node *node))
{
    if (node->left)
        visitTree(node->left, func);
    if (node->right)
        visitTree(node->right, func);
    func(node);
}

int countNodes(Node *node)
{
    node->myTree->nodeAmount++;
    return 0;
}

#define RETURN_COND(cond) return (cond) ? 0 : 1;


int constructNode(Node **node)
{
    Node *elem = (Node *) calloc(1, sizeof(Node));
    *node = elem;
    RETURN_COND (*node)
}


int nodeSetName(Node *node, const char *content)
{
    node->content = (char *) calloc(strlen(content) + 1, sizeof(char));
    strcpy(node->content, content);
    RETURN_COND (node->content)
}


Node *copyTree(const Node *node, Tree *newTree)
{
    assert (node);
    Node *retNode;
    constructNode(&retNode);

    retNode->type = node->type;
    retNode->value = node->value;

    if (node->content)
    {
        retNode->content = (char *) calloc(strlen(node->content) + 1, sizeof(char));
        strcpy(retNode->content, node->content);
    }
    retNode->myTree = node->myTree;
    if (node->left)
    {
        retNode->left = copyTree(node->left, newTree);
        retNode->left->parent = retNode;
        retNode->left->myTree = newTree;
    }
    if (node->right)
    {
        retNode->right = copyTree(node->right, newTree);
        retNode->right->parent = retNode;
        retNode->right->myTree = newTree;
    }
    return retNode;
}

int treeConstruct(Tree *tree)
{
    assert (tree);
    tree->root = NULL;
    tree->eventFlag = 0;
    tree->nodeAmount = 0;
    return 0;
}


void destructNode(Node *node)
{
    free(node->content);
    free(node);
}


void destructTree(Tree *tree)
{
    if (tree->root)
    {
        destructNodeRec(tree->root);
        (tree->root) = NULL;
    }
}


void destructNodeRec(Node *node)
{
    if (node->left)
        destructNodeRec(node->left);
    if (node->right)
        destructNodeRec(node->right);
    destructNode(node);
}


Node *createLiteralNode(int type, const char *content, Tree *newTree)
{
    assert (content);
    assert (newTree);

    Node *mainNode = {};
    constructNode(&mainNode);

    mainNode->myTree = newTree;
    mainNode->content = (char *) calloc(strlen(content) + 1, sizeof(char));

    strcpy(mainNode->content, content);
    mainNode->type = type;
    return mainNode;
}


Node *createTypeNode(int type, Tree *newTree)
{
    assert(newTree);

    Node *mainNode = {};
    constructNode(&mainNode);
    mainNode->myTree = newTree;

    mainNode->type = type;
    return mainNode;
}


Node *createNumericalNode(int type, double value, Tree *newTree)
{
    assert(newTree);

    Node *mainNode = {};
    constructNode(&mainNode);
    mainNode->type = type;

    mainNode->value = value;
    return mainNode;
}


#define PRINTOUT_REC(function)                                                         \
    if (n->left != NULL)                                                               \
        function (n->left, NodeCounter, outPictureFile1);                              \
    if (n->right != NULL)                                                              \
       function (n->right, NodeCounter, outPictureFile1);                              \
    if (n->parent != NULL)                                                             \
        fprintf (outPictureFile1, "\nNode%p -> Node%p\n", n->parent, n);


#define CYCLE_CHEACK                                                            \
    static int CycleChecker;                                                    \
    if (n->myTree->nodeAmount == (*NodeCounter - 1) || CycleChecker == 1) {     \
        CycleChecker = 1;                                                       \
    return CycleChecker;                                                        \
    } else                                                                      \
    CycleChecker = 0;


int dumpRecNode(const Node *n, int *NodeCounter, FILE *outPictureFile1)
{
    fprintf(outPictureFile1,
            "Node%p [shape = record,  color = red, label = \"{ { %s| '%p' } | { parent| '%p' }  | { type | '%d' } | { Tree| '%p' }| ",
            n, (char *) n->content, n, n->parent, n->type, n->myTree);

    fprintf(outPictureFile1, "{ left = %p ", n->left);
    fprintf(outPictureFile1, "| right = %p } }\"]\n", n->right);

    (*NodeCounter)++;

    CYCLE_CHEACK
    PRINTOUT_REC(dumpRecNode)
    return CycleChecker;
}


int printRecNode(const Node *n, int *NodeCounter, FILE *outPictureFile1)
{
    if (n)
        fprintf(outPictureFile1, "Node%p [shape = record,  color = blue, label = \" { %s }\"] ", n,
                n->content);
    (*NodeCounter)++;

    CYCLE_CHEACK
    PRINTOUT_REC(printRecNode);
    return CycleChecker;
}


#undef CYCLE_CHEACK
#undef PRINTOUT_REC


#define PRINTOUT_GLOBAL(function)                                           \
    assert (tree->root);                                                    \
                                                                            \
                                                                            \
    int NodeCounter = 0;                                                    \
                                                                            \
    fprintf (outPictureFile, "digraph dump\n" "{\n");                       \
                                                                            \
    NodeCounter= function (tree->root, &NodeCounter, outPictureFile) ;      \
                                                                            \
    fprintf (outPictureFile, "}");                                          \
                                                                            \
    fclose (outPictureFile);                                                \


int dumpTreePicture(const Tree *tree, const char *outFileName)
{
    FILE *outPictureFile = fopen(outFileName, "w");
    PRINTOUT_GLOBAL (dumpRecNode)

    system("dot -Tpng picture/dotDump.txt -o picture/Dump.png");
    return NodeCounter;
}


int printTree(const Tree *tree, const char *outFileName)
{
    FILE *outPictureFile = fopen(outFileName, "w");
    PRINTOUT_GLOBAL (printRecNode)

    system("dot -Tpng picture/dotPrint.txt -o picture/Print.png");
    return NodeCounter;
}

#undef PRINTOUT_GLOBAL