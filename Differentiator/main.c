
#include "Differentiator.h"
#include "../RecursiveDescent/Descent.h"

int main()
{
    calculator calc = {};
    parser pars = {};

    int status = constructParser(&pars, inputFilename) &
                 constructCalculator(&calc, outFileTex);
    if (status)
        return status;
    GetTree(&pars);
    findDerivative(&pars, &calc);

    printTree(&calc.tree, outFilenameForDotDump, dumpRecNode);
    system("dot -Tpng picture/dotDump.txt -o picture/Dump.png");

    destructParser(&pars);
    destructCalculator(&calc);

    return 0;
}