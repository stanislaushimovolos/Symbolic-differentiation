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

	diffMain (&begTree, &diffTree, MainVariable);

	printTree (&diffTree, outFilenameForDotPrint);
	free (buffer);

	dumpTreePicture (&diffTree, outFilenameForDotDump);
	return 0;
}