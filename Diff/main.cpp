#include <iostream>
#include "../Diff/Diff.h"


int main ()
{
	Tree begTree = {}, diffTree = {};
	char buffer[] = "1/ln(x^2)^2 - 16 /sin(0)";

	treeConstruct (&begTree);
	treeConstruct (&diffTree);

	GetTree (&begTree, buffer);

	diffMain (&begTree, &diffTree, MainVariable);

	dumpTreePicture (&diffTree, outFilenameForDotDump);

	destructTree (&diffTree);
	destructTree (&begTree);

	return 0;
}