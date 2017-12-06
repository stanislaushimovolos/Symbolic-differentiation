#include <assert.h>
#include <iostream>
#include <cstring>
#include <windows.h>
#include <math.h>
#include <ctime>

#include "Diff.h"


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


int TreeChecker = 0;

Node *diffMain (const Tree *const BegTree, Tree *FinalTree, const char *const currValue)
{
	FILE *outTexFile = fopen (outFilename, "w");
	assert (BegTree);
	assert (FinalTree);
	assert (currValue);

	if (!BegTree->root->content)
		return 0;

	fprintf (outTexFile, "\\documentclass{article}\n"
			"\n"
			"\\usepackage[utf8x]{inputenc}\n"
			"\n"
			"\\usepackage[russian]{babel}\n"
			"\n"
			"    \\usepackage{geometry}\n"
			"\n"
			"\\begin{document} \\begin{center} The original expression \n");

	fprintf (outTexFile, " \\[ ");

	TexExp (BegTree->root, outTexFile);

	fprintf (outTexFile, " \\] ");

	Node *helpNode = {};
	nodeConstruct (&helpNode);

	helpNode = NodeCopy (BegTree->root, FinalTree);
	helpNode->myTree = FinalTree;

	FinalTree->root = diffRec (helpNode, currValue, FinalTree, outTexFile);

	while (TreeChecker == 0)
	{
		treeVisitorInf (FinalTree->root, simpleTree);

		if (TreeChecker == 0)
		{
			TreeChecker = 1;

		}
		else

			TreeChecker = 0;
	}

	fprintf (outTexFile, "\n\\begin{center}\nThe final expression\n");

	fprintf (outTexFile, " \\[ ");

	TexExp (FinalTree->root, outTexFile);

	fprintf (outTexFile, " \\]");

	fprintf (outTexFile, "\n\n\\end{document}\n"
			"\\end");

	fclose (outTexFile);

	system ("pdflatex.exe -synctex=1 -interaction=nonstopmode ../Tex/tex.txt");
	system ("start tex.pdf");

	FinalTree->nodeAmount = 0;

	treeVisitorInf (FinalTree->root, nodeCount);

	outTexFile = NULL;
}


#define  DEF_CMD(operator_, number, code)                   \
    case number :                                           \
    {                                                       \
       code                                                 \
    }


Node *diffRec (const Node *const node, const char *const currValue, Tree *FinalTree, FILE *outFileName)
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


void TexExp (const Node *const node, FILE *outBaseFile)
{
	int priorityHelper = 0;
	printResultRec (node, priorityHelper, outBaseFile);
}

int strCounter = 0;

void TexStr (FILE *outBaseFile)
{
	strCounter++;
	strCounter = strCounter % JOKE_AMOUNT;

	fprintf (outBaseFile, "$$");
	fprintf (outBaseFile, "%s\n", Comments[strCounter]);
	fprintf (outBaseFile, "$$");
}


int printResultRec (const Node *const Node, int prior, FILE *outBaseFile1)
{
	switch (Node->type)
	{
		case Add :
		{
			if (prior == 0)
			{
				printResultRec (Node->Left, 0, outBaseFile1);
				fprintf (outBaseFile1, " + ");
				printResultRec (Node->Right, 0, outBaseFile1);
			}

			else
			{
				fprintf (outBaseFile1, "(");
				printResultRec (Node->Left, 0, outBaseFile1);
				fprintf (outBaseFile1, " + ");
				printResultRec (Node->Right, 0, outBaseFile1);
				fprintf (outBaseFile1, ")");
			}

			break;
		}

		case Ln :
		{
			fprintf (outBaseFile1, "\\ln(");
			printResultRec (Node->Left, 0, outBaseFile1);
			fprintf (outBaseFile1, ")");

			break;
		}

		case Cos :
		{
			fprintf (outBaseFile1, "\\cos(");
			printResultRec (Node->Left, 0, outBaseFile1);
			fprintf (outBaseFile1, ")");

			break;
		}

		case Sin :
		{
			fprintf (outBaseFile1, "\\sin(");
			printResultRec (Node->Left, 0, outBaseFile1);
			fprintf (outBaseFile1, ")");

			break;
		}

		case Sub :
		{
			if (prior == 0)
			{
				printResultRec (Node->Left, 0, outBaseFile1);
				fprintf (outBaseFile1, " - ");
				printResultRec (Node->Right, 0, outBaseFile1);
			}

			else
			{
				fprintf (outBaseFile1, "(");
				printResultRec (Node->Left, 0, outBaseFile1);
				fprintf (outBaseFile1, " - ");
				printResultRec (Node->Right, 0, outBaseFile1);
				fprintf (outBaseFile1, ")");
			}

			break;
		}


		case number:
		{
			if (*(Node->content) == '-')
			{
				fprintf (outBaseFile1, "(%s)", Node->content);
			}

			else
				fprintf (outBaseFile1, "%s", Node->content);

			break;
		}

		case charConst:
		{
			fprintf (outBaseFile1, "%s", Node->content);
			break;
		}

		case curVariable:
		{
			fprintf (outBaseFile1, "%s", Node->content);
			break;
		}


		case Mul :
		{

			printResultRec (Node->Left, 1, outBaseFile1);
			fprintf (outBaseFile1, "\\cdot ");
			printResultRec (Node->Right, 1, outBaseFile1);


			break;
		}

		case Div :
		{
			fprintf (outBaseFile1, "\\frac{");
			printResultRec (Node->Left, 0, outBaseFile1);
			fprintf (outBaseFile1, "}{");
			printResultRec (Node->Right, 0, outBaseFile1);
			fprintf (outBaseFile1, "}");

			break;
		}

		case Expo :
		{
			printResultRec (Node->Left, 0, outBaseFile1);
			fprintf (outBaseFile1, "^{");
			printResultRec (Node->Right, 0, outBaseFile1);
			fprintf (outBaseFile1, "}");

			break;
		}

		default:
			break;
	}
}


Node *complicatedDiff (Node *mainNode, Tree *FinalTree, const char *const currValue,
                       Node *(diff) (Node *node, Tree *FinalTree, const char *const currValue, FILE *outFileName),
                       FILE *outFileName)
{
	Node *mul = createNode (Mul, '*', FinalTree);
	connectLeft (mul, diff (mainNode, FinalTree, currValue, outFileName));
	connectRight (mul, diffRec (mainNode, currValue, FinalTree, outFileName));

	return mul;
}


#define STANDARD_SIMPLIFICATION_LEFT                    \
do {      if (node->Parent)                                   \
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
} while (0)


#define STANDARD_SIMPLIFICATION_RIGHT                   \
do {                                                    \
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


#define  CONST_FOLD(operator_) \
do { \
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
} while(0)


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
			TreeChecker++;
			return 0;
		}

		if (strcmp (node->Right->content, "1") == 0)
		{
			STANDARD_SIMPLIFICATION_RIGHT;
			TreeChecker++;
			return 0;

		}

		if (strcmp (node->Left->content, "1") == 0)
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

	if (strcmp (node->content, "+") == 0)
	{


		if (strcmp (node->Left->content, "0") == 0)
		{
			STANDARD_SIMPLIFICATION_LEFT;
			TreeChecker++;
			return 0;
		}

		if (strcmp (node->Right->content, "0") == 0)
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

	if (strcmp (node->content, "-") == 0)
	{


		if (strcmp (node->Right->content, "0") == 0)
		{
			STANDARD_SIMPLIFICATION_RIGHT;
			TreeChecker++;
			return 0;
		}

		if (strcmp (node->Left->content, "0") == 0)
		{
			char str1[] = "-1";
			char str2[] = "*";
			nodeSetName (node->Left, str1);
			node->Left->type = number;
			nodeSetName (node, str2);
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
			TreeChecker++;
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
			TreeChecker++;
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
			TreeChecker++;
			return 0;
		}

		if (strcmp (node->Right->content, "1") == 0)
		{
			STANDARD_SIMPLIFICATION_RIGHT;
			TreeChecker++;
		}
		return 0;
	}

}


Node *lnDiff (Node *node, Tree *FinalTree, const char *const currValue, FILE *outFileName)
{
	Node *mainNode = createNode (Div, '/', FinalTree);
	Node *leftNode = createNode (number, 1, FinalTree);
	connectLeft (mainNode, leftNode);
	connectRight (mainNode, node);
	return mainNode;
}


Node *sinDiff (Node *node, Tree *FinalTree, const char *const currValue, FILE *outFileName)
{
	char cos[] = "cos";
	Node *mainNode = createNode (Cos, cos, FinalTree);
	connectLeft (mainNode, node);
	return mainNode;
}


Node *cosDiff (Node *node, Tree *FinalTree, const char *const currValue, FILE *outFileName)
{
	char sin[] = "sin";

	Node *mainNode = createNode (Mul, '*', FinalTree);
	Node *rightNode = createNode (number, -1, FinalTree);

	Node *leftNode = createNode (Sin, sin, FinalTree);
	connectLeft (leftNode, node);

	connectRight (mainNode, rightNode);
	connectLeft (mainNode, leftNode);

	return mainNode;
}


Node *addDiff (const Node *const node, Tree *FinalTree, const char *const currValue, FILE *outFileName)
{
	Node *LeftCopy = NodeCopy (node->Left, FinalTree);
	LeftCopy->myTree = FinalTree;

	Node *RightCopy = NodeCopy (node->Right, FinalTree);
	RightCopy->myTree = FinalTree;

	Node *dL = diffRec (LeftCopy, currValue, FinalTree, outFileName);
	Node *dR = diffRec (RightCopy, currValue, FinalTree, outFileName);

	Node *mainNode = createNode (Add, *node->content, FinalTree);

	connectLeft (mainNode, dL);
	connectRight (mainNode, dR);

	return mainNode;
}


Node *subDiff (const Node *const node, Tree *FinalTree, const char *const currValue, FILE *outFileName)
{

	Node *LeftCopy = NodeCopy (node->Left, FinalTree);
	LeftCopy->myTree = FinalTree;

	Node *RightCopy = NodeCopy (node->Right, FinalTree);
	RightCopy->myTree = FinalTree;

	Node *dL = diffRec (LeftCopy, currValue, FinalTree, outFileName);
	Node *dR = diffRec (RightCopy, currValue, FinalTree, outFileName);

	Node *mainNode = createNode (Sub, *node->content, FinalTree);

	connectLeft (mainNode, dL);
	connectRight (mainNode, dR);

	return mainNode;
}


Node *mulDiff (const Node *const node, Tree *FinalTree, const char *const currValue, FILE *outFileName)
{

	Node *LeftCopy = NodeCopy (node->Left, FinalTree);
	LeftCopy->myTree = FinalTree;

	Node *RightCopy = NodeCopy (node->Right, FinalTree);
	RightCopy->myTree = FinalTree;

	Node *dL = diffRec (LeftCopy, currValue, FinalTree, outFileName);
	Node *dR = diffRec (RightCopy, currValue, FinalTree, outFileName);

	Node *mainNode = createNode (Add, '+', FinalTree);

	Node *FirstProNode = createNode (Mul, *node->content, FinalTree);
	Node *SecondProNode = createNode (Mul, *node->content, FinalTree);

	connectRight (FirstProNode, dL);
	connectLeft (FirstProNode, RightCopy);

	connectRight (SecondProNode, dR);
	connectLeft (SecondProNode, LeftCopy);

	connectLeft (mainNode, FirstProNode);
	connectRight (mainNode, SecondProNode);

	return mainNode;

}


Node *divDiff (const Node *const node, Tree *FinalTree, const char *const currValue, FILE *outFileName)
{
	Node *LeftCopy = NodeCopy (node->Left, FinalTree);
	LeftCopy->myTree = FinalTree;

	Node *RightCopy = NodeCopy (node->Right, FinalTree);
	RightCopy->myTree = FinalTree;

	Node *dL = diffRec (LeftCopy, currValue, FinalTree, outFileName);
	Node *dR = diffRec (RightCopy, currValue, FinalTree, outFileName);

	Node *minusNode = createNode (Sub, '-', FinalTree);

	Node *FirstProNode = createNode (Mul, '*', FinalTree);
	Node *SecondProNode = createNode (Mul, '*', FinalTree);

	connectRight (FirstProNode, dL);
	connectLeft (FirstProNode, RightCopy);

	connectRight (SecondProNode, dR);
	connectLeft (SecondProNode, LeftCopy);

	connectLeft (minusNode, FirstProNode);
	connectRight (minusNode, SecondProNode);

	Node *denPro = createNode (Mul, '*', FinalTree);

	Node *RightCopy1 = NodeCopy (node->Right, FinalTree);
	RightCopy1->myTree = FinalTree;


	Node *RightCopy2 = NodeCopy (node->Right, FinalTree);
	RightCopy2->myTree = FinalTree;

	connectRight (denPro, RightCopy1);
	connectLeft (denPro, RightCopy2);

	Node *MainNode = createNode (Div, '/', FinalTree);

	connectLeft (MainNode, minusNode);
	connectRight (MainNode, denPro);

	return MainNode;

}


Node *expoDiff (const Node *const node, Tree *FinalTree, const char *const currValue, FILE *outFileName)
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

	Node *mainMinus = createNode (Sub, '-', FinalTree);
	Node *MainPlus = createNode (Add, '+', FinalTree);
	Node *ProNode1 = createNode (Mul, '*', FinalTree);
	Node *ProNode2 = createNode (Mul, '*', FinalTree);
	Node *ProNode3 = createNode (Mul, '*', FinalTree);
	Node *ProNode4 = createNode (Mul, '*', FinalTree);

	Node *Cap1 = createNode (Expo, '^', FinalTree);
	Node *Cap2 = createNode (Expo, '^', FinalTree);

	Node *LN = {};
	nodeConstruct (&LN);
	nodeSetName (LN, ln);
	LN->type = Ln;

	connectLeft (ProNode1, LN);
	connectLeft (LN, base1);
	connectRight (ProNode1, diffRec (index1, currValue, FinalTree, outFileName));
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
	connectRight (ProNode4, diffRec (base3, currValue, FinalTree, outFileName));
	connectLeft (Cap2, base4);
	connectRight (Cap2, mainMinus);

	connectRight (mainMinus, createNode (number, 1, FinalTree));
	connectLeft (mainMinus, index4);
	return MainPlus;
}

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
