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

	helpNode = NodeCopy (BegTree->root);
	helpNode->myTree = FinalTree;

	FinalTree->root = diffRec (helpNode, MainVariable, FinalTree);

}

Node *diffRec (const Node *const node, const char *const currValue, Tree *FinalTree)
{
	switch (node->type)
	{
		case number:
			return createNode (number, FinalTree);

		case curVariable:
			return createNode (curVariable, FinalTree);

		case operator_:
		{
			Node *LeftCopy = NodeCopy (node->Left);
			LeftCopy->myTree = FinalTree;

			Node *RightCopy = NodeCopy (node->Right);
			RightCopy->myTree = FinalTree;

			Node *dL = diffRec (LeftCopy, currValue, FinalTree);
			Node *dR = diffRec (RightCopy, currValue, FinalTree);

			return createNode (operator_, *node->content, dL, dR, FinalTree);
		}

		case charConst:
			return createNode (charConst, FinalTree);
		default:
			break;
	}
}

Node *createNode (const char type, Tree *FinalTree)
{
	switch (type)
	{
		case number:
		{
			Node *helpNode = {};
			nodeConstruct (&helpNode);

			helpNode->myTree = FinalTree;

			helpNode->content = (char *) calloc (2, sizeof (char));
			strcpy (helpNode->content, "0");
			helpNode->type = number;

			return helpNode;
		}

		case charConst:
		{
			Node *helpNode = {};
			nodeConstruct (&helpNode);

			helpNode->myTree = FinalTree;

			helpNode->content = (char *) calloc (2, sizeof (char));
			strcpy (helpNode->content, "0");
			helpNode->type = number;

			return helpNode;

		}

		case curVariable:
		{
			Node *helpNode = {};
			nodeConstruct (&helpNode);

			helpNode->myTree = FinalTree;

			helpNode->content = (char *) calloc (2, sizeof (char));
			strcpy (helpNode->content, "1");
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

Node *createNode (char type, char operator_, Node *dL, Node *dR, Tree *FinalTree)
{
	switch (operator_)
	{

//#include "MathFunc.h"

		case '+':
		{
			char operatorPtr[] = "+";
			Node *mainNode = {};
			nodeConstruct (&mainNode);

			mainNode->myTree = FinalTree;

			mainNode->content = (char *) calloc (2, sizeof (char));
			strcpy (mainNode->content, operatorPtr);

			connectLeft (mainNode, dL);
			connectRight (mainNode, dR);

			return mainNode;

		}

		case '-':
		{
			char operatorPtr[] = "-";
			Node *mainNode = {};
			nodeConstruct (&mainNode);

			mainNode->myTree = FinalTree;

			mainNode->content = (char *) calloc (2, sizeof (char));
			strcpy (mainNode->content, operatorPtr);

			connectLeft (mainNode, dL);
			connectRight (mainNode, dR);

			return mainNode;

		}
		default:
			break;
	}
}
