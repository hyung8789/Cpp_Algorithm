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
/// ��� Ʈ���� �Ҵ� �� ��� ����� �޸� ���� (Recursive)
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
			
			1) ���� Call Stack�� �ʱ� ��Ʈ ���� ��Ʈ ��忡�� ���� �ڽ� ��� �� ������ ���� ����� �湮 ���� (false) ���� 
			
			2) Call Stack�� �� ������ ���� �ݺ�
			
			3) ���� ��� �� ���� ����� ���� �ڽ� ��� �� ������ ���� ����� �湮 ���� �Ҵ�
				3-1) ���� ��� = Call Stack�� �ֻ��� ���
			
				3-2) ���� ����� ���� �ڽ� ��� �� ������ ���� ����� �湮 ���� = Call Stack�� �ֻ��� ����� �� �湮 ����

			4) ���� ����� ���� �ڽ� ��尡 �����ϸ�, ���� ����� ���� �ڽ� ��忡 ���� ���� �湮���� ���� ���
				4-1) Call Stack�� �ֻ��� ��忡 Call Stack�� �ֻ��� ����� ���� �ڽ� ���� �湮������ ���� (true)

				4-2) Call Stack�� ���� ����� ���� �ڽ� ���� ���� ��忡�� ���� �ڽ� ��� �� ������ ���� ����� �湮 ���� (false) ����
				
				4-3) 2)�� �̵� (���� ����� ���� �ڽ� ���� �̵�)

			4) ���� ����� ������ ���� ��尡 �����ϸ�, ���� ����� ������ ���� ��忡 ���� ���� �湮���� ���� ���
				4-1) Call Stack�� �ֻ��� ��忡 Call Stack�� �ֻ��� ����� ������ ���� ���� �湮������ ���� (true)

				4-2) Call Stack�� ���� ����� ������ ���� ���� ���� ��忡�� ���� �ڽ� ��� �� ������ ���� ����� �湮 ���� (false) ����
				
				4-2) 2)�� �̵� (���� ����� ������ ���� ���� �̵�)
			
			5) ���� ��忡�� �� �̻� ���� �ڽ� ���� ������ �ڽ� ��忡 ���� �̵��� �Ұ��� �� ���
				5-1) �޸� �Ҵ� ���� ����� Call Stack�� �ֻ��� ��� ���� �� ���� ��� �޸� �Ҵ� ����
				
				5-2) 2)�� �̵� (���� ��忡 �����ϱ� �� ���� �̵�) 
				: �޸� �Ҵ� ���� �� ��忡 ���� �� �������� �ʵ��� �̹� �湮 �� ��� ���� �̿�
		***/

		Node* currentNode = (*srcRootNode); //���� ���
		bool isLeftChildVisited = false; //���� ����� ���� �ڽ� ��忡 ���� �̹� �湮 �� ��� ����
		bool isRightSiblingVisited = false; //���� ����� ������ ���� ��忡 ���� �̹� �湮 �� ��� ����

		std::stack<std::tuple<Node*, bool, bool>> callStack; //Call Stack
		callStack.push(std::make_tuple(currentNode, isLeftChildVisited, isRightSiblingVisited));

		while (!callStack.empty())
		{
			currentNode = std::get<0>(callStack.top());
			isLeftChildVisited = std::get<1>(callStack.top());
			isRightSiblingVisited = std::get<2>(callStack.top());
			
			if (currentNode->leftChild != NULL && !isLeftChildVisited)
			{
				std::get<1>(callStack.top()) = true; //���� ��忡�� ���� ����� ���� �ڽ� ���� �湮������ ����

				callStack.push(std::make_tuple(currentNode->leftChild, false, false));
				continue;
			}

			if (currentNode->rightSibling != NULL && !isRightSiblingVisited)
			{
				std::get<2>(callStack.top()) = true; //���� ��忡�� ���� ����� ������ ���� ���� �湮������ ����

				callStack.push(std::make_tuple(currentNode->rightSibling, false, false));
				continue;
			}

			callStack.pop();
			//std::cout << "Dealloc : " << currentNode->data << std::endl;
			LCRS_DeallocateNode(&currentNode);

		}
#endif
		(*srcRootNode) = NULL;
	}
}

/// <summary>
/// ��� ��忡 �� �ڽ� ��� ����
/// </summary>
/// <param name="srcTargetNode">��� ���</param>
/// <param name="srcNewNode">��� ��忡 ���� �� �� �ڽ� ���</param>
void LCRS_AddChildNode(Node* srcTargetNode, Node* srcNewNode)
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
/// <param name="targetNodeDepth">��� Ʈ���� Ư�� ����� ����</param>
void LCRS_DispTreeNodesAt(Node* srcRootNode, TreeIndexType targetNodeDepth)
{
	if (srcRootNode == NULL || targetNodeDepth < 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	Node* currentNode = srcRootNode;

	//TODO : ��� Ʈ���� Ư�� ������ ��� ��� ���
	while (targetNodeDepth > 0) //Ư�� ���̿� ���� �� ������
	{
		if (currentNode->leftChild != NULL)
		{
			currentNode = currentNode->leftChild;
			targetNodeDepth--;
		}

	}

	std::cout << "- " << currentNode->data << std::endl;
}

/// <summary>
/// ��� Ʈ���� ��� ��� ���
/// </summary>
/// <param name="srcRootNode">��� Ʈ���� �ֻ��� ��Ʈ ���</param>
/// <param name="rootNodeDepth">��� Ʈ���� �ֻ��� ��Ʈ ����� ����</param>
void LCRS_DispTree(Node* srcRootNode, TreeIndexType rootNodeDepth)
{
	if (srcRootNode == NULL || rootNodeDepth < 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

#ifdef RECURSIVE_METHOD
	for (TreeIndexType i = 0; i < rootNodeDepth; i++)
	{
		std::cout << "|";
	}
	std::cout << "- " << srcRootNode->data << std::endl;

	if (srcRootNode->leftChild != NULL)
		LCRS_DispTree(srcRootNode->leftChild, rootNodeDepth + 1);

	if (srcRootNode->rightSibling != NULL)
		LCRS_DispTree(srcRootNode->rightSibling, rootNodeDepth);

#elif defined ITERATIVE_METHOD
	/***
		< RECURSIVE_METHOD�� Call Stack ���� >

	***/


#endif
}