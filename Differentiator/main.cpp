#include <iostream>
#include "Differentiator .h"
#include "../RecursiveDescent/Descent.h"

int main()
{
    calculator calc = {};
    parser pars = {};

    auto status = constructParser(&pars, inputFilename) &
                  constructCalculator(&calc, outFilename);
    if (status)
        return status;
    GetTree(&pars);
    diffMain(&pars, &calc);

    destructParser(&pars);
    destructCalculator(&calc);
    return 0;
}