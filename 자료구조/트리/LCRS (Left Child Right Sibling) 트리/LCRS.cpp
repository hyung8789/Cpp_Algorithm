﻿#include "LCRS_Core.h"

/// <summary>
/// 새로운 노드 생성 및 생성 된 노드 반환
/// </summary>
/// <param name="srcData">노드의 데이터</param>
/// <returns>생성 된 노드</returns>
NODE* LCRS_CreateNode(DATA_TYPE srcData)
{
	NODE* retVal = (NODE*)malloc(sizeof(NODE));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_data = srcData;
	retVal->_leftChild = retVal->_rightSibling = NULL;

	return retVal;
}

/// <summary>
/// 대상 노드에 할당 된 메모리 해제
/// </summary>
/// <param name="srcNode">대상 노드</param>
void LCRS_DeallocateNode(NODE** srcNode)
{
	if ((*srcNode) != NULL)
	{
		free(*srcNode);
		(*srcNode) = NULL;
	}
}

/// <summary>
/// 대상 트리에 할당 된 모든 노드의 메모리 해제
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
void LCRS_DeallocateTree(NODE** srcRootNode)
{
	if ((*srcRootNode) != NULL)
	{
#ifdef RECURSIVE_METHOD
		if ((*srcRootNode)->_leftChild != NULL)
			LCRS_DeallocateTree(&((*srcRootNode)->_leftChild));

		if ((*srcRootNode)->_rightSibling != NULL)
			LCRS_DeallocateTree(&((*srcRootNode)->_rightSibling));

		//std::cout << "Dealloc : " << (*srcRootNode)->_data << std::endl;
		LCRS_DeallocateNode(srcRootNode);

#elif defined ITERATIVE_METHOD
		/***
			< RECURSIVE_METHOD의 Call Stack 구현 >

			1) 최초 Call Stack에 초기 루트 노드와 실행 분기 단일 플래그 삽입

			2) Call Stack이 빌 때까지 이하 반복

			3) 현재 노드 (Call Stack의 최상위 노드) 및 현재 노드의 실행 분기 단일 플래그 할당

			4) 현재 노드의 왼쪽 자식 노드가 존재할 경우
				4-1) 헌재 노드에 대해 다음 번 실행 시 오른쪽 형제 노드를 방문
				4-2) 현재 노드의 왼쪽 자식 노드로 이동

			5) 현재 노드의 오른쪽 형제 노드가 존재할 경우
				5-1) 현재 노드에 대해 다음 번 실행 시 Call Stack에서 pop 및 마지막 작업 수행
				5-2) 현재 노드의 오른쪽 형제 노드로 이동

			6) 현재 노드에서 더 이상 왼쪽 자식 노드와 오른쪽 자식 노드에 대해 이동이 불가능 할 경우
				6-1) 메모리 할당 해제 대상인 Call Stack의 최상위 노드 제거 및 현재 노드 메모리 할당 해제
				6-2) 현재 노드에 도달하기 전 노드로 이동
		***/

		NODE* currentNode = (*srcRootNode); //현재 노드
		char execBranchSingleFlag = (0x0); //실행 분기 단일 플래그

		/***
			< 실행 분기 단일 플래그 >

			0000(2) : Left Child 방문
			0001(2) : Right Sibling 방문
			0010(2) : pop 및 필요 할 경우 해당 요소에 대한 마지막 작업 수행
		***/

		std::stack<std::tuple<NODE*, char>> callStack; //Call Stack
		callStack.push(std::make_tuple(currentNode, execBranchSingleFlag));

		while (!callStack.empty())
		{
			std::tie(currentNode, execBranchSingleFlag) = callStack.top();

			switch (execBranchSingleFlag)
			{
			case (0x0): //Left Child 방문
				if (currentNode->_leftChild != NULL) //현재 노드의 왼쪽 자식 노드가 존재 할 경우
				{
					std::get<1>(callStack.top()) = (0x1); //헌재 노드에 대해 다음 번 실행 시 오른쪽 형제 노드를 방문
					callStack.push(std::make_tuple(currentNode->_leftChild, (0x0)));
					continue;
				}

			case (0x1): //Right Sibling 방문
				if (currentNode->_rightSibling != NULL) //현재 노드의 오른쪽 형제 노드가 존재 할 경우
				{
					std::get<1>(callStack.top()) = (0x2); //현재 노드에 대해 다음 번 실행 시 Call Stack에서 pop 및 필요 할 경우 마지막 작업 수행
					callStack.push(std::make_tuple(currentNode->_rightSibling, (0x0)));
					continue;
				}

			case (0x2): //pop 및 필요 할 경우 해당 요소에 대한 마지막 작업 수행
				callStack.pop();
				//std::cout << "Dealloc : " << currentNode->_data << std::endl;
				LCRS_DeallocateNode(&currentNode);
				break;

			default:
				throw std::logic_error(std::string(__func__) + std::string(" : Invalid Flag"));
			}
		}
#endif
		(*srcRootNode) = NULL;
	}
}

