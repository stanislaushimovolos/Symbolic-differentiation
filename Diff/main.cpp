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

	printResultFile (&diffTree, outFilename);

	dumpTreePicture (&diffTree, outFilenameForDotDump);
	free (buffer);


	return 0;
}