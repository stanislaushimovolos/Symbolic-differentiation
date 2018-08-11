#include <iostream>
#include "Differentiator.h"
#include "../RecursiveDescent/Descent.h"

int main()
{
    calculator calc = {};
    parser pars = {};

    auto status = constructParser(&pars, inputFilename) &
                  constructCalculator(&calc, outFileTex);
    if (status)
        return status;
    GetTree(&pars);
    findDerivative(&pars, &calc);

    printTree(&pars.tree, outFilenameForDotPrint);

    destructParser(&pars);
    destructCalculator(&calc);
    return 0;
}