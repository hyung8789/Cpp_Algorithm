﻿#include "RBT_Core.h"

/// <summary>
/// 새로운 노드 생성 및 생성 된 노드 반환
/// </summary>
/// <param name="srcData">노드의 데이터</param>
/// <param name="color">노드의 색</param>
/// <returns>생성 된 노드</returns>
Node* RBT_CreateNode(DataType srcData)
{
	Node* retVal = (Node*)malloc(sizeof(Node));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_data = srcData;
	retVal->_color = COLOR::RED; //항상 새 노드는 빨간색으로 가정
	retVal->_parent = retVal->_left = retVal->_right = NULL;

	return retVal;
}

/// <summary>
/// 대상 트리에 새 노드 삽입
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <param name="srcNewNode">새 노드</param>
void RBT_InsertNode(Node** srcRootNode, Node* srcNewNode)
{
	if (dummyBlackTerminalNode == NULL) //초기 생성 혹은 트리 할당 해제 후 재 생성 시
	{
		dummyBlackTerminalNode = RBT_CreateNode(0);
		dummyBlackTerminalNode->_color = COLOR::BLACK;
	}

	if (dummyBlackTerminalNode->_parent != NULL) //검은색 더미 단말 노드에서 부모로의 연결은 허용하지 않음 
		throw std::logic_error(std::string(__func__) + std::string(" : Not allowed parent connection"));

	if (srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	/***
		< 새 노드 삽입에 따른 레드 블랙 트리의 정의 위반에 대한 후속 처리 >

		항상 삽입되는 새 노드는 빨간색으로 가정하며,
		단말 노드는 항상 검은색인 정의을 만족하기 위해 별도의 검은색 더미 단말 노드를 사용
		이에 따른, 새 노드 삽입 시 위반되는 정의와 수정 사항은,

		1) 루트 노드는 항상 검은색
		: 삽입되는 새 노드는 항상 빨간색이므로 위반

			1-1) 삽입되는 새 노드가 루트 노드일 시 검은색으로 변경

		2) 빨간 노드의 한 단계 하위 자식 노드는 항상 검은색
		: 삽입되는 새 노드는 항상 빨간색이므로, 빨간 노드 다음에 빨간 노드가 삽입 될 경우 위반 (부모 노드와 부모 노드의 자식인 새 노드가 빨간색)
		삽입되는 새 노드의 부모가 빨간색이면 삽입되는 새 노드를 검은색으로 변경만 할 경우,
		왜 삼촌


			2-1) 새 노드의 부모의 형제 노드 (새 노드의 부모의 부모의 반대쪽 자식 노드 (삼촌))가 존재 할 경우
			: 새 노드의 부모의 형제 노드의 색에 따라,

				2-1-1) 새 노드의 부모의 형제 노드가 빨간색일 경우
				:

				2-1-2) 새 노드의 부모의 형제 노드가

			2-2) 새 노드의 부모의 형제 노드가 존재하지 않을 경우
			:

		---

		ex) 새 노드의 부모의 형제 노드가 존재하지 않을 경우

				5 (Root, Black)
			3 (Parent, Red)
		1 (New, Red)

		=> 부모 노드 3를 기준으로 우회전 수행

				3
					5


	***/

	if ((*srcRootNode) == NULL) //초기 루트가 존재하지 않을 경우
	{
		(*srcRootNode) = srcNewNode;
		srcNewNode->_color = COLOR::BLACK; //루트 노드를 검은색으로 변경
		srcNewNode->_left = srcNewNode->_right = dummyBlackTerminalNode; //단말 노드는 항상 검은색
	}
	else //초기 루트 노드가 존재 할 경우
	{
		if ((*srcRootNode)->_data == srcNewNode->_data) //중복 데이터가 존재 할 경우
			throw myexception::NOT_ALLOWED_DUPLICATE_DATA_EXCEPTION(std::string(__func__) + std::string(" : Not allowed duplicate data"));

		if ((*srcRootNode)->_data > srcNewNode->_data) //현재 노드의 데이터 > 삽입하고자 하는 노드의 데이터인 경우
		{
			if ((*srcRootNode)->_left == dummyBlackTerminalNode) //현재 노드의 왼쪽 자식이 검은색 더미 단말 노드일 경우
			{
				(*srcRootNode)->_left = srcNewNode;
				srcNewNode->_parent = (*srcRootNode);
				srcNewNode->_left = srcNewNode->_right = dummyBlackTerminalNode; //단말 노드는 항상 검은색
			}
			else
			{
				RBT_InsertNode(&((*srcRootNode)->_left), srcNewNode); //현재 노드의 왼쪽 자식이 검은색 단말 노드일 때까지 탐색 수행
			}
		}
		else //현재 노드의 데이터 < 삽입하고자 하는 노드의 데이터인 경우
		{
			if ((*srcRootNode)->_right == dummyBlackTerminalNode) //현재 노드의 오른쪽 자식이 검은색 더미 단말 노드일 경우
			{
				(*srcRootNode)->_right = srcNewNode;
				srcNewNode->_parent = (*srcRootNode);
				srcNewNode->_left = srcNewNode->_right = dummyBlackTerminalNode; //단말 노드는 항상 검은색
			}
			else
			{
				RBT_InsertNode(&((*srcRootNode)->_right), srcNewNode); //현재 노드의 오른쪽 자식이 검은색 단말 노드일 때까지 탐색 수행
			}
		}
	}
}

/// <summary>
/// 대상 트리의 대상 부모 노드와 대상 부모 노드의 자식 노드 간에 회전 방향에 따른 회전 수행
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <param name="targetParentNode">대상 부모 노드</param>
/// <param name="rotateDirection">회전 방향</param>
void RBT_RotateTree(Node** srcRootNode, Node* targetParentNode, ROTATE_DIRECTION rotateDirection)
{
	/***
		< 트리 회전 - 우회전 >

		! 레드 블랙 트리 정의인 루트 노드 및 단말 노드는 항상 검은색를 만족하기 위해,
		별도의 검은색 더미 노드를 단말 노드로 이용하며, 
		이에 따라 어떠한 노드의 왼쪽 자식 노드 혹은 오른쪽 자식 노드는 항상 NULL이 아님

		! 대상 부모 노드가 루트 노드일 경우 별도의 처리 요구

		1) 대상 부모 노드의 왼쪽 자식 노드와 검은색 더미 단말 노드의 일치 여부에 따라,
		(회전 방향에 따라, 회전을 위해 이동 될 자식 노드 및 회전을 위해 이동 될 자식 노드의 자식 변동)

			1-1) 대상 부모 노드의 왼쪽 자식 노드 == 검은색 더미 단말 노드
			: 적절하지 않는 상황에 회전 시도, 예외 발생

			1-2) 대상 부모 노드의 왼쪽 자식 노드 != 검은색 더미 단말 노드
			: 대상 부모 노드의 왼쪽 자식 노드의 오른쪽 자식 노드와 검은색 더미 단말 노드의 일치 여부에 따라,

				1-2-1) 대상 부모 노드의 왼쪽 자식 노드의 오른쪽 자식 노드 == 검은색 더미 단말 노드
				: do nothing (검은색 더미 단말 노드는 부모와의 연결을 가지지 않음)

				1-2-2) 대상 부모 노드의 왼쪽 자식 노드의 오른쪽 자식 노드 != 검은색 더미 단말 노드
				: 대상 부모 노드의 왼쪽 자식 노드의 오른쪽 자식 노드의 부모를 대상 부모 노드로 연결
				(대상 부모 노드의 왼쪽 자식 노드 < 대상 부모 노드의 왼쪽 자식 노드의 오른쪽 자식 노드 < 대상 부모 노드)

		2) 대상 부모 노드와 루트 노드 일치 여부에 따라,

			2-1) 대상 부모 노드 == 루트 노드
			: 루트 노드를 대상 부모 노드의 왼쪽 자식 노드로 변경

			2-2) 대상 부모 노드 != 루트 노드

				2-2-1) 대상 부모 노드의 왼쪽 자식 노드의 부모를 대상 부모 노드의 부모로 연결

				2-2-2) 대상 부모 노드의 부모의 자식 (왼쪽 혹은 오른쪽)을 대상 부모 노드의 왼쪽 자식 노드로 연결

		3) 대상 부모 노드의 왼쪽 자식 노드의 오른쪽을 대상 부모 노드로 연결

		4) 대상 부모 노드의 부모를 대상 부모 노드의 왼쪽 자식 노드로 연결

		5) 대상 부모 노드의 왼쪽을 대상 부모 노드의 왼쪽 자식의 오른쪽으로 연결

		---

		ex)
					8 (회전 대상 부모 노드)
			5				9
		3		6

		<= 5를 기준으로 좌회전 후,
		=> 8을 기준으로 우회전 후,

					5 (회전 대상 부모 노드)
			3				8
						6		9
	***/

	if ((*srcRootNode) == NULL || targetParentNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	Node* moveTargetNode = NULL; //회전을 위해 이동 될 노드
	Node* moveTargetChildNode = NULL; //회전을 위해 이동 될 노드의 자식 노드

	Node** moveTargetParentToChildConnection = NULL; //회전을 위해 이동 될 노드의 부모 노드에서 회전을 위해 이동 될 노드로의 연결
	Node** moveTargetToChildConnection = NULL; //회전을 위해 이동 될 노드에서 회전을 위해 이동 될 노드의 자식 노드로의 연결

	switch (rotateDirection)
	{
	case ROTATE_DIRECTION::LEFT:
		moveTargetNode = targetParentNode->_left;
		moveTargetChildNode = moveTargetNode->_right;
		moveTargetParentToChildConnection = &(moveTargetNode->_parent->_left);
		moveTargetToChildConnection = &(moveTargetNode->_right);
		break;

	case ROTATE_DIRECTION::RIGHT:
		moveTargetNode = targetParentNode->_right;
		moveTargetChildNode = moveTargetNode->_left;
		moveTargetParentToChildConnection = &(moveTargetNode->_parent->_right);
		moveTargetToChildConnection = &(moveTargetNode->_left);
		break;
	}

	if (moveTargetNode == NULL || moveTargetChildNode == NULL ||
		moveTargetParentToChildConnection == NULL || moveTargetToChildConnection == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Invalid Red-Black Tree"));

	if (moveTargetNode == dummyBlackTerminalNode)
		throw std::logic_error(std::string(__func__) + std::string(" : Wrong rotate cond"));

	if (moveTargetChildNode != dummyBlackTerminalNode)
		moveTargetChildNode->_parent = targetParentNode;

	if (targetParentNode == (*srcRootNode)) //대상 부모 노드가 루트 노드일 경우
	{
		(*srcRootNode) = moveTargetNode;
	}
	else //대상 부모 노드가 루트 노드가 아닐 경우
	{
		//우회전 : 대상 부모 노드의 왼쪽 노드에서 부모를 대상 부모 노드의 부모 노드로 연결
		moveTargetNode->_parent = targetParentNode->_parent;

		//우회전 : 대상 부모 노드의 부모에서 대상 부모 노드로의 연결을 대상 부모 노드의 왼쪽 자식 노드로 연결
		(targetParentNode->_parent->_left == targetParentNode) ?
			targetParentNode->_parent->_left = moveTargetNode : targetParentNode->_parent->_right = moveTargetNode;
	}

	//우회전 : 대상 부모 노드의 왼쪽 자식 노드에서 오른쪽 자식 노드로의 연결을 대상 부모 노드로 연결
	(*moveTargetToChildConnection) = targetParentNode;

	//우회전 : 대상 부모 노드의 부모를 대상 부모 노드의 왼쪽 자식 노드로 연결
	targetParentNode->_parent = moveTargetNode;

	//우회전 : 대상 부모 노드에서 왼쪽을 대상 부모 노드의 왼쪽 자식 노드의 오른쪽 자식 노드로 연결
	(*moveTargetParentToChildConnection) = moveTargetChildNode;
}