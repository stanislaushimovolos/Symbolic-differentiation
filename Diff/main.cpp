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

	printTreeFile (&begTree, outFilename);
	printTree (&begTree, outFilenameForDotPrint);

	dumpTreePicture (&begTree, outFilenameForDotDump);
	return 0;
}