//
// Created by Tom on 22.11.2017.
//
#include <assert.h>
#include <iostream>
#include <cstring>
#include "../Tree_t/Tree.h"
#include <stdlib.h>

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

	char *helpBuffer = (char *) calloc (sizeOfBuffer1 + 1, sizeof (char));
	fread (helpBuffer, 1, sizeOfBuffer1, file);
	fclose (file);

	if (sizeOfBuffer != NULL)
		*sizeOfBuffer = sizeOfBuffer1;

	return helpBuffer;
}

#define DEF_CMD_OPERATOR(operator_, number, code)           \
    case operator_ :                                        \
     {                                                      \
        node->type = number;                                \
        operatorFlag = 1;                                    \
        break;                                              \
    }


char contentAnalyze (Node *node, const char *const currValue)
{

	char *NumHelper = (node->content);
	strtod (node->content, &NumHelper);
	bool operatorFlag = 0;

	if (NumHelper != node->content)
		node->type = number;

	else if (strcmp (currValue, node->content) == 0)
		node->type = curVariable;

	else if (strlen (node->content) == 1)
	{
		switch (*node->content)
		{

#include "MathFunc.h"

			default:
				break;
		}

		if (!operatorFlag)
			node->type = charConst;
	}
	else
		node->type = charConst;
}

#undef DEF_CMD_OPERATOR

Node *diffMain (const Tree *const BegTree, Tree *FinalTree, const char *const currValue)
{
	assert (BegTree);
	assert (FinalTree);
	assert (currValue);

	Node *helpNode = {};
	nodeConstruct (&helpNode);

	helpNode = NodeCopy (BegTree->root, FinalTree);
	helpNode->myTree = FinalTree;

	FinalTree->root = diffRec (helpNode, currValue, FinalTree);

}

Node *diffRec (const Node *const node, const char *const currValue, Tree *FinalTree)
{
	switch (node->type)
	{
		case number:
			return createNode (number, 0, FinalTree);

		case curVariable:
			return createNode (curVariable, 1, FinalTree);

		case operator_:
		{
			switch (*node->content)
			{

//#include "MathFunc.h"

				case '+':
				{
					Node *LeftCopy = NodeCopy (node->Left, FinalTree);
					LeftCopy->myTree = FinalTree;

					Node *RightCopy = NodeCopy (node->Right, FinalTree);
					RightCopy->myTree = FinalTree;

					Node *dL = diffRec (LeftCopy, currValue, FinalTree);
					Node *dR = diffRec (RightCopy, currValue, FinalTree);

					Node *mainNode = createNode (operator_, *node->content, FinalTree);

					connectLeft (mainNode, dL);
					connectRight (mainNode, dR);

					return mainNode;

				}

				case '-':
				{
					Node *LeftCopy = NodeCopy (node->Left, FinalTree);
					LeftCopy->myTree = FinalTree;

					Node *RightCopy = NodeCopy (node->Right, FinalTree);
					RightCopy->myTree = FinalTree;

					Node *dL = diffRec (LeftCopy, currValue, FinalTree);
					Node *dR = diffRec (RightCopy, currValue, FinalTree);

					Node *mainNode = createNode (operator_, *node->content, FinalTree);

					connectLeft (mainNode, dL);
					connectRight (mainNode, dR);

					return mainNode;
				}

				case '*':
				{

					Node *LeftCopy = NodeCopy (node->Left, FinalTree);
					LeftCopy->myTree = FinalTree;

					Node *RightCopy = NodeCopy (node->Right, FinalTree);
					RightCopy->myTree = FinalTree;

					Node *dL = diffRec (LeftCopy, currValue, FinalTree);
					Node *dR = diffRec (RightCopy, currValue, FinalTree);

					Node *mainNode = createNode (operator_, '+', FinalTree);

					Node *FirstProNode = createNode (operator_, *node->content, FinalTree);
					Node *SecondProNode = createNode (operator_, *node->content, FinalTree);

					connectRight (FirstProNode, dL);
					connectLeft (FirstProNode, RightCopy);

					connectRight (SecondProNode, dR);
					connectLeft (SecondProNode, LeftCopy);

					connectLeft (mainNode, FirstProNode);
					connectRight (mainNode, SecondProNode);

					return mainNode;
				}

				case '/':
				{

					Node *LeftCopy = NodeCopy (node->Left, FinalTree);
					LeftCopy->myTree = FinalTree;

					Node *RightCopy = NodeCopy (node->Right, FinalTree);
					RightCopy->myTree = FinalTree;

					Node *dL = diffRec (LeftCopy, currValue, FinalTree);
					Node *dR = diffRec (RightCopy, currValue, FinalTree);

					Node *minusNode = createNode (operator_, '-', FinalTree);

					Node *FirstProNode = createNode (operator_, '*', FinalTree);
					Node *SecondProNode = createNode (operator_, '*', FinalTree);

					connectRight (FirstProNode, dL);
					connectLeft (FirstProNode, RightCopy);

					connectRight (SecondProNode, dR);
					connectLeft (SecondProNode, LeftCopy);

					connectLeft (minusNode, FirstProNode);
					connectRight (minusNode, SecondProNode);

					Node *denPro = createNode (operator_, '*', FinalTree);


					Node *RightCopy1 = NodeCopy (node->Right, FinalTree);
					RightCopy1->myTree = FinalTree;


					Node *RightCopy2 = NodeCopy (node->Right, FinalTree);
					RightCopy2->myTree = FinalTree;

					connectRight (denPro, RightCopy1);
					connectLeft (denPro, RightCopy2);

					Node *MainNode = createNode (operator_, '/', FinalTree);

					connectLeft (MainNode, minusNode);
					connectRight (MainNode, denPro);

					return MainNode;
				}

				default:
					break;
			}
		}

		case charConst:
			return createNode (charConst, 0, FinalTree);
		default:
			break;
	}
}

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

#define DEF_CMD_OPERATOR(operator_, number, code)          \
    case operator_ :                                       \
    {                                                      \
       code                                                \
    }


Node *createNode (const char type, char operator__, Tree *FinalTree)
{
	Node *mainNode = {};
	nodeConstruct (&mainNode);

	mainNode->myTree = FinalTree;
	mainNode->content = (char *) calloc (2, sizeof (char));

	*mainNode->content = operator__;
	return mainNode;
}
