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

	diffMain (&begTree, &diffTree, curVar);


	destructTree (&diffTree);
	destructTree (&begTree);

	return 0;
}