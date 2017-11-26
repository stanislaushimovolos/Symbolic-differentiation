#include <iostream>
#include "../Tree_t/Tree.h"
#include "../Diff/Diff.h"

int main ()
{

	Tree begTree = {}, diffTree = {};
	char *buffer = 0;

	treeConstruct (&begTree);
	treeConstruct (&diffTree);
	readBase (&buffer, inputFilename);

	createBase (buffer, begTree.root);
	free (buffer);

	diffMain (&begTree, &diffTree, MainVariable);

	printTreeFile (&diffTree, outFilename);

	printTree (&diffTree, outFilenameForDotPrint);
	return 0;
}