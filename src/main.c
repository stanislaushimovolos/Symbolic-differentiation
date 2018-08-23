/*!
 * @file main.c
 * @brief Main program module.
 * @author Stanislau Shimovolos
 * @date 2018-8-15
 */

#include "Differentiator/Differentiator.h"
#include "RecursiveDescent/Descent.h"

int main(int argc, char *argv[])
{
    calculator calc = {};
    parser pars = {};
    int status = 0;

    //Read data and check output file
    if (argc > 1)
        status = constructParser(&pars, argv[1]);
    else
        status = constructParser(&pars, defaultInputFilename);

    if (status)
        return status;

    status = constructCalculator(&calc, outFileTex);
    if (status)
        return status;

    //Create a binary tree
    status = getTree(&pars);
    if (status)
        return status;

    findDerivative(&pars.tree, &calc);

    destructParser(&pars);
    destructCalculator(&calc);
    return 0;
}