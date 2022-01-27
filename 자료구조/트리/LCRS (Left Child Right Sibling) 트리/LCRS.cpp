#include "LCRS_Core.h"

/// <summary>
/// ���ο� ��� ���� �� ���� �� ��� ��ȯ
/// </summary>
/// <param name="srcData">����� ������</param>
/// <returns>���� �� ���</returns>
Node* LCRS_CreateNode(DataType srcData)
{
	Node* retVal = (Node*)malloc(sizeof(Node));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->data = srcData;
	retVal->leftChild = retVal->rightSibling = NULL;

	return retVal;
}

/// <summary>
/// ��� ��忡 �Ҵ� �� �޸� ����
/// </summary>
/// <param name="srcNode">��� ���</param>
void LCRS_DeallocateNode(Node** srcNode)
{
	if ((*srcNode) != NULL)
	{
		free(*srcNode);
		(*srcNode) = NULL;
	}
}

/// <summary>
/// ��� Ʈ���� �Ҵ� �� ��� ����� �޸� ����
/// </summary>
/// <param name="srcRootNode">��� Ʈ���� �ֻ��� ��Ʈ ���</param>
void LCRS_DeallocateTree(Node** srcRootNode)
{
	if ((*srcRootNode) != NULL)
	{
#ifdef RECURSIVE_METHOD
		if ((*srcRootNode)->leftChild != NULL)
			LCRS_DeallocateTree(&((*srcRootNode)->leftChild));

		if ((*srcRootNode)->rightSibling != NULL)
			LCRS_DeallocateTree(&((*srcRootNode)->rightSibling));

		//std::cout << "Dealloc : " << (*srcRootNode)->data << std::endl;
		LCRS_DeallocateNode(srcRootNode);

#elif defined ITERATIVE_METHOD
		/***
			< RECURSIVE_METHOD�� Call Stack ���� >

			1) ���� Call Stack�� �ʱ� ��Ʈ ���� ���� �б� ���� �÷��� ����

			2) Call Stack�� �� ������ ���� �ݺ�

			3) ���� ��� (Call Stack�� �ֻ��� ���) �� ���� ����� ���� �б� ���� �÷��� �Ҵ�

			4) ���� ����� ���� �ڽ� ��尡 ������ ���
				4-1) ���� ��忡 ���� ���� �� ���� �� ������ ���� ��带 �湮
				4-2) ���� ����� ���� �ڽ� ���� �̵�

			5) ���� ����� ������ ���� ��尡 ������ ���
				5-1) ���� ��忡 ���� ���� �� ���� �� Call Stack���� pop �� ������ �۾� ����
				5-2) ���� ����� ������ ���� ���� �̵�

			6) ���� ��忡�� �� �̻� ���� �ڽ� ���� ������ �ڽ� ��忡 ���� �̵��� �Ұ��� �� ���
				6-1) �޸� �Ҵ� ���� ����� Call Stack�� �ֻ��� ��� ���� �� ���� ��� �޸� �Ҵ� ����
				6-2) ���� ��忡 �����ϱ� �� ���� �̵�
		***/

		Node* currentNode = (*srcRootNode); //���� ���
		char execBranchSingleFlag = (0x0); //���� �б� ���� �÷���

		/***
			< ���� �б� ���� �÷��� >

			0000(2) : �ʱ� ���� (������ ����)
			0001(2) : Left Child �湮
			0010(2) : Right Sibling �湮
			0011(2) : pop �� �ʿ� �� ��� �ش� ��ҿ� ���� ������ �۾� ����
		***/

		std::stack<std::tuple<Node*, char>> callStack; //Call Stack
		callStack.push(std::make_tuple(currentNode, execBranchSingleFlag));

		while (!callStack.empty())
		{
			currentNode = std::get<0>(callStack.top());
			execBranchSingleFlag = std::get<1>(callStack.top());

			switch (execBranchSingleFlag)
			{
			case (0x0): //�ʱ� ���� (������ ����)
				break;

			case (0x1): //Left Child �湮
				goto VISIT_LEFT_CHILD_PROC;

			case (0x2): //Right Sibling �湮
				goto VISIT_RIGHT_SIBLING_PROC;

			case (0x3): //pop �� �ʿ� �� ��� �ش� ��ҿ� ���� ������ �۾� ����
				goto FINALIZE_PROC;

			default:
				throw std::logic_error(std::string(__func__) + std::string(" : Invalid Flag"));
			}

		VISIT_LEFT_CHILD_PROC: //0x1
			if (currentNode->leftChild != NULL) //���� ����� ���� �ڽ� ��尡 ���� �� ���
			{
				std::get<1>(callStack.top()) = (0x2); //���� ��忡 ���� ���� �� ���� �� ������ ���� ��带 �湮
				callStack.push(std::make_tuple(currentNode->leftChild, (0x0)));
				continue;
			}

		VISIT_RIGHT_SIBLING_PROC: //0x2
			if (currentNode->rightSibling != NULL) //���� ����� ������ ���� ��尡 ���� �� ���
			{
				std::get<1>(callStack.top()) = (0x3); //���� ��忡 ���� ���� �� ���� �� Call Stack���� pop �� �ʿ� �� ��� ������ �۾� ����
				callStack.push(std::make_tuple(currentNode->rightSibling, (0x0)));
				continue;
			}

		FINALIZE_PROC: //0x3
			callStack.pop();
			//std::cout << "Dealloc : " << currentNode->data << std::endl;
			LCRS_DeallocateNode(&currentNode);
		}
#endif
		(*srcRootNode) = NULL;
	}
}

/// <summary>
/// ��� ��忡 �� ��� ����
/// </summary>
/// <param name="srcTargetNode">��� ���</param>
/// <param name="srcNewNode">��� ��忡 ���� �� �� ���</param>
void LCRS_AppendNode(Node* srcTargetNode, Node* srcNewNode)
{
	if (srcTargetNode == NULL || srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (srcTargetNode->leftChild == NULL) //��� ����� �ڽ� ��尡 �������� ������
	{
		srcTargetNode->leftChild = srcNewNode; //�� ��带 ��� ����� ���� �ڽ����� ����
	}
	else //��� ����� �ڽ� ��尡 �����ϸ�
	{
		Node* targetChildNode = srcTargetNode->leftChild; //��� ����� �ڽ� ���

		while (targetChildNode->rightSibling != NULL)
		{
			targetChildNode = targetChildNode->rightSibling;
		}

		targetChildNode->rightSibling = srcNewNode; //��� ����� �ڽ� ����� ������ ���� ��� ������ �� ��带 ����
	}
}

/// <summary>
/// ��� Ʈ���� Ư�� ������ ��� ��� ���
/// </summary>
/// <param name="">��� Ʈ���� �ֻ��� ��Ʈ ���</param>
/// <param name="targetNodeDepth">��� Ʈ���� ��� Ư�� ����� ����</param>
void LCRS_DispTreeNodesAt(Node* srcRootNode, TreeDepthType targetNodeDepth)
{
	if (srcRootNode == NULL || targetNodeDepth < 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

#ifdef RECURSIVE_METHOD
	if (targetNodeDepth == 0) //��� Ư�� ����� ���̿� ���� ��
	{
		std::cout << "- " << srcRootNode->data << std::endl;
	}

	if (srcRootNode->leftChild != NULL && targetNodeDepth > 0)
		LCRS_DispTreeNodesAt(srcRootNode->leftChild, targetNodeDepth - 1);
	
	if(srcRootNode->rightSibling != NULL)
		LCRS_DispTreeNodesAt(srcRootNode->rightSibling, targetNodeDepth);

#elif defined ITERATIVE_METHOD
	Node* currentNode = srcRootNode; //���� ���
	TreeDepthType currentNodeDepth = 0; //���� ����� ����
	char execBranchSingleFlag = (0x0); //���� �б� ���� �÷���

	/***
		< ���� �б� ���� �÷��� >

		0000(2) : �ʱ� ���� (������ ����)
		0001(2) : Ư�� ����� ���̿� ���� �� ���� ��� ���
		0010(2) : ���� ��尡 ��� Ư�� ����� ���̺��� ���� ����� ��� Left Child �湮
		0011(2) : Right Sibling �湮
		0100(2) : pop �� �ʿ� �� ��� �ش� ��ҿ� ���� ������ �۾� ����
	***/

	std::stack<std::tuple<Node*, TreeDepthType, char>> callStack; //Call Stack
	callStack.push(std::make_tuple(currentNode, currentNodeDepth, execBranchSingleFlag));

	while (!callStack.empty())
	{
		currentNode = std::get<0>(callStack.top());
		currentNodeDepth = std::get<1>(callStack.top());
		execBranchSingleFlag = std::get<2>(callStack.top());

		switch (execBranchSingleFlag)
		{
		case (0x0): //�ʱ� ���� (������ ����)
			break;

		case (0x1): //��� Ư�� ����� ���̿� ���� �� ���� ��� ���
			goto DISP_CURRENT_NODE_PROC;

		case (0x2): //���� ��尡 ��� Ư�� ����� ���̺��� ���� ����� ��� Left Child �湮
			goto VISIT_LEFT_CHILD_PROC;

		case (0x3): //Right Sibling �湮
			goto VISIT_RIGHT_SIBLING_PROC;

		case (0x4): //pop �� �ʿ� �� ��� �ش� ��ҿ� ���� ������ �۾� ����
			goto FINALIZE_PROC;

		default:
			throw std::logic_error(std::string(__func__) + std::string(" : Invalid Flag"));
		}

	DISP_CURRENT_NODE_PROC: //0x1
		std::get<2>(callStack.top()) = (0x2); //���� ��忡 ���� ���� �� ���� �� ���� ��尡 Ư�� ����� ���̺��� ���� ����� ��� Left Child �湮

		if (currentNodeDepth == targetNodeDepth) //Ư�� ����� ���̿� ���� ��
		{
			std::cout << "- " << currentNode->data << std::endl;
		}
		
	VISIT_LEFT_CHILD_PROC: //0x2
		if (currentNode->leftChild != NULL && currentNodeDepth < targetNodeDepth) //���� ����� ���� �ڽ� ��尡 �����ϸ�, ���� ��尡 Ư�� ����� ���̺��� ���� ����� ���
		{
			std::get<2>(callStack.top()) = (0x3); //���� ��忡 ���� ���� �� ���� �� ������ ���� ��带 �湮
			callStack.push(std::make_tuple(currentNode->leftChild, currentNodeDepth + 1, (0x0)));
			continue;
		}

	VISIT_RIGHT_SIBLING_PROC: //0x3
		if (currentNode->rightSibling != NULL) //���� ����� ������ ���� ��尡 ���� �� ���
		{
			std::get<2>(callStack.top()) = (0x4); //���� ��忡 ���� ���� �� ���� �� Call Stack���� pop �� ������ �۾� ����
			callStack.push(std::make_tuple(currentNode->rightSibling, currentNodeDepth, (0x0)));
			continue;
		}

	FINALIZE_PROC: //0x4
		callStack.pop();
	}
#endif
}

/// <summary>
/// ��� Ʈ���� ��� ��� ���
/// </summary>
/// <param name="srcRootNode">��� Ʈ���� �ֻ��� ��Ʈ ���</param>
/// <param name="rootNodeDepth">��� Ʈ���� �ֻ��� ��Ʈ ����� ����</param>
void LCRS_DispTree(Node* srcRootNode, TreeDepthType rootNodeDepth)
{
	if (srcRootNode == NULL || rootNodeDepth < 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

#ifdef RECURSIVE_METHOD
	for (TreeDepthType i = 0; i < rootNodeDepth; i++)
	{
		std::cout << "|";
	}
	std::cout << "- " << srcRootNode->data << std::endl;

	if (srcRootNode->leftChild != NULL)
		LCRS_DispTree(srcRootNode->leftChild, rootNodeDepth + 1);

	if (srcRootNode->rightSibling != NULL)
		LCRS_DispTree(srcRootNode->rightSibling, rootNodeDepth);

#elif defined ITERATIVE_METHOD
	Node* currentNode = srcRootNode; //���� ���
	TreeDepthType currentNodeDepth = rootNodeDepth; //���� ����� ����
	char execBranchSingleFlag = (0x0); //���� �б� ���� �÷���

	/***
		< ���� �б� ���� �÷��� >

		0000(2) : �ʱ� ���� (������ ����)
		0001(2) : ���� ��� ���
		0010(2) : Left Child �湮
		0011(2) : Right Sibling �湮
		0100(2) : pop �� �ʿ� �� ��� �ش� ��ҿ� ���� ������ �۾� ����
	***/

	std::stack<std::tuple<Node*, TreeDepthType, char>> callStack; //Call Stack
	callStack.push(std::make_tuple(currentNode, currentNodeDepth, execBranchSingleFlag));

	while (!callStack.empty())
	{
		currentNode = std::get<0>(callStack.top());
		currentNodeDepth = std::get<1>(callStack.top());
		execBranchSingleFlag = std::get<2>(callStack.top());

		switch (execBranchSingleFlag)
		{
		case (0x0): //�ʱ� ���� (������ ����)
			break;

		case (0x1): //���� ��� ���
			goto DISP_CURRENT_NODE_PROC;

		case (0x2): //Left Child �湮
			goto VISIT_LEFT_CHILD_PROC;

		case (0x3): //Right Sibling �湮
			goto VISIT_RIGHT_SIBLING_PROC;

		case (0x4): //pop �� �ʿ� �� ��� �ش� ��ҿ� ���� ������ �۾� ����
			goto FINALIZE_PROC;

		default:
			throw std::logic_error(std::string(__func__) + std::string(" : Invalid Flag"));
		}

	DISP_CURRENT_NODE_PROC: //0x1
		std::get<2>(callStack.top()) = (0x2); //���� ��忡 ���� ���� �� ���� �� ���� �ڽ� ��带 �湮

		for (TreeDepthType i = 0; i < currentNodeDepth; i++)
		{
			std::cout << "|";
		}
		std::cout << "- " << currentNode->data << std::endl;

	VISIT_LEFT_CHILD_PROC: //0x2
		if (currentNode->leftChild != NULL)
		{
			std::get<2>(callStack.top()) = (0x3); //���� ��忡 ���� ���� �� ���� �� ������ ���� ��带 �湮
			callStack.push(std::make_tuple(currentNode->leftChild, currentNodeDepth + 1, (0x0)));
			continue;
		}

	VISIT_RIGHT_SIBLING_PROC: //0x3
		if (currentNode->rightSibling != NULL)
		{
			std::get<2>(callStack.top()) = (0x4); //���� ��忡 ���� ���� �� ���� �� Call Stack���� pop �� �ʿ� �� ��� ������ �۾� ����
			callStack.push(std::make_tuple(currentNode->rightSibling, currentNodeDepth, (0x0)));
			continue;
		}

	FINALIZE_PROC: //0x4
		callStack.pop();
	}
#endif
}