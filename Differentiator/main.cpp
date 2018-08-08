#include <iostream>
#include "Differentiator .h"
#include "../RecursiveDescent/Descent.h"

int main()
{
    Tree diffTree = {};
    treeConstruct(&diffTree);

    parser pars = {};

    auto status = constructParser(&pars, inputFilename);
    if (status)
        return status;

    GetTree(&pars);

    dumpTreePicture(&pars.tree, outFilenameForDotDump);
    diffMain(&pars.tree, &diffTree, pars.curVar);

    destructTree(&diffTree);
    destructParser(&pars);

    return 0;
}