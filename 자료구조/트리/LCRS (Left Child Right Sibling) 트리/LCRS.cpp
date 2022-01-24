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

		//std::cout << "Dealloc : " << (*srcRootNode)->data << std::endl;
		LCRS_DeallocateNode(srcRootNode);

#elif defined ITERATIVE_METHOD
		/***
			< RECURSIVE_METHOD의 Call Stack 구현 >
			
			1) 최초 Call Stack에 초기 루트 노드와 루트 노드에서 왼쪽 자식 노드 및 오른쪽 형제 노드의 방문 여부 (false) 삽입 
			
			2) Call Stack이 빌 때까지 이하 반복
			
			3) 현재 노드 및 현재 노드의 왼쪽 자식 노드 및 오른쪽 형제 노드의 방문 여부 할당
				3-1) 현재 노드 = Call Stack의 최상위 노드
			
				3-2) 현재 노드의 왼쪽 자식 노드 및 오른쪽 형제 노드의 방문 여부 = Call Stack의 최상위 노드의 각 방문 여부

			4) 현재 노드의 왼쪽 자식 노드가 존재하며, 현재 노드의 왼쪽 자식 노드에 대해 아직 방문하지 않은 경우
				4-1) Call Stack의 최상위 노드에 Call Stack의 최상위 노드의 왼쪽 자식 노드로 방문했음을 저장 (true)

				4-2) Call Stack에 현재 노드의 왼쪽 자식 노드와 현재 노드에서 왼쪽 자식 노드 및 오른쪽 형제 노드의 방문 여부 (false) 삽입
				
				4-3) 2)로 이동 (현재 노드의 왼쪽 자식 노드로 이동)

			4) 현재 노드의 오른쪽 형제 노드가 존재하며, 현재 노드의 오른쪽 형제 노드에 대해 아직 방문하지 않은 경우
				4-1) Call Stack의 최상위 노드에 Call Stack의 최상위 노드의 오른쪽 형제 노드로 방문했음을 저장 (true)

				4-2) Call Stack에 현재 노드의 오른쪽 형제 노드와 현재 노드에서 왼쪽 자식 노드 및 오른쪽 형제 노드의 방문 여부 (false) 삽입
				
				4-2) 2)로 이동 (현재 노드의 오른쪽 형제 노드로 이동)
			
			5) 현재 노드에서 더 이상 왼쪽 자식 노드와 오른쪽 자식 노드에 대해 이동이 불가능 할 경우
				5-1) 메모리 할당 해제 대상인 Call Stack의 최상위 노드 제거 및 현재 노드 메모리 할당 해제
				
				5-2) 2)로 이동 (현재 노드에 도달하기 전 노드로 이동) 
				: 메모리 할당 해제 된 노드에 대해 재 접근하지 않도록 이미 방문 된 노드 여부 이용
		***/

		Node* currentNode = (*srcRootNode); //현재 노드
		bool isLeftChildVisited = false; //현재 노드의 왼쪽 자식 노드에 대해 이미 방문 된 노드 여부
		bool isRightSiblingVisited = false; //현재 노드의 오른쪽 형제 노드에 대해 이미 방문 된 노드 여부

		std::stack<std::tuple<Node*, bool, bool>> callStack; //Call Stack
		callStack.push(std::make_tuple(currentNode, isLeftChildVisited, isRightSiblingVisited));

		while (!callStack.empty())
		{
			currentNode = std::get<0>(callStack.top());
			isLeftChildVisited = std::get<1>(callStack.top());
			isRightSiblingVisited = std::get<2>(callStack.top());
			
			if (currentNode->leftChild != NULL && !isLeftChildVisited)
			{
				std::get<1>(callStack.top()) = true; //현재 노드에서 현재 노드의 왼쪽 자식 노드로 방문했음을 저장

				callStack.push(std::make_tuple(currentNode->leftChild, false, false));
				continue;
			}

			if (currentNode->rightSibling != NULL && !isRightSiblingVisited)
			{
				std::get<2>(callStack.top()) = true; //현재 노드에서 현재 노드의 오른쪽 형제 노드로 방문했음을 저장

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
	/***
		< RECURSIVE_METHOD의 Call Stack 구현 >

	***/


#endif
}