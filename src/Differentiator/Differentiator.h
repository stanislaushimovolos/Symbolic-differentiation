/*!
 * @file Differentiator.h
 * @brief Contains headers of functions for expression's simplification and for taking derivatives.
 * @author Stanislau Shimovolos
 * @date 2018-8-15
 */

#include <assert.h>
#include "../Tree_t/Tree.h"
#include "List.h"
#include "../Commands/Commands.h"


static const char outFileTex[] = "Tex/Diff_Result";
static const char outFilenameForDotDump[] = "picture/dotDump.txt";
static const char outFilenameForDotPrint[] = "picture/dotPrint.txt";

#define NUMBER_OF_COMMENTS 7
static const char *const comments[] = {"After mental calculation: ",
                                       "It is well known that: ",
                                       "Using  WolframAlpha we have: ",
                                       "Every child knows that: ",
                                       "For a detailed explanation you should purchase the paid version: ",
                                       "If you are familiar with calculus: ",
                                       "It's not a secret that: "};


/*!
   @brief The structure which keeps a binary tree with derivative.
   It contains a binary tree and an output Tex file.
 */
typedef struct calculator
{
    Tree tree;                      //!<A binary tree with the derivative.
    FILE *texFile;                  //!<File with data for LaTex.
} calculator;

/*!
    @brief Does simplification like this : 4*x + 3 + 5*x = 9*x +3.
    @param[in] node pointer to the subtree.
    @return it returns 1, if operation was made, else returns 0.
*/
int simplifyTreeAddition(Node *node);

/*!
    @brief Does simplification like this : x*1 = x, etc.
    @param[in] node pointer to the subtree.
    @return it returns 1, if operation was made, else returns 0.
*/
int simplifyTreeNumerical(Node *node);

/*!
    @brief Restores tree after simplifications.
    @param[in] node pointer to the subtree.
    @return it returns 1, if operation was made, else returns 0.
*/
int prepareTreeForPrinting(Node *node);

/*!
    @brief Simplify tree (do all simplifications).
    @param[in] tree current subtree.
    @return it returns 1, if operation was made, else returns 0.
*/
int simplifyExpression(Tree *tree);

/*!
    @brief Collects all numbers (5+ x + x^2 + 4 => x + x^2 + 9).
    @param[in] node pointer to the subtree.
    @return it returns 1, if operation was made, else returns 0.
*/
int foldConstantsGlobal(Node *node);

/*!
    @brief Returns sum of numbers in the subtree (3 +x + 14 => result = 17).
    @param[in] node pointer to the node.
    @return returns sum of numbers.
*/
double findNumericalSums(Node *node);

/*!
    @brief Does simplification like this : 5*3*4*x = 60*x.
    @param[in] node pointer to the subtree.
    @return it returns 1, if operation was made, else returns 0.
*/
int simplifyTreeMultiplication(Node *node);

/*!
    @brief Replaces subtraction by addition and do other things to improve the result of simplifications.
    @param[in] node pointer to the subtree.
    @return returns sum of numbers.
*/
int prepareTreeForSimplification(Node *node);

/*!
    @brief Creates list of numerical terms in the current subtree.
    @param[in] node pointer to the subtree.
    @param[in] lst list with multiplier.
    @return it returns 1, if operation was made, else returns 0.
*/
int findSimilarTerms(const Node *node, List *lst);

/*!
    @brief Creates list of numerical multipliers in the current subtree.
    @param[in] node pointer to the subtree.
    @param[in] lst list with multiplier.
    @return it returns 1, if operation was made, else returns 0.
*/
int findNumericalMultipliers(const Node *node, List *lst);

/*!
    @brief if node type is equal to add, sub, ... it will replace the node by the result.
    @param[in] node pointer to the node.
    @return it returns 1, if operation was made, else returns 0.
*/
int foldConstants(Node *node);

/*!
    @brief Main function which finds derivative, simplifies expression and prints it.
    @param[in] calc pointer to @ref calculator structure.
    @param[in] tree original expression (in binary tree form).
*/
int findDerivative(Tree *tree, calculator *calc);

/*!
    @brief Prints subtree to the Tex file.
    @param[in] node pointer to the subtree.
    @param[in] outFileName Tex file.
*/
int texResult(const Node *node, FILE *outFileName);

/*!
    @brief Prints derivative step to the Tex file ((x^2)' = 2*x, for example).
    @param[in] original pointer to the original subtree.
    @param[in] outFileName Tex file.
    @param[in] result result of derivative step.
*/
int texDerivativeStep(const Node *original, const Node *result, FILE *outFileName);

/*!
    @brief Finds the derivative of the natural logarithm.
    @param[in] node pointer to differentiable expression.
    @param[in] calc pointer to @ref calculator structure.
    @return returns result of the operation.
*/
Node *logDerivative(const Node *node, calculator *calc);

/*!
    @brief Finds the derivative of the sinus.
    @param[in] node pointer to differentiable expression.
    @param[in] calc pointer to @ref calculator structure.
    @return returns result of the operation.
*/
Node *sinDerivative(const Node *node, calculator *calc);

/*!
    @brief Finds the derivative of the cosine.
    @param[in] node pointer to differentiable expression.
    @param[in] calc pointer to @ref calculator structure.
    @return returns result of the operation.
*/
Node *cosDerivative(const Node *node, calculator *calc);

/*!
    @brief Finds the derivative of the addition.
    @param[in] node pointer to differentiable expression.
    @param[in] calc pointer to @ref calculator structure.
    @return returns result of the operation.
*/
Node *addDerivative(const Node *node, calculator *calc);

/*!
    @brief Finds the derivative of the subtraction.
    @param[in] node pointer to differentiable expression.
    @param[in] calc pointer to @ref calculator structure.
    @return returns result of the operation.
*/
Node *subDerivative(const Node *node, calculator *calc);

/*!
    @brief Finds the derivative of the multiplication.
    @param[in] node pointer to differentiable expression.
    @param[in] calc pointer to @ref calculator structure.
    @return returns result of the operation.
*/
Node *mulDerivative(const Node *node, calculator *calc);

/*!
    @brief Finds the derivative of the division.
    @param[in] node pointer to differentiable expression.
    @param[in] calc pointer to @ref calculator structure.
    @return returns result of the operation.
*/
Node *divDerivative(const Node *node, calculator *calc);

/*!
    @brief Finds the derivative of the exponential function.
    @param[in] node pointer to differentiable expression.
    @param[in] calc pointer to @ref calculator structure.
    @return returns result of the operation.
*/
Node *expDerivative(const Node *node, calculator *calc);

/*!
    @brief Merges two subtrees into one with multiplier @ref value.
    @param[in] main main subtree.
    @param[in] value multiplier.
    @param[in] second second subtree which will be destroyed.
    @return returns merged subtree.
*/
Node *mergeTrees(Node *main, Node *second, double value);

/*!
    @brief Finds the derivative of the current subtree and prints information to LaTex file (if it necessary).
    @param[in] node pointer to the current subtree.
    @param[in] calc pointer to @ref calculator structure.
    @return returns result of the operation.
*/
Node *makeDerivativeStep(const Node *node, calculator *calc);

/*!
    @brief Returns the derivative of complex function (func(x)' = func'(x)*x').
    @param[in] node pointer to the current subtree.
    @param[in] calc pointer to @ref calculator structure.
    @param[in] func pointer to the function.
    @return returns result of the operation.
*/
Node *findComplexDerivative(const Node *node, calculator *calc,
                            Node *(func)(const Node *node, calculator *calc));

/*!
    @brief Destructs @ref calculator object.
    @param[in] calc pointer to @ref calculator structure.
*/
int destructCalculator(calculator *calc);

/*!
    @brief Constructs @ref calculator object.
    @param[in] calc pointer to @ref calculator structure.
    @param[in] inputFileName name of the file with the expression.
*/
int constructCalculator(calculator *calc, const char *inputFileName);