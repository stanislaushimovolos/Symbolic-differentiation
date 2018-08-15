/*!
 * @file main.c
 * @brief Main program module.
 * @author Stanislau Shimovolos
 * @date 2018-8-15
 */

#include "Differentiator.h"
#include "../RecursiveDescent/Descent.h"

int main(int argc, char *argv[])
{
    calculator calc = {};
    parser pars = {};

    //Read data and check output file
    int status = constructParser(&pars, inputFilename);
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