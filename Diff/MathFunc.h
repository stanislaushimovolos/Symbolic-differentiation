//
// Created by Tom on 02.12.2017.
//

#define  DEF_CMD_UNARY(operator_, number, func)                                     \
	DEF_CMD(operator_, number, {Node *LeftCopy = NodeCopy (node->Left, FinalTree);  \
		return complicatedDiff (LeftCopy, FinalTree, MainVariable, func);})


#define  DEF_CMD_BINARY(operator_, number, func)                                    \
	DEF_CMD(operator_, number, {                                                    \
		return func(node, FinalTree, currValue);})


DEF_CMD_UNARY (ln, Ln, lnDiff)

DEF_CMD_UNARY (sin, Sin, sinDiff)

DEF_CMD_UNARY (cos,Cos, cosDiff)

DEF_CMD_BINARY (+, Add, addDiff)

DEF_CMD_BINARY (-, Sub, subDiff)

DEF_CMD_BINARY (*, Mul, mulDiff)

DEF_CMD_BINARY (/, Div, divDiff)

DEF_CMD_BINARY (^,Expo, expoDiff)


#undef DEF_CMD_UNARY

#undef DEF_CMD_BINARY