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

DEF_CMD_BIN_OPERATOR(^, binOperator_, {

	char ln[] = "ln";

	Node *base1 = NodeCopy (node->Left, FinalTree);
	Node *base2 = NodeCopy (node->Left, FinalTree);
	Node *base3 = NodeCopy (node->Left, FinalTree);
	Node *base4 = NodeCopy (node->Left, FinalTree);

	Node *index1 = NodeCopy (node->Right, FinalTree);
	Node *index2 = NodeCopy (node->Right, FinalTree);
	Node *index3 = NodeCopy (node->Right, FinalTree);
	Node *index4 = NodeCopy (node->Right, FinalTree);

	Node *mainMinus = createNode (binOperator_, '-', FinalTree);
	Node *MainPlus = createNode (binOperator_, '+', FinalTree);
	Node *ProNode1 = createNode (binOperator_, '*', FinalTree);
	Node *ProNode2 = createNode (binOperator_, '*', FinalTree);
	Node *ProNode3 = createNode (binOperator_, '*', FinalTree);
	Node *ProNode4 = createNode (binOperator_, '*', FinalTree);

	Node *Cap1 = createNode (binOperator_, '^', FinalTree);
	Node *Cap2 = createNode (binOperator_, '^', FinalTree);

	Node *Ln = {};
	nodeConstruct (&Ln);
	nodeSetName (Ln, ln);

	connectLeft (ProNode1, Ln);
	connectLeft (Ln, base1);
	connectRight (ProNode1, diffRec (index1, currValue, FinalTree));
	connectLeft (ProNode2, ProNode1);
	connectRight (ProNode2, Cap1);
	connectLeft (Cap1, base2);
	connectRight (Cap1, index2);
	connectRight (ProNode2, Cap1);
	connectLeft (MainPlus, ProNode2);

	connectRight (MainPlus, ProNode3);
	connectRight (ProNode3, Cap2);
	connectLeft (ProNode3, ProNode4);

	connectLeft (ProNode4, index3);
	connectRight (ProNode4, diffRec (base3, currValue, FinalTree));
	connectLeft (Cap2, base4);
	connectRight (Cap2, mainMinus);

	connectRight (mainMinus, createNode (number, 1, FinalTree));
	connectLeft (mainMinus, index4);

	return MainPlus;
})
