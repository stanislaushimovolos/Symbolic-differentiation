#include <iostream>
#include "../Diff/Diff.h"


int main ()
{
	Tree begTree = {}, diffTree = {};
	char *curVar = NULL;
	char* buffer = getBuff (inputFilename, &curVar);

	treeConstruct (&begTree);
	treeConstruct (&diffTree);

	GetTree (&begTree, buffer, curVar);

	printTree (&begTree, outFilenameForDotPrint);
	diffMain (&begTree, &diffTree, curVar);

	destructTree (&diffTree);
	destructTree (&begTree);
	free (buffer);
	return 0;
}