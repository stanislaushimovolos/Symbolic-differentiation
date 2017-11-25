//
// Created by Tom on 22.11.2017.
//
#include <assert.h>
#include <iostream>
#include <cstring>
#include "../Tree_t/Tree.h"

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

Node *diffMain (const Node *const node, const char *const currValue)
{

}

Node *diffRec (const Node *const node, const char *const currValue)
{
	switch (node->type)
	{
		case number:

		case curVariable:

		default:
			break;
	}
}