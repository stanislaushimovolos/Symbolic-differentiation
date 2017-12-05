#include <assert.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include "../Tree_t/Tree.h"
#include <stdlib.h>
#include <math.h>

#include "Diff.h"

enum
{
	number, binOperator_, unaryOperator_, curVariable, charConst, Add, Sub, Mul, Div, Ln, Sin, Cos, Expo

};

char *getBufferFromFileGetSzOfBuf (const char *_inputFileName, size_t *sizeOfBuffer)
{

	assert (_inputFileName);

	size_t sizeOfBuffer1 = 0;
	FILE *file = fopen (_inputFileName, "r");

	assert (file);

	fseek (file, 0, SEEK_END);
	sizeOfBuffer1 = (size_t) ftell (file);
	fseek (file, 0, SEEK_SET);

	char *helpBuffer = (char *) calloc (sizeOfBuffer1 + 2, sizeof (char));
	fread (helpBuffer, 1, sizeOfBuffer1, file);
	fclose (file);

	if (sizeOfBuffer != NULL)
		*sizeOfBuffer = sizeOfBuffer1;

	return helpBuffer;
}


#define  DEF_CMD(operator_, number, code)                   \
    else if (strcmp (#operator_, node->content) == 0)       \
     {                                                      \
        node->type = number;                                \
    }


char contentAnalyze (Node *node, const char *const currValue)
{

	char *NumHelper = (node->content);
	strtod (node->content, &NumHelper);

	if (NumHelper != node->content)
		node->type = number;

	else if (strcmp (currValue, node->content) == 0)
		node->type = curVariable;


#include "MathFunc.h"

	else
		node->type = charConst;

}

#undef DEF_CMD


Node *diffMain (const Tree *const BegTree, Tree *FinalTree, const char *const currValue)
{
	assert (BegTree);
	assert (FinalTree);
	assert (currValue);

	if (!BegTree->root->content)
		return 0;

	Node *helpNode = {};
	nodeConstruct (&helpNode);

	helpNode = NodeCopy (BegTree->root, FinalTree);
	helpNode->myTree = FinalTree;

	FinalTree->root = diffRec (helpNode, currValue, FinalTree);

	FinalTree->nodeAmount = 0;

	treeVisitorInf (FinalTree->root, simpleTree);
	treeVisitorInf (FinalTree->root, nodeCount);

}


#define  DEF_CMD(operator_, number, code)                   \
    case number :                                         \
     {                                                      \
       code                                                 \
    }


Node *diffRec (const Node *const node, const char *const currValue, Tree *FinalTree)
{
	switch (node->type)
	{
		case number:
			return createNode (number, 0, FinalTree);

		case curVariable:
			return createNode (curVariable, 1, FinalTree);


		case charConst:
			return createNode (charConst, 0, FinalTree);

#include "MathFunc.h"


		default:
			break;
	}
}


#undef DEF_CMD_BIN_OPERATOR

Node *createNode (const char type, int value, Tree *FinalTree)
{
	switch (type)
	{
		case number:
		{
			Node *helpNode = {};
			nodeConstruct (&helpNode);

			helpNode->myTree = FinalTree;

			helpNode->content = (char *) calloc (2, sizeof (char));
			itoa (value, helpNode->content, 10);
			helpNode->type = number;

			return helpNode;
		}

		case charConst:
		{
			Node *helpNode = {};
			nodeConstruct (&helpNode);

			helpNode->myTree = FinalTree;

			helpNode->content = (char *) calloc (2, sizeof (char));
			itoa (value, helpNode->content, 10);
			helpNode->type = number;

			return helpNode;

		}

		case curVariable:
		{
			Node *helpNode = {};
			nodeConstruct (&helpNode);

			helpNode->myTree = FinalTree;

			helpNode->content = (char *) calloc (2, sizeof (char));
			itoa (value, helpNode->content, 10);
			helpNode->type = number;

			return helpNode;
		}

		default:
			break;
	}
}


Node *createNode (const char type, char operator__, Tree *FinalTree)
{
	Node *mainNode = {};
	nodeConstruct (&mainNode);

	mainNode->myTree = FinalTree;
	mainNode->content = (char *) calloc (2, sizeof (char));

	*mainNode->content = operator__;
	mainNode->type = type;
	return mainNode;
}


Node *createNode (const char type, const char *const func, Tree *FinalTree)
{
	assert (func);
	assert (FinalTree);

	Node *mainNode = {};
	nodeConstruct (&mainNode);

	mainNode->myTree = FinalTree;
	mainNode->content = (char *) calloc (strlen (func) + 1, sizeof (char));

	strcpy (mainNode->content, func);
	mainNode->type = type;
	return mainNode;
}


Node *complicatedDiff (Node *mainNode, Tree *FinalTree, const char *const currValue,
                       Node *(diff) (Node *node, Tree *FinalTree, const char *const currValue))
{
	Node *mul = createNode (binOperator_, '*', FinalTree);
	connectLeft (mul, diff (mainNode, FinalTree, currValue));
	connectRight (mul, diffRec (mainNode, currValue, FinalTree));

	return mul;
}


#define STANDARD_SIMPLIFICATION_LEFT                    \
    if (node->Parent)                                   \
    {                                                   \
        destructTreeRec(node->Left);                    \
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
    return 0;


#define STANDARD_SIMPLIFICATION_RIGHT                   \
                                                        \
    if (node->Parent)                                   \
    {                                                   \
        destructTreeRec(node->Right);                   \
        if (node == node->Parent->Right)                \
            connectRight (node->Parent, node->Left);    \
                                                        \
        if (node == node->Parent->Left)                 \
            connectLeft (node->Parent, node->Left);     \
                                                        \
        destructNode (node);                            \
        return 0;                                       \
    }                                                   \
    else                                                \
    {                                                   \
        node->myTree->root = node->Left;                \
        node->Left->Parent = NULL;                      \
        destructNode (node->Right);                     \
        destructNode (node);                            \
        return 0;                                       \
    }


#define  CONST_FOLD(operator_)                                                                          \
    char **ptrEnd = NULL;                                                                               \
    double val = strtod (node->Left->content, ptrEnd) operator_ strtod (node->Right->content, ptrEnd);  \
    double FrVal = 0;                                                                                   \
    double WhVal = 0;                                                                                   \
                                                                                                        \
    destructNode (node->Left);                                                                          \
    destructNode (node->Right);                                                                         \
                                                                                                        \
    FrVal = modf (val, &WhVal);                                                                         \
    int numCounter = 0;                                                                                 \
                                                                                                        \
        int helper = (int) WhVal;                                                                       \
        while (helper > 0)                                                                              \
        {                                                                                               \
            helper /= 10;                                                                               \
            numCounter++;                                                                               \
        }                                                                                               \
                                                                                                        \
                                                                                                        \
    node->content = (char *) calloc ((size_t) (numCounter + 1), sizeof (char));                         \
    sprintf (node->content, "%d", (int) WhVal);                                                         \
    node->type = number;                                                                                \
    node->Left = NULL;                                                                                  \
    node->Right = NULL;                                                                                 \
    return 0;                                                                                           \


int simpleTree (Node *node)
{


	if (strcmp (node->content, "*") == 0)
	{

		if (strcmp (node->Left->content, "0") == 0 || strcmp (node->Right->content, "0") == 0)
		{
			destructTreeRec (node->Left);
			destructTreeRec (node->Right);
			*(node->content) = '0';
			node->type = number;
			node->Right = NULL;
			node->Left = NULL;
			return 0;
		}

		if (strcmp (node->Right->content, "1") == 0)
		{
			STANDARD_SIMPLIFICATION_RIGHT

		}

		if (strcmp (node->Left->content, "1") == 0)
		{
			STANDARD_SIMPLIFICATION_LEFT
		}

		if (node->Right->type == number && node->Left->type == number)
		{
			CONST_FOLD(*);
		}
	}

	if (strcmp (node->content, "+") == 0)
	{


		if (strcmp (node->Left->content, "0") == 0)
		{
			STANDARD_SIMPLIFICATION_LEFT
		}

		if (strcmp (node->Right->content, "0") == 0)
		{
			STANDARD_SIMPLIFICATION_RIGHT
		}

		if (node->Right->type == number && node->Left->type == number)
		{
			CONST_FOLD(+);
		}
		return 0;
	}

	if (strcmp (node->content, "-") == 0)
	{


		if (strcmp (node->Right->content, "0") == 0)
		{
			STANDARD_SIMPLIFICATION_RIGHT
		}

		if (strcmp (node->Left->content, "0") == 0)
		{
			char str1[] = "-1";
			char str2[] = "*";
			nodeSetName (node->Left, str1);
			nodeSetName (node, str2);
			return 0;
		}

		if (node->Right->type == number && node->Left->type == number)
		{
			CONST_FOLD(-);
		}
		return 0;
	}

	if (strcmp (node->content, "^") == 0)
	{


		if (strcmp (node->Left->content, "0") == 0)
		{
			destructTreeRec (node->Left);
			destructTreeRec (node->Right);
			*(node->content) = '0';
			node->type = number;
			node->Right = NULL;
			node->Left = NULL;
			return 0;
		}

		if (strcmp (node->Left->content, "1") == 0)
		{
			destructTreeRec (node->Left);
			destructTreeRec (node->Right);
			*(node->content) = '1';
			node->type = number;
			node->Right = NULL;
			node->Left = NULL;
			return 0;
		}

		if (strcmp (node->Right->content, "0") == 0)
		{
			destructTreeRec (node->Left);
			destructTreeRec (node->Right);
			*(node->content) = '1';
			node->type = number;
			node->Right = NULL;
			node->Left = NULL;
			return 0;
		}

		if (strcmp (node->Right->content, "1") == 0)
		{
			STANDARD_SIMPLIFICATION_RIGHT
		}
	}


}


Node *lnDiff (Node *node, Tree *FinalTree, const char *const currValue)
{
	Node *mainNode = createNode (binOperator_, '/', FinalTree);
	Node *leftNode = createNode (number, 1, FinalTree);
	connectLeft (mainNode, leftNode);
	connectRight (mainNode, node);
	return mainNode;
}


Node *sinDiff (Node *node, Tree *FinalTree, const char *const currValue)
{
	char cos[] = "cos";
	Node *mainNode = createNode (binOperator_, cos, FinalTree);
	connectLeft (mainNode, node);
	return mainNode;
}


Node *cosDiff (Node *node, Tree *FinalTree, const char *const currValue)
{
	char sin[] = "sin";

	Node *mainNode = createNode (binOperator_, '*', FinalTree);
	Node *rightNode = createNode (number, -1, FinalTree);

	Node *leftNode = createNode (binOperator_, sin, FinalTree);
	connectLeft (leftNode, node);

	connectRight (mainNode, rightNode);
	connectLeft (mainNode, leftNode);

	return mainNode;
}


Node *addDiff (const Node *const node, Tree *FinalTree, const char *const currValue)
{
	Node *LeftCopy = NodeCopy (node->Left, FinalTree);
	LeftCopy->myTree = FinalTree;

	Node *RightCopy = NodeCopy (node->Right, FinalTree);
	RightCopy->myTree = FinalTree;

	Node *dL = diffRec (LeftCopy, currValue, FinalTree);
	Node *dR = diffRec (RightCopy, currValue, FinalTree);

	Node *mainNode = createNode (binOperator_, *node->content, FinalTree);

	connectLeft (mainNode, dL);
	connectRight (mainNode, dR);

	return mainNode;
}


Node *subDiff (const Node *const node, Tree *FinalTree, const char *const currValue)
{

	Node *LeftCopy = NodeCopy (node->Left, FinalTree);
	LeftCopy->myTree = FinalTree;

	Node *RightCopy = NodeCopy (node->Right, FinalTree);
	RightCopy->myTree = FinalTree;

	Node *dL = diffRec (LeftCopy, currValue, FinalTree);
	Node *dR = diffRec (RightCopy, currValue, FinalTree);

	Node *mainNode = createNode (binOperator_, *node->content, FinalTree);

	connectLeft (mainNode, dL);
	connectRight (mainNode, dR);

	return mainNode;
}


Node *mulDiff (const Node *const node, Tree *FinalTree, const char *const currValue)
{

	Node *LeftCopy = NodeCopy (node->Left, FinalTree);
	LeftCopy->myTree = FinalTree;

	Node *RightCopy = NodeCopy (node->Right, FinalTree);
	RightCopy->myTree = FinalTree;

	Node *dL = diffRec (LeftCopy, currValue, FinalTree);
	Node *dR = diffRec (RightCopy, currValue, FinalTree);

	Node *mainNode = createNode (binOperator_, '+', FinalTree);

	Node *FirstProNode = createNode (binOperator_, *node->content, FinalTree);
	Node *SecondProNode = createNode (binOperator_, *node->content, FinalTree);

	connectRight (FirstProNode, dL);
	connectLeft (FirstProNode, RightCopy);

	connectRight (SecondProNode, dR);
	connectLeft (SecondProNode, LeftCopy);

	connectLeft (mainNode, FirstProNode);
	connectRight (mainNode, SecondProNode);

	return mainNode;

}


Node *divDiff (const Node *const node, Tree *FinalTree, const char *const currValue)
{
	Node *LeftCopy = NodeCopy (node->Left, FinalTree);
	LeftCopy->myTree = FinalTree;

	Node *RightCopy = NodeCopy (node->Right, FinalTree);
	RightCopy->myTree = FinalTree;

	Node *dL = diffRec (LeftCopy, currValue, FinalTree);
	Node *dR = diffRec (RightCopy, currValue, FinalTree);

	Node *minusNode = createNode (binOperator_, '-', FinalTree);

	Node *FirstProNode = createNode (binOperator_, '*', FinalTree);
	Node *SecondProNode = createNode (binOperator_, '*', FinalTree);

	connectRight (FirstProNode, dL);
	connectLeft (FirstProNode, RightCopy);

	connectRight (SecondProNode, dR);
	connectLeft (SecondProNode, LeftCopy);

	connectLeft (minusNode, FirstProNode);
	connectRight (minusNode, SecondProNode);

	Node *denPro = createNode (binOperator_, '*', FinalTree);

	Node *RightCopy1 = NodeCopy (node->Right, FinalTree);
	RightCopy1->myTree = FinalTree;


	Node *RightCopy2 = NodeCopy (node->Right, FinalTree);
	RightCopy2->myTree = FinalTree;

	connectRight (denPro, RightCopy1);
	connectLeft (denPro, RightCopy2);

	Node *MainNode = createNode (binOperator_, '/', FinalTree);

	connectLeft (MainNode, minusNode);
	connectRight (MainNode, denPro);

	return MainNode;

}

Node *expoDiff (const Node *const node, Tree *FinalTree, const char *const currValue)
{
	char ln[] = "ln";

	Node *base1 = NodeCopy (node->Left, FinalTree);
	Node *base2 = NodeCopy (node->Left, FinalTree);
	Node *base3 = NodeCopy (node->Left, FinalTree);
	Node *base4 = NodeCopy (node->Left, FinalTree);

	Node *index1 = NodeCopy (node->Right, FinalTree);
	Node *index2 = NodeCopy (node->Right, FinalTree);
	Node *index3 = NodeCopy (node->Right, FinalTree);
	Node *index4 = NodeCopy (node->Right, FinalTree);

	Node *mainMinus = createNode (binOperator_, '-', FinalTree);
	Node *MainPlus = createNode (binOperator_, '+', FinalTree);
	Node *ProNode1 = createNode (binOperator_, '*', FinalTree);
	Node *ProNode2 = createNode (binOperator_, '*', FinalTree);
	Node *ProNode3 = createNode (binOperator_, '*', FinalTree);
	Node *ProNode4 = createNode (binOperator_, '*', FinalTree);

	Node *Cap1 = createNode (binOperator_, '^', FinalTree);
	Node *Cap2 = createNode (binOperator_, '^', FinalTree);

	Node *Ln = {};
	nodeConstruct (&Ln);
	nodeSetName (Ln, ln);
	Ln->type = unaryOperator_;

	connectLeft (ProNode1, Ln);
	connectLeft (Ln, base1);
	connectRight (ProNode1, diffRec (index1, currValue, FinalTree));
	connectLeft (ProNode2, ProNode1);
	connectRight (ProNode2, Cap1);
	connectLeft (Cap1, base2);
	connectRight (Cap1, index2);
	connectRight (ProNode2, Cap1);
	connectLeft (MainPlus, ProNode2);

	connectRight (MainPlus, ProNode3);
	connectRight (ProNode3, Cap2);
	connectLeft (ProNode3, ProNode4);

	connectLeft (ProNode4, index3);
	connectRight (ProNode4, diffRec (base3, currValue, FinalTree));
	connectLeft (Cap2, base4);
	connectRight (Cap2, mainMinus);

	connectRight (mainMinus, createNode (number, 1, FinalTree));
	connectLeft (mainMinus, index4);

	return MainPlus;
}



