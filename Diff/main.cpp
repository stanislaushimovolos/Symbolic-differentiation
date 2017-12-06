#include <iostream>
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

	free (buffer);
	destructTree (&begTree);
	destructTree (&diffTree);


	return 0;
}