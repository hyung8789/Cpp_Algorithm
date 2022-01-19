#include "LCRS_Core.h"

/// <summary>
/// 새로운 노드 생성 및 생성 된 노드 반환
/// </summary>
/// <param name="srcData">노드의 데이터</param>
/// <returns>생성 된 노드</returns>
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
/// 대상 노드에 할당 된 메모리 해제
/// </summary>
/// <param name="srcNode">대상 노드</param>
void LCRS_DeallocateNode(Node** srcNode)
{
	if ((*srcNode) != NULL)
	{
		free(*srcNode);
		(*srcNode) = NULL;
	}
}

/// <summary>
/// 대상 트리에 할당 된 모든 노드의 메모리 해제 (Recursive)
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
void LCRS_DeallocateTree(Node** srcRootNode)
{
	if ((*srcRootNode) != NULL)
	{
#ifdef RECURSIVE_METHOD
		if ((*srcRootNode)->leftChild != NULL)
			LCRS_DeallocateTree(&((*srcRootNode)->leftChild));

		if ((*srcRootNode)->rightSibling != NULL)
			LCRS_DeallocateTree(&((*srcRootNode)->rightSibling));

		LCRS_DeallocateNode(srcRootNode); //최하위 자식 노드의 오른쪽 끝 형제 노드부터 해제

#elif defined ITERATIVE_METHOD
		/***
			1) 최초 대상 트리의 최상위 루트 노드를 메모리 해제 대기열에 삽입

			2) 메모리 해제 대기열이 빌 때까지 이하 반복

				2-1) 메모리 해제 대기열에서 처음 요소 (노드)의 왼쪽 자식 노드가 존재 할 경우
				: 해당 노드의 왼쪽 자식 노드를 메모리 해제 대기열에 삽입

				2-2) 메모리 해제 대기열에서 처음 요소 (노드)의 오른쪽 형제 노드가 존재 할 경우
				: 해당 노드의 오른쪽 형제 노드를 메모리 해제 대기열에 삽입

				2-3) 메모리 해제 대기열에서 처음 요소 (노드) 제거 및 메모리 해제
		***/

		std::queue<Node*> deallocateQueue; //메모리 해제 대기열
		deallocateQueue.push((*srcRootNode));

		while (!deallocateQueue.empty())
		{
			Node* tmp = deallocateQueue.front();
			deallocateQueue.pop();

			if (tmp->leftChild != NULL) //메모리 해제 대기열에서 처음 요소 (노드)의 왼쪽 자식 노드가 존재 할 경우
				deallocateQueue.push(tmp->leftChild);

			if (tmp->rightSibling != NULL) //메모리 해제 대기열에서 처음 요소 (노드)의 오른쪽 형제 노드가 존재 할 경우
				deallocateQueue.push(tmp->rightSibling);

			//std::cout << "Dealloc : " << tmp->data << std::endl;
			LCRS_DeallocateNode(&tmp);
		}
#endif
		(*srcRootNode) = NULL;
	}
}

/// <summary>
/// 대상 노드에 새 자식 노드 연결
/// </summary>
/// <param name="srcTargetNode">대상 노드</param>
/// <param name="srcNewNode">대상 노드에 연결 될 새 자식 노드</param>
void LCRS_AddChildNode(Node* srcTargetNode, Node* srcNewNode)
{
	if (srcTargetNode == NULL || srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (srcTargetNode->leftChild == NULL) //대상 노드의 자식 노드가 존재하지 않으면
	{
		srcTargetNode->leftChild = srcNewNode; //새 노드를 대상 노드의 왼쪽 자식으로 연결
	}
	else //대상 노드의 자식 노드가 존재하면
	{
		Node* targetChildNode = srcTargetNode->leftChild; //대상 노드의 자식 노드

		while (targetChildNode->rightSibling != NULL)
		{
			targetChildNode = targetChildNode->rightSibling;
		}

		targetChildNode->rightSibling = srcNewNode; //대상 노드의 자식 노드의 마지막 형제 노드 다음에 새 노드를 연결
	}
}

/// <summary>
/// 대상 트리의 특정 깊이의 모든 노드 출력
/// </summary>
/// <param name="">대상 트리의 최상위 루트 노드</param>
/// <param name="targetNodeDepth">대상 트리의 특정 노드의 깊이</param>
void LCRS_DispTreeNodesAt(Node* srcRootNode, TreeIndexType targetNodeDepth)
{
	if (srcRootNode == NULL || targetNodeDepth < 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	Node* currentNode = srcRootNode;

	//TODO : 대상 트리의 특정 깊이의 모든 노드 출력
	while (targetNodeDepth > 0) //특정 깊이에 도달 할 때까지
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
/// 대상 트리의 모든 노드 출력
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <param name="rootNodeDepth">대상 트리의 최상위 루트 노드의 깊이</param>
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
	//TODO : RECURSIVE_METHOD와 동일한 출력 순서 구현
	//deque?
	/***
		1) 최초 대상 트리의 최상위 루트 노드를 왼쪽 자식 노드 대기열에 삽입

		//2) 현재 왼쪽 자식 노드 대기열의 처음 요소 출력

		3) 현재 왼쪽 자식 노드 대기열의 처음 요소의 모든 왼쪽 자식 노드를 왼쪽 자식 노드 대기열에 삽입
		3) 현재 왼쪽 자식 노드 대기열의 마지막 노드의 모든 오른쪽 형제 노드를 오른쪽 형제 노드 대기열에 삽입
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