/// <summary>
/// 대상 노드에 새 노드 연결
/// </summary>
/// <param name="srcTargetNode">대상 노드</param>
/// <param name="srcNewNode">대상 노드에 연결 될 새 노드</param>
void LCRS_AppendNode(NODE* srcTargetNode, NODE* srcNewNode)
{
	if (srcTargetNode == NULL || srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (srcTargetNode->_leftChild == NULL) //대상 노드의 자식 노드가 존재하지 않으면
	{
		srcTargetNode->_leftChild = srcNewNode; //새 노드를 대상 노드의 왼쪽 자식으로 연결
	}
	else //대상 노드의 자식 노드가 존재하면
	{
		NODE* targetChildNode = srcTargetNode->_leftChild; //대상 노드의 자식 노드

		while (targetChildNode->_rightSibling != NULL)
		{
			targetChildNode = targetChildNode->_rightSibling;
		}

		targetChildNode->_rightSibling = srcNewNode; //대상 노드의 자식 노드의 마지막 형제 노드 다음에 새 노드를 연결
	}
}

/// <summary>
/// 대상 트리의 특정 깊이의 모든 노드 출력
/// </summary>
/// <param name="">대상 트리의 최상위 루트 노드</param>
/// <param name="targetNodeDepth">대상 트리의 대상 특정 노드의 깊이</param>
void LCRS_DispTreeNodesAt(NODE* srcRootNode, TREE_DEPTH_TYPE targetNodeDepth)
{
	if (srcRootNode == NULL || targetNodeDepth < 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

#ifdef RECURSIVE_METHOD
	if (targetNodeDepth == 0) //대상 특정 노드의 깊이에 도달 시
	{
		std::cout << "- " << srcRootNode->_data << std::endl;
	}

	if (srcRootNode->_leftChild != NULL && targetNodeDepth > 0)
		LCRS_DispTreeNodesAt(srcRootNode->_leftChild, targetNodeDepth - 1);

	if (srcRootNode->_rightSibling != NULL)
		LCRS_DispTreeNodesAt(srcRootNode->_rightSibling, targetNodeDepth);

#elif defined ITERATIVE_METHOD
	NODE* currentNode = srcRootNode; //현재 노드
	TREE_DEPTH_TYPE currentNodeDepth = 0; //현재 노드의 깊이
	char execBranchSingleFlag = (0x0); //실행 분기 단일 플래그

	/***
		< 실행 분기 단일 플래그 >

		0000(2) : 특정 노드의 깊이에 도달 시 현재 노드 출력
		0001(2) : 현재 노드가 대상 특정 노드의 깊이보다 상위 노드일 경우 Left Child 방문
		0010(2) : Right Sibling 방문
		0011(2) : pop 및 필요 할 경우 해당 요소에 대한 마지막 작업 수행
	***/

	std::stack<std::tuple<NODE*, TREE_DEPTH_TYPE, char>> callStack; //Call Stack
	callStack.push(std::make_tuple(currentNode, currentNodeDepth, execBranchSingleFlag));

	while (!callStack.empty())
	{
		std::tie(currentNode, currentNodeDepth, execBranchSingleFlag) = callStack.top();

		switch (execBranchSingleFlag)
		{
		case (0x0): //대상 특정 노드의 깊이에 도달 시 현재 노드 출력
			std::get<2>(callStack.top()) = (0x1); //헌재 노드에 대해 다음 번 실행 시 현재 노드가 특정 노드의 깊이보다 상위 노드일 경우 Left Child 방문

			if (currentNodeDepth == targetNodeDepth) //특정 노드의 깊이에 도달 시
			{
				std::cout << "- " << currentNode->_data << std::endl;
			}

		case (0x1): //현재 노드가 대상 특정 노드의 깊이보다 상위 노드일 경우 Left Child 방문
			if (currentNode->_leftChild != NULL && currentNodeDepth < targetNodeDepth) //현재 노드의 왼쪽 자식 노드가 존재하며, 현재 노드가 특정 노드의 깊이보다 상위 노드일 경우
			{
				std::get<2>(callStack.top()) = (0x2); //헌재 노드에 대해 다음 번 실행 시 오른쪽 형제 노드를 방문
				callStack.push(std::make_tuple(currentNode->_leftChild, currentNodeDepth + 1, (0x0)));
				continue;
			}

		case (0x2): //Right Sibling 방문
			if (currentNode->_rightSibling != NULL) //현재 노드의 오른쪽 형제 노드가 존재 할 경우
			{
				std::get<2>(callStack.top()) = (0x3); //현재 노드에 대해 다음 번 실행 시 Call Stack에서 pop 및 마지막 작업 수행
				callStack.push(std::make_tuple(currentNode->_rightSibling, currentNodeDepth, (0x0)));
				continue;
			}

		case (0x3): //pop 및 필요 할 경우 해당 요소에 대한 마지막 작업 수행
			callStack.pop();
			break;

		default:
			throw std::logic_error(std::string(__func__) + std::string(" : Invalid Flag"));
		}
	}
#endif
}

/// <summary>
/// 대상 트리의 모든 노드 출력
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <param name="rootNodeDepth">대상 트리의 최상위 루트 노드의 깊이</param>
void LCRS_DispTree(NODE* srcRootNode, TREE_DEPTH_TYPE rootNodeDepth)
{
	if (srcRootNode == NULL || rootNodeDepth < 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

#ifdef RECURSIVE_METHOD
	for (TREE_DEPTH_TYPE i = 0; i < rootNodeDepth; i++)
	{
		std::cout << "|";
	}
	std::cout << "- " << srcRootNode->_data << std::endl;

	if (srcRootNode->_leftChild != NULL)
		LCRS_DispTree(srcRootNode->_leftChild, rootNodeDepth + 1);

	if (srcRootNode->_rightSibling != NULL)
		LCRS_DispTree(srcRootNode->_rightSibling, rootNodeDepth);

#elif defined ITERATIVE_METHOD
	NODE* currentNode = srcRootNode; //현재 노드
	TREE_DEPTH_TYPE currentNodeDepth = rootNodeDepth; //현재 노드의 깊이
	char execBranchSingleFlag = (0x0); //실행 분기 단일 플래그

	/***
		< 실행 분기 단일 플래그 >

		0000(2) : 현재 노드 출력
		0001(2) : Left Child 방문
		0010(2) : Right Sibling 방문
		0011(2) : pop 및 필요 할 경우 해당 요소에 대한 마지막 작업 수행
	***/

	std::stack<std::tuple<NODE*, TREE_DEPTH_TYPE, char>> callStack; //Call Stack
	callStack.push(std::make_tuple(currentNode, currentNodeDepth, execBranchSingleFlag));

	while (!callStack.empty())
	{
		std::tie(currentNode, currentNodeDepth, execBranchSingleFlag) = callStack.top();

		switch (execBranchSingleFlag)
		{
		case (0x0): //현재 노드 출력
			std::get<2>(callStack.top()) = (0x1); //헌재 노드에 대해 다음 번 실행 시 왼쪽 자식 노드를 방문

			for (TREE_DEPTH_TYPE i = 0; i < currentNodeDepth; i++)
			{
				std::cout << "|";
			}
			std::cout << "- " << currentNode->_data << std::endl;

		case (0x1): //Left Child 방문
			if (currentNode->_leftChild != NULL)
			{
				std::get<2>(callStack.top()) = (0x2); //헌재 노드에 대해 다음 번 실행 시 오른쪽 형제 노드를 방문
				callStack.push(std::make_tuple(currentNode->_leftChild, currentNodeDepth + 1, (0x0)));
				continue;
			}

		case (0x2): //Right Sibling 방문
			if (currentNode->_rightSibling != NULL)
			{
				std::get<2>(callStack.top()) = (0x3); //현재 노드에 대해 다음 번 실행 시 Call Stack에서 pop 및 필요 할 경우 마지막 작업 수행
				callStack.push(std::make_tuple(currentNode->_rightSibling, currentNodeDepth, (0x0)));
				continue;
			}

		case (0x3): //pop 및 필요 할 경우 해당 요소에 대한 마지막 작업 수행
			callStack.pop();
			break;

		default:
			throw std::logic_error(std::string(__func__) + std::string(" : Invalid Flag"));
		}
	}
#endif
}