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
    status = getTree(&pars);

    if (status)
        return status;

    findDerivative(&pars, &calc);

    destructParser(&pars);
    destructCalculator(&calc);

    return 0;
}