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

		LCRS_DeallocateNode(srcRootNode); //������ �ڽ� ����� ������ �� ���� ������ ����

#elif defined ITERATIVE_METHOD
		/***
			1) ���� ��� Ʈ���� �ֻ��� ��Ʈ ��带 �޸� ���� ��⿭�� ����

			2) �޸� ���� ��⿭�� �� ������ ���� �ݺ�

				2-1) �޸� ���� ��⿭���� ó�� ��� (���)�� ���� �ڽ� ��尡 ���� �� ���
				: �ش� ����� ���� �ڽ� ��带 �޸� ���� ��⿭�� ����

				2-2) �޸� ���� ��⿭���� ó�� ��� (���)�� ������ ���� ��尡 ���� �� ���
				: �ش� ����� ������ ���� ��带 �޸� ���� ��⿭�� ����

				2-3) �޸� ���� ��⿭���� ó�� ��� (���) ���� �� �޸� ����
		***/

		std::queue<Node*> deallocateQueue; //�޸� ���� ��⿭
		deallocateQueue.push((*srcRootNode));

		while (!deallocateQueue.empty())
		{
			Node* tmp = deallocateQueue.front();
			deallocateQueue.pop();

			if (tmp->leftChild != NULL) //�޸� ���� ��⿭���� ó�� ��� (���)�� ���� �ڽ� ��尡 ���� �� ���
				deallocateQueue.push(tmp->leftChild);

			if (tmp->rightSibling != NULL) //�޸� ���� ��⿭���� ó�� ��� (���)�� ������ ���� ��尡 ���� �� ���
				deallocateQueue.push(tmp->rightSibling);

			//std::cout << "Dealloc : " << tmp->data << std::endl;
			LCRS_DeallocateNode(&tmp);
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
	//TODO : RECURSIVE_METHOD�� ������ ��� ���� ����
	//deque?
	/***
		1) ���� ��� Ʈ���� �ֻ��� ��Ʈ ��带 ���� �ڽ� ��� ��⿭�� ����

		//2) ���� ���� �ڽ� ��� ��⿭�� ó�� ��� ���

		3) ���� ���� �ڽ� ��� ��⿭�� ó�� ����� ��� ���� �ڽ� ��带 ���� �ڽ� ��� ��⿭�� ����
		3) ���� ���� �ڽ� ��� ��⿭�� ������ ����� ��� ������ ���� ��带 ������ ���� ��� ��⿭�� ����
	***/

	std::queue<QueueElement> leftChlidNodeQueue;
	std::queue<QueueElement> rightSiblingNodeQueue;

	QueueElement element;
	element.node = srcRootNode;
	element.nodeDepth = rootNodeDepth;
	leftChlidNodeQueue.push(element);



	while (!leftChlidNodeQueue.empty())
	{
		element = leftChlidNodeQueue.front();
		leftChlidNodeQueue.pop();

		std::cout << element.nodeDepth << std::endl;
	}
#endif
}