/*!
 * @file main.c
 * @brief Main program module.
 * @author Stanislau Shimovolos
 * @date 2018-8-15
 */

#include "Differentiator.h"
#include "../RecursiveDescent/Descent.h"

int main()
{
    calculator calc = {};
    parser pars = {};

    //Read data and check output file
    int status = constructParser(&pars, inputFilename) ^
                 constructCalculator(&calc, outFileTex);
    if (status)
        return status;
    status = getTree(&pars);

    if (status)
        return status;

    findDerivative(&pars.tree, &calc);

    destructParser(&pars);
    destructCalculator(&calc);

    return 0;
}