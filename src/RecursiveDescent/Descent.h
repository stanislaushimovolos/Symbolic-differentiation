/*!
 * @file Descent.h
 * @brief Contains funcion's headers for the recursive descent.
 * @author Stanislau Shimovolos
 * @date 2018-8-15
 */

#ifndef DIFF3_0_DESCENT_H
#define DIFF3_0_DESCENT_H

#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../Tree_t/Tree.h"
#include "../Commands/Commands.h"

//Default input for expressions.
static const char inputFilename[] = "Differentiator/expression.txt";

/**
* @brief The list of possible errors
*/
enum errors
{
    FILE_OPENING_ERROR = 1,        //!<Means that some problem was detected in process of opening file.
    FORMAT_ERROR,                  //!<Means that construction d/d(variable) wasn't detected.
    UNKNOWN_FUNC,                  //!<Means that program met unknown function with branches (superTan(5), for example)
    ARITHMETIC_OP,                 //!<Means that some arithmetic sign was missed. (x + x^2 x^3, for example).
    ARGUMENTS_ERROR,               //!<Means that function's argument or some of them are equal NULL.
    USER_ERROR,                    //!<Means that user has made a mistake(any mistake).
    OUT_OF_ERR_RANGE_ERR,
};

static const char *errorList[] = {"",
                                  "",
                                  "Wrong format\nExpected d/d <variable> <expression>\n",
                                  "Unknown function:\n",
                                  "Expected arithmetic operation\n"
};


/*!
   @brief The structure which input data, and binary tree constructed from that.
   It contains data as a sequence of symbols, size of this data,
   a binary tree with this data and current variable (d/dx => x).
 */
typedef struct parser
{
    Tree tree;              //!<A binary tree with operators, variables and functions.
    size_t sizeOfCode;      //!<Size of the expression.
    int curCodePos;         //!<Number of character which is currently used to build a binary tree.
    char *curVar;           //!<Current variable.
    char *code;             //!<Pointer to a symbol form of the expression.
} parser;


/*!
    @brief Constructs  a subtree from expressions like this one: exp1 +-exp2 +-exp3...
    @param[in] pars pointer to @ref parser structure.
    @return returns subtree or NULL if it couldn't read matching data.
*/
Node *getAddSub(parser *tree);

/*!
    @brief Constructs  a subtree from expressions like this one: exp1 *exp2 *exp3...
    @param[in] pars pointer to @ref parser structure.
    @return returns subtree or NULL if it couldn't read matching data.
*/
Node *getMulDiv(parser *tree);

/*!
    @brief Constructs a subtrees from expressions like this one: exp1 ^exp2 ^exp3...
    @param[in] pars pointer to @ref parser structure.
    @return returns subtree or NULL if it couldn't read matching data.
*/
Node *getExpo(parser *tree);

/*!
    @brief Constructs a subtree from expressions like this one: (exp1 +exp2 +exp3...)
    @param[in] pars pointer to @ref parser structure.
    @return returns subtree or NULL if it couldn't read matching data.
*/
Node *getBranches(parser *tree);

/*!
    @brief Tries to read a function with branches (log (x^2 + 5*x), for example).
    @param[in] pars pointer to @ref parser structure.
    @return returns subtree or NULL if it couldn't read matching data.
*/
Node *getFunc(parser *tree);

/*!
    @brief Tries to read a Number.
    @param[in] pars pointer to @ref parser structure.
    @return returns subtree or NULL if it couldn't read matching data.
*/
Node *getNumber(parser *tree);

/*!
    @brief Constructs a binary tree.
    @param[in] pars pointer to @ref parser structure.
    @return returns an error code.
*/
int getTree(parser *pars);


/*!
    @brief Returns type of node.
    @param[in] pars pointer to @ref parser structure..
    @param[in] node node, which type this function tries to detect.
    @return returns type of the node(if type is not defined function interprets it as a constant).
*/
int analyzeData(Node *node, parser *pars);


/*!
    @brief Reads data from @ref inputFileName for parser (detects current variable d/dx, for instance).
    @param[in] pars pointer to @ref parser structure.
    @param[in] inputFileName name of the file with expression.
    @return returns an error code.
*/
int getData(parser *pars, const char *inputFileName);


/*!
    @brief Reads data from @ref inputFileName for parser.
    @param[in] pars pointer to @ref parser structure.
    @param[in] inputFileName name of the file with expression.
    @return returns an error code.
*/
int getBuf(parser *data, const char *inputFileName);


/*!
    @brief Destructs @ref parser object.
    @param[in] pars pointer to @ref parser object.
*/
int destructParser(parser *pars);


/*!
    @brief Reads data from @ref inputFileName and constructs parser.
    @param[in] pars pointer to @ref parser structure.
    @param[in] inputFileName name of the file with expression.
    @return returns an error code.
*/
int constructParser(parser *pars, const char *inputFileName);


/*!
    @brief Returns an error code and prints information about the error(standard output + standard error output).
    @param[in] err_num  error code.
    @param[in] usr_msg information about the error which is available for user.
    @param[in] _func name of the function where an error was detected.
    @param[in] _line number of a code line where an error was detected.
    @param[in] _file a name of  file where an error was detected.
    @return returns an error code.
*/
int throw_error(unsigned int err_num, const char *usr_msg, const char *err_msg, const char *_func, int _line,
                const char *_file);

#endif //DIFF3_0_DESCENT_H
