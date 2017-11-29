#include <assert.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include "../Tree_t/Tree.h"
#include <stdlib.h>
#include <math.h>

#include "Diff.h"


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


#define DEF_CMD_BIN_OPERATOR(operator_, number, code)           \
    else if (strcmp (#operator_, node->content) == 0)            \
     {                                                      \
        node->type = number;                                \
    }

#define DEF_CMD_UNARY_OPERATOR(operator_, number)      \
    else if (strcmp (#operator_, node->content) == 0)        \
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


#include "BinaryMathFunc.h"

#include "UnaryMathFunc.h"


	else
		node->type = charConst;

}


#undef DEF_CMD_BIN_OPERATOR
#undef DEF_CMD_UNARY_OPERATOR


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


#define DEF_CMD_BIN_OPERATOR(operator_, number, code)       \
    if (strcmp (#operator_, node->content) == 0)            \
     {                                                      \
        code                                                \
    }

#define DEF_CMD_UNARY_OPERATOR(operator_, number)                           \
     if (strcmp (#operator_, node->content) == 0)                                   \
    {                                                                               \
        Node *LeftCopy = NodeCopy (node->Left, FinalTree);                          \
        return complicatedDiff (LeftCopy, FinalTree, MainVariable, operator_##Diff);                                                           \
    }


Node *diffRec (const Node *const node, const char *const currValue, Tree *FinalTree)
{
	switch (node->type)
	{
		case number:
			return createNode (number, 0, FinalTree);

		case curVariable:
			return createNode (curVariable, 1, FinalTree);

		case binOperator_:
		{

#include "BinaryMathFunc.h"

		}

		case unaryOperator_:
		{

#include "UnaryMathFunc.h"

		}

		case charConst:
			return createNode (charConst, 0, FinalTree);

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
                       Node *(diff) (Node *node, Tree *FinalTree))
{
	Node *mul = createNode (binOperator_, '*', FinalTree);
	connectLeft (mul, diff (mainNode, FinalTree));
	connectRight (mul, diffRec (mainNode, currValue, FinalTree));

	return mul;
}


Node *lnDiff (Node *node, Tree *FinalTree)
{
	Node *mainNode = createNode (binOperator_, '/', FinalTree);
	Node *leftNode = createNode (number, 1, FinalTree);
	connectLeft (mainNode, leftNode);
	connectRight (mainNode, node);
	return mainNode;
}

Node *sinDiff (Node *node, Tree *FinalTree)
{
	char cos[] = "cos";
	Node *mainNode = createNode (binOperator_, cos, FinalTree);
	connectLeft (mainNode, node);
	return mainNode;
}

Node *cosDiff (Node *node, Tree *FinalTree)
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
    break;


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
        break;                                          \
    }                                                   \
    else                                                \
    {                                                   \
        node->myTree->root = node->Left;                \
        node->Left->Parent = NULL;                      \
        destructNode (node->Right);                     \
        destructNode (node);                            \
        break;                                          \
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
    break;                                                                                              \


void simpleTree (Node *node)
{
	if (node->type == binOperator_)
	{
		switch (*(node->content))
		{
			case '*':
			{
				if (strcmp (node->Left->content, "0") == 0 || strcmp (node->Right->content, "0") == 0)
				{
					destructTreeRec (node->Left);
					destructTreeRec (node->Right);
					*(node->content) = '0';
					node->type = number;
					node->Right = NULL;
					node->Left = NULL;
					break;
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
				break;
			}
			case '+':
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
				break;

			}

			case '-':
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
					break;
				}

				if (node->Right->type == number && node->Left->type == number)
				{
					CONST_FOLD(-);
				}
				break;

			}

			case '^':
			{
				if (strcmp (node->Left->content, "0") == 0)
				{
					destructTreeRec (node->Left);
					destructTreeRec (node->Right);
					*(node->content) = '0';
					node->type = number;
					node->Right = NULL;
					node->Left = NULL;
					break;
				}

				if (strcmp (node->Left->content, "1") == 0)
				{
					destructTreeRec (node->Left);
					destructTreeRec (node->Right);
					*(node->content) = '1';
					node->type = number;
					node->Right = NULL;
					node->Left = NULL;
					break;
				}

				if (strcmp (node->Right->content, "0") == 0)
				{
					destructTreeRec (node->Left);
					destructTreeRec (node->Right);
					*(node->content) = '1';
					node->type = number;
					node->Right = NULL;
					node->Left = NULL;
					break;
				}

				if (strcmp (node->Right->content, "1") == 0)
				{
					STANDARD_SIMPLIFICATION_RIGHT
				}


			}
			default:
				break;
		}
	}
}
