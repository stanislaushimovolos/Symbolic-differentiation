//
// Created by Tom on 22.11.2017.
//


DEF_CMD_BIN_OPERATOR(+, binOperator_, {

	Node *LeftCopy = NodeCopy (node->Left, FinalTree);
	LeftCopy->myTree = FinalTree;

	Node *RightCopy = NodeCopy (node->Right, FinalTree);
	RightCopy->myTree = FinalTree;

	Node *dL = diffRec (LeftCopy, currValue, FinalTree);
	Node *dR = diffRec (RightCopy, currValue, FinalTree);

	Node *mainNode = createNode (binOperator_, *node->content, FinalTree);

	connectLeft (mainNode, dL);
	connectRight (mainNode, dR);

	return mainNode;
})


DEF_CMD_BIN_OPERATOR(*, binOperator_, {

	Node *LeftCopy = NodeCopy (node->Left, FinalTree);
	LeftCopy->myTree = FinalTree;

	Node *RightCopy = NodeCopy (node->Right, FinalTree);
	RightCopy->myTree = FinalTree;

	Node *dL = diffRec (LeftCopy, currValue, FinalTree);
	Node *dR = diffRec (RightCopy, currValue, FinalTree);

	Node *mainNode = createNode (binOperator_, '+', FinalTree);

	Node *FirstProNode = createNode (binOperator_, *node->content, FinalTree);
	Node *SecondProNode = createNode (binOperator_, *node->content, FinalTree);

	connectRight (FirstProNode, dL);
	connectLeft (FirstProNode, RightCopy);

	connectRight (SecondProNode, dR);
	connectLeft (SecondProNode, LeftCopy);

	connectLeft (mainNode, FirstProNode);
	connectRight (mainNode, SecondProNode);

	return mainNode;
})

DEF_CMD_BIN_OPERATOR(/, binOperator_, {
	Node *LeftCopy = NodeCopy (node->Left, FinalTree);
	LeftCopy->myTree = FinalTree;

	Node *RightCopy = NodeCopy (node->Right, FinalTree);
	RightCopy->myTree = FinalTree;

	Node *dL = diffRec (LeftCopy, currValue, FinalTree);
	Node *dR = diffRec (RightCopy, currValue, FinalTree);

	Node *minusNode = createNode (binOperator_, '-', FinalTree);

	Node *FirstProNode = createNode (binOperator_, '*', FinalTree);
	Node *SecondProNode = createNode (binOperator_, '*', FinalTree);

	connectRight (FirstProNode, dL);
	connectLeft (FirstProNode, RightCopy);

	connectRight (SecondProNode, dR);
	connectLeft (SecondProNode, LeftCopy);

	connectLeft (minusNode, FirstProNode);
	connectRight (minusNode, SecondProNode);

	Node *denPro = createNode (binOperator_, '*', FinalTree);

	Node *RightCopy1 = NodeCopy (node->Right, FinalTree);
	RightCopy1->myTree = FinalTree;


	Node *RightCopy2 = NodeCopy (node->Right, FinalTree);
	RightCopy2->myTree = FinalTree;

	connectRight (denPro, RightCopy1);
	connectLeft (denPro, RightCopy2);

	Node *MainNode = createNode (binOperator_, '/', FinalTree);

	connectLeft (MainNode, minusNode);
	connectRight (MainNode, denPro);

	return MainNode;
})

DEF_CMD_BIN_OPERATOR(-, binOperator_, {
	Node *LeftCopy = NodeCopy (node->Left, FinalTree);
	LeftCopy->myTree = FinalTree;

	Node *RightCopy = NodeCopy (node->Right, FinalTree);
	RightCopy->myTree = FinalTree;

	Node *dL = diffRec (LeftCopy, currValue, FinalTree);
	Node *dR = diffRec (RightCopy, currValue, FinalTree);

	Node *mainNode = createNode (binOperator_, *node->content, FinalTree);

	connectLeft (mainNode, dL);
	connectRight (mainNode, dR);

	return mainNode;
})

DEF_CMD_BIN_OPERATOR(^, binOperator_, {})
