/*!
 * @file Descent.c
 * @brief Implementation of @ref  Descent.h functions.
 * @author Stanislau Shimovolos
 * @date 2018-8-15
 */

#include "Descent.h"

const char difVariableFlag[] = "d/d";

#define SKIP_SPASES                                                                     \
    while (isspace (pars->code[pars->curCodePos]))                                      \
        pars->curCodePos++;                                                             \


#define ARG_CHECK(var)                                                                  \
if (!(var))                                                                             \
    return throw_error(ARGUMENTS_ERROR, "", "Unexpected values of arguments (nullptr)." \
                                          , __PRETTY_FUNCTION__, __LINE__, __FILE__);   \


int constructParser(parser *pars, const char *inputFileName)
{
    ARG_CHECK(pars && inputFileName)

    pars->curCodePos = 0;
    treeConstruct(&pars->tree);

    int status = getData(pars, inputFileName);
    return status;
}


int destructParser(parser *pars)
{
    destructTree(&pars->tree);
    free(pars->curVar);
    free(pars->code);
}


int getData(parser *pars, const char *inputFileName)
{
    ARG_CHECK(pars && inputFileName);

    int status = (getBuf(pars, inputFileName));
    if (status)
        return status;

    SKIP_SPASES;
    if (pars->curCodePos + sizeof(difVariableFlag) < pars->sizeOfCode)
    {
        SKIP_SPASES;
        int j = 0;

        for (int i = 0; i < sizeof(difVariableFlag) - 1; i++)
        {
            if (pars->code[pars->curCodePos++] != difVariableFlag[j++])
            {
                printf("%s", errorList[FORMAT_ERROR]);
                return FORMAT_ERROR;
            }
        }
        SKIP_SPASES;
        size_t currentVariableSizeCounter = 0;

        while (isalpha(pars->code[pars->curCodePos++]))
            currentVariableSizeCounter++;

        if (currentVariableSizeCounter == 0)
        {
            printf("%s", errorList[FORMAT_ERROR]);
            return FORMAT_ERROR;
        }
        pars->curVar = (char *) calloc(currentVariableSizeCounter + 1, sizeof(char));
        memcpy(pars->curVar, pars->code + pars->curCodePos - currentVariableSizeCounter - 1,
               currentVariableSizeCounter);
    }
    else
    {
        printf("%s", errorList[FORMAT_ERROR]);
        return FORMAT_ERROR;
    }

    return 0;
}


int getBuf(parser *data, const char *inputFileName)
{
    assert(data);
    assert (inputFileName);
    ARG_CHECK(data && inputFileName)

    size_t l_buf_sz = 0;
    FILE *file = fopen(inputFileName, "r");

    if (!file)
    {
        printf("Couldn't open file:\n%s\n", inputFileName);
        return FILE_OPENING_ERROR;
    }
    fseek(file, 0, SEEK_END);
    l_buf_sz = (size_t) ftell(file);
    fseek(file, 0, SEEK_SET);

    data->code = (char *) calloc(l_buf_sz + 1, sizeof(char));
    assert(data->code);
    ARG_CHECK(data->code)

    data->sizeOfCode = l_buf_sz;
    fread(data->code, 1, l_buf_sz, file);
    fclose(file);

    return 0;
}


int getTree(parser *pars)
{
    ARG_CHECK(pars)
    pars->tree.root = getAddSub(pars);

    if (pars->tree.root == NULL)
        return USER_ERROR;
    return 0;
}


Node *getAddSub(parser *pars)
{
    assert(pars);
    SKIP_SPASES;

    //curLeftNode + other nodes (if there are any)
    Node *curLeftNode = getMulDiv(pars);
    if (!curLeftNode)
        return NULL;

    SKIP_SPASES;

    //plus or minus
    char op = 0;

    Node *curSignNode = NULL;
    Node *curRightNode = NULL;

    while (pars->code[pars->curCodePos] == '+' ||
           pars->code[pars->curCodePos] == '-')
    {
        SKIP_SPASES;
        op = pars->code[pars->curCodePos];
        pars->curCodePos++;

        switch (op)
        {
            case '+':
            {
                curSignNode = createSimpleNode(Add, &pars->tree);
                break;
            }

            case '-':
            {
                curSignNode = createSimpleNode(Sub, &pars->tree);
                break;
            }
            default:
                return NULL;
        }

        /*
         *                          ____________    ...
         *                         |            (next node)
         *                         | Left
         *                   ______|_______
         *                  |              |
         *           _______| curSignNode  |________
         *          |       |    + or -    |        |
         *          | Left  |______________|  Right |
         *  ________|__                           __|_________
         * |           |                         |            |
         * |curLeftNode|                         |curRightNode|
         * |___________|                         |____________|
         */


        connectLeft(curSignNode, curLeftNode);

        curRightNode = getMulDiv(pars);
        if (!curRightNode)
        {
            printf("Incorrect expression after \"%c\"\n", op);
            return NULL;
        }
        connectRight(curSignNode, curRightNode);

        //Push curSignNode below to connect new nodes.
        curLeftNode = curSignNode;
    }
    return curLeftNode;
}


Node *getMulDiv(parser *pars)
{
    assert(pars);
    SKIP_SPASES;

    //curLeftNode * other nodes (if there are any)
    Node *curLeftNode = getExpo(pars);
    if (!curLeftNode)
        return NULL;

    SKIP_SPASES;

    //mul or div
    char curOperator = 0;

    Node *curSignNode = NULL;
    Node *curRightNode = NULL;

    while (pars->code[pars->curCodePos] == '*' ||
           pars->code[pars->curCodePos] == '/')
    {
        SKIP_SPASES;
        curOperator = pars->code[pars->curCodePos];
        pars->curCodePos++;

        switch (curOperator)
        {
            case '*':
            {
                curSignNode = createSimpleNode(Mul, &pars->tree);
                break;
            }
            case '/':
            {
                curSignNode = createSimpleNode(Div, &pars->tree);
                break;
            }
            default:
                return NULL;
        }

        /*
         *                          ____________    ...
         *                         |            (next node)
         *                         | Left
         *                   ______|_______
         *                  |              |
         *           _______| curSignNode  |________
         *          |       |    / or *    |        |
         *          | Left  |______________|  Right |
         *  ________|__                           __|_________
         * |           |                         |            |
         * |curLeftNode|                         |curRightNode|
         * |___________|                         |____________|
         */


        connectLeft(curSignNode, curLeftNode);

        curRightNode = getExpo(pars);
        if (!curRightNode)
        {
            printf("Incorrect expression after %c\n", curOperator);
            return NULL;
        }
        connectRight(curSignNode, curRightNode);

        //Push curSignNode below to connect new nodes.
        curLeftNode = curSignNode;
    }
    return curLeftNode;
}


Node *getExpo(parser *pars)
{
    assert(pars);
    SKIP_SPASES

    Node *curLeftNode = getBranches(pars);
    if (!curLeftNode)
        return NULL;

    Node *curExpoNode = NULL;
    Node *curRightNode = NULL;

    SKIP_SPASES;
    while (pars->code[pars->curCodePos] == '^')
    {
        SKIP_SPASES;
        pars->curCodePos++;

        curExpoNode = createSimpleNode(Expo, &pars->tree);

        /*
         *                          ____________    ...
         *                         |            (next node)
         *                         | Left
         *                   ______|_______
         *                  |              |
         *           _______| curExpoNode  |________
         *          |       |      ^       |        |
         *          | Left  |______________|  Right |
         *  ________|__                           __|_________
         * |           |                         |            |
         * |curLeftNode|                         |curRightNode|
         * |___________|                         |____________|
         */


        connectLeft(curExpoNode, curLeftNode);

        curRightNode = getBranches(pars);
        if (!curRightNode)
        {
            printf("Incorrect expression after \"^\"\n");
            return NULL;
        }
        connectRight(curExpoNode, curRightNode);

        //Push curExpoNode below to connect new nodes.
        curLeftNode = curExpoNode;
    }
    return curLeftNode;
}


Node *getBranches(parser *pars)
{
    assert(pars);
    SKIP_SPASES;

    if (pars->code[pars->curCodePos] == '(')
    {
        pars->curCodePos++;
        Node *node = getAddSub(pars);
        SKIP_SPASES;

        if (pars->code[pars->curCodePos] != ')')
        {
            printf("Error, expected \')\'\n");
            return NULL;
        }
        pars->curCodePos++;
        return node;
    }
    return getFunc(pars);
}


Node *getFunc(parser *pars)
{
    assert(pars);
    SKIP_SPASES;

    size_t tokenSize = 0;
    while (isalpha(pars->code[pars->curCodePos + tokenSize]))
        tokenSize++;

    if (tokenSize == 0)
    {
        Node *Number = getNumber(pars);
        SKIP_SPASES;

        if (pars->code[pars->curCodePos] != '+' && pars->code[pars->curCodePos] != '-' &&
            pars->code[pars->curCodePos] != '*' && pars->code[pars->curCodePos] != '/' &&
            pars->code[pars->curCodePos] != '^' && pars->code[pars->curCodePos] != ')' && pars->code[pars->curCodePos])
        {
            printf("%sAfter %s\n", errorList[ARITHMETIC_OP], Number->content);
            destructNode(Number);
            return NULL;
        }
        return Number;
    }

    Node *func = NULL;
    constructNode(&func);
    func->myTree = &pars->tree;

    func->content = (char *) calloc(tokenSize + 1, sizeof(char));
    memcpy(func->content, pars->code + pars->curCodePos, tokenSize);
    pars->curCodePos += tokenSize;

    int status = analyzeData(func, pars);
    if (status)
        return NULL;

    if (func->type != CurVariable && func->type != CharConst)
    {
        Node *FuncArg = getBranches(pars);
        if (!FuncArg)
        {
            printf("Error, wrong argument at function \"%s\"\n", func->content);
            return NULL;
        }
        connectLeft(func, FuncArg);
    }

    SKIP_SPASES;
    if (pars->code[pars->curCodePos] != '+' && pars->code[pars->curCodePos] != '-' &&
        pars->code[pars->curCodePos] != '*' && pars->code[pars->curCodePos] != '/' &&
        pars->code[pars->curCodePos] != '^' && pars->code[pars->curCodePos] != ')' && pars->code[pars->curCodePos])
    {
        printf("%sAfter \")\"", errorList[ARITHMETIC_OP]);
        destructNodeRec(func);
        return NULL;
    }
    return func;
}


Node *getNumber(parser *pars)
{
    assert(pars);
    SKIP_SPASES;

    int sizeOfNumber = 0;
    double val = 0;

    sscanf(pars->code + pars->curCodePos, "%lg%n", &val, &sizeOfNumber);

    if (sizeOfNumber == 0)
        return NULL;

    pars->curCodePos += sizeOfNumber;
    Node *number = createNumericalNode(Number, val, &pars->tree);

    return number;
}

#define  DEF_CMD_UNARY DEF_CMD

#define  DEF_CMD(operator_, number, func)                   \
    else if (strcmp (#operator_, node->content) == 0)       \
     {                                                      \
        node->type = number;                                \
        return 0;                                           \
    }


int analyzeData(Node *node, parser *pars)
{
    assert(pars);
    assert(node);
    assert(node->content);

    if (strcmp(pars->curVar, node->content) == 0)
    {
        node->type = CurVariable;
        return 0;
    }

#include "../Commands/MathFunc.h"

    else
    {
        SKIP_SPASES;
        if (pars->code[pars->curCodePos] != '(')
        {
            node->type = CharConst;
            return 0;
        }
        else
        {
            printf("%s%s\n", errorList[UNKNOWN_FUNC], node->content);
            return UNKNOWN_FUNC;
        }
    }
}


#undef DEF_CMD
#undef DEF_CMD_UNARY

int throw_error(unsigned int err_num, const char *usr_msg, const char *err_msg, const char *_func, int _line,
                const char *_file)
{
    if (usr_msg && usr_msg[0])
        printf("%s\n", usr_msg);

    if ((err_num < OUT_OF_ERR_RANGE_ERR) && err_msg && _func && _file)
    {
        if (err_msg[0])
            fprintf(stderr, "%s\n", err_msg);
        fprintf(stderr, "Error № %d: "
                        "In function: %s\n"
                        "Line %d\n"
                        "File: %s\n\n", err_num, _func, _line, _file);

        return err_num;
    }
    else return OUT_OF_ERR_RANGE_ERR;
}

#undef DEF_CMD
#undef SKIP_SPASES