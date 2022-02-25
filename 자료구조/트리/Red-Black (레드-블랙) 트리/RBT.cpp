#include "RBT_Core.h"

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
	retVal->_color = COLOR::RED; //항상 새 노드는 빨간색
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

	RBT_InsertNodeHelper(srcRootNode, srcNewNode); //새 노드 삽입
	srcNewNode->_left = srcNewNode->_right = dummyBlackTerminalNode;
	RBT_PostProcAfterInsert(srcRootNode, srcNewNode); //후처리 수행
}

/// <summary>
/// 직접적인 대상 트리에 새 노드 삽입 수행
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <param name="srcNewNode">새 노드</param>
void RBT_InsertNodeHelper(Node** srcRootNode, Node* srcNewNode)
{
	if (srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if ((*srcRootNode) == NULL) //초기 루트가 존재하지 않을 경우
	{
		(*srcRootNode) = srcNewNode;
	}
	else //초기 루트 노드가 존재 할 경우
	{
		if ((*srcRootNode)->_data == srcNewNode->_data) //중복 데이터가 존재 할 경우
			throw myexception::NOT_ALLOWED_DUPLICATE_DATA_EXCEPTION(std::string(__func__) + std::string(" : Not allowed duplicate data"));

		if ((*srcRootNode)->_data > srcNewNode->_data) //현재 노드의 데이터 > 삽입하고자 하는 노드의 데이터인 경우
		{
			if ((*srcRootNode)->_left == dummyBlackTerminalNode) //현재 노드의 왼쪽 자식이 검은색 더미 단말 노드일 경우
			{
				//현재 노드의 왼쪽에 새 노드 삽입
				(*srcRootNode)->_left = srcNewNode;
				srcNewNode->_parent = (*srcRootNode);
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
				//현재 노드의 오른쪽에 새 노드 삽입
				(*srcRootNode)->_right = srcNewNode;
				srcNewNode->_parent = (*srcRootNode);
			}
			else
			{
				RBT_InsertNode(&((*srcRootNode)->_right), srcNewNode); //현재 노드의 오른쪽 자식이 검은색 단말 노드일 때까지 탐색 수행
			}
		}
	}
}

/// <summary>
/// 대상 트리에 새 노드 삽입 발생 후 후처리
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <param name="srcNewNode">새 노드</param>
void RBT_PostProcAfterInsert(Node** srcRootNode, Node* srcNewNode)
{
	/***
		< 새 노드 삽입 발생 후 후처리 >

		! 레드 블랙 트리 정의인 루트 노드 및 단말 노드는 항상 검은색를 만족하기 위해,
		별도의 검은색 더미 노드를 단말 노드로 이용하며, 이에 따라 어떠한 노드의 왼쪽 자식 노드 혹은 오른쪽 자식 노드는 항상 NULL이 아님
		! 항상 삽입되는 새 노드는 빨간색
		! 새 노드 삽입 시 위반될 수 있는 정의와 수정 사항은,

		DEF1) 루트 노드는 항상 검은색
		: 삽입되는 새 노드는 항상 빨간색이므로 위반, 삽입되는 새 노드가 루트 노드일 시 검은색으로 변경

		DEF2) 빨간 노드의 한 단계 하위 자식 노드는 항상 검은색
		: 삽입되는 새 노드는 항상 빨간색이므로, 빨간 노드 다음에 빨간 노드가 삽입 될 경우 위반

		---

		< Determining Proc >

		1) 새 노드가 루트 노드이거나, 새 노드의 부모 노드가 검은색인 경우
		: do nothing

		2) 새 노드의 부모 노드가 새 노드의 부모의 부모의 왼쪽 자식 혹은 오른쪽 자식 여부에 따라, 새 노드의 부모의 형제 노드 (삼촌) 판별

		3) DEF2)의 위반 사항을 해결하기 위해, 새 노드의 부모의 형제 노드 (삼촌)의 색에 따라,

			3-1) 새 노드의 부모의 형제 노드 (삼촌)가 검은색일 경우
			: goto Restructuring Proc

			3-2) 새 노드의 부모의 형제 노드 (삼촌)가 빨간색일 경우
			: goto Recoloring Proc

		ex) DEF2)를 만족하기 위해,
		삽입되는 새 노드의 부모가 빨간색이면 삽입되는 새 노드를 단순히 검은색으로 변경만 할 경우, DEF4)를 위반 할 수 있음
		(즉, 이진 탐색 트리가 한 쪽 방향으로 기형적으로 성장하는 것처럼 균형이 꺠질 수 있음)

		---
		
		< Restructuring Proc >

		! 새 노드의 부모의 형제 노드 (삼촌)가 검은색인 상황에서 트리 재구성 (회전 발생)
		
		1) ND : 새 노드의 새 노드의 부모에 대한 자식 방향 (L : 왼쪽 자식, R : 오른쪽 자식)
		2) NPD : 새 노드의 부모 노드의 새 노드의 부모의 부모에 대한 자식 방향 (L : 왼쪽 자식, R : 오른쪽 자식)
		3) P : 이에 따른 수행 작업
		
		TODO :

		ND | NPD | P
		L	 L		1) 새 노드의 부모의 부모 노드를 기준으로 오른쪽 회전
					: 새 노드 < 새 노드의 부모 노드 < 새 노드의 부모의 부모 노드

					2) 새 노드의 부모 노드와 새 노드의 부모의 부모 노드 간의 색상 SWAP
					: 이에 따라, DEF2)를 만족

		L	 R		새 노드의 부모의 부모 노드 < 새 노드 < 새 노드의 부모 노드

		R	 L		새 노드의 부모 노드 < 새 노드 < 새 노드의 부모의 부모 노드

		R	 R		새 노드의 부모의 부모 노드 < 새 노드의 부모 노드 < 새 노드

		ex) 
		- LR : 단순히 새 노드의 부모의 부모 노드를 기준으로 오른쪽으로 회전만 수행하지 않고,
		왜 새 노드의 부모 노드를 기준으로 왼쪽 회전을 먼저 수행하고,
		이어서, 새 노드의 부모의 부모 노드를 기준으로 오른쪽 회전을 수행하여야 하는가?
		
		- RL : 단순히 새 노드의 부모의 부모 노드를 기준으로 왼쪽으로 회전만 수행하지 않고,
		왜 새 노드의 부모 노드를 기준으로 오른쪽 회전을 먼저 수행하고,
		이어서, 새 노드의 부모의 부모 노드를 기준으로 왼쪽 회전을 수행하여야 하는가?

		LR에 대해, 다음의 트리 가정,

							4 (Root, 오른쪽 하위 트리 생략)
					3 (오른쪽 하위 트리 생략)
			1 (Red)
				2 (newNode, Red)

		1) 단순히, 새 노드의 부모의 부모 노드 3를 기준으로 오른쪽으로 회전만 수행 할 경우

							4 (Root)
					1 (Red)
							3
						2 (newNode, Red)

		=> 균형이 꺠질 수 있음

		2) 새 노드의 부모 노드 1를 기준으로 왼쪽 회전을 먼저 수행하고,
		이어서, 새 노드의 부모의 부모 노드 3를 기준으로 오른쪽 회전을 수행 할 경우

			2-1) 새 노드의 부모 노드 1를 기준으로 왼쪽 회전을 먼저 수행

							4 (Root)
						3 (oldParent)
					2 (newNode, Red)
				1 (Red)

			2-2) 이어서, 새 노드의 부모의 부모 노드 3를 기준으로 오른쪽 회전을 수행
			
							4 (Root)
						2 (newNode, Red)
				1 (Red)		3 (oldParent)

		---

		< Recoloring Proc >

		! 새 노드의 부모의 형제 노드 (삼촌)가 빨간색인 상황에서 색 변경

		1) 새 노드의 부모와 새 노드의 부모의 형제 노드 (삼촌)를 검은색으로 변경

		2) 새 노드의 부모의 부모 노드의 상태에 따라,

			2-1) 새 노드의 부모의 부모 노드 == 루트 노드
			: do nothing (새 노드의 부모의 부모 노드는 검은색)

			2-2) 새 노드의 부모의 부모 노드 != 루트 노드

				2-2-1) 새 노드의 부모의 부모 노드를 빨간색으로 변경

				2-2-2) 새 노드의 부모의 부모 노드를 삽입이 발생하는 새 노드로 간주
				: goto Determining Proc (새 노드의 부모의 부모 노드와 새 노드의 부모의 부모의 부모 노드 간에 DEF2)의 위반 사항이 존재하는지 다시 판별)
	***/

	Node* uncleNode = NULL; //새 노드의 부모의 형제 노드 (삼촌)
	
	if (srcNewNode == (*srcRootNode)) //삽입되는 새 노드가 루트 노드일 시
	{
		srcNewNode->_color = COLOR::BLACK; //DEF1) 루트 노드는 항상 검은색
		return;
	}

DETERMINING_PROC: //판별 처리 루틴
	if (srcNewNode == (*srcRootNode) || srcNewNode->_parent->_color == COLOR::BLACK) //새 노드가 루트 노드이거나, 새 노드의 부모 노드가 검은색인 경우
		return;

	uncleNode = (srcNewNode->_parent->_parent->_left == srcNewNode->_parent) ?
		srcNewNode->_parent->_parent->_right : srcNewNode->_parent->_parent->_left;

	if (uncleNode == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Invalid Red-Black Tree"));

	switch (uncleNode->_color) //새 노드의 부모의 형제 노드 (삼촌)의 색에 따라,
	{
	case COLOR::BLACK:
		goto RESTRUCTURING_PROC;

	case COLOR::RED:
		goto RECOLORING_PROC;
	}

RESTRUCTURING_PROC: //트리 재구성 처리 루틴




RECOLORING_PROC: //색 변경 처리 루틴
	srcNewNode->_parent->_color = uncleNode->_color = COLOR::BLACK; //새 노드의 부모와 새 노드의 부모의 형제 노드(삼촌)를 검은색으로 변경

	if (srcNewNode->_parent->_parent == (*srcRootNode)) //새 노드의 부모의 부모 노드가 루트 노드인 경우
		return;

	//새 노드의 부모의 부모 노드가 루트 노드가 아닌 경우, 새 노드의 부모의 부모 노드를 빨간색으로 변경
	srcNewNode->_parent->_parent->_color = COLOR::RED;

	//새 노드의 부모의 부모 노드를 삽입이 발생하는 새 노드로 간주
	srcNewNode = srcNewNode->_parent->_parent;

	//새 노드의 부모의 부모 노드와 새 노드의 부모의 부모의 부모 노드 간에 DEF2)의 위반 사항이 존재하는지 다시 판별
	goto DETERMINING_PROC;
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
		별도의 검은색 더미 노드를 단말 노드로 이용하며, 이에 따라 어떠한 노드의 왼쪽 자식 노드 혹은 오른쪽 자식 노드는 항상 NULL이 아님

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
	case ROTATE_DIRECTION::RIGHT:
		moveTargetNode = targetParentNode->_left;
		moveTargetChildNode = moveTargetNode->_right;
		moveTargetParentToChildConnection = &(targetParentNode->_left);
		moveTargetToChildConnection = &(moveTargetNode->_right);
		break;

	case ROTATE_DIRECTION::LEFT:
		moveTargetNode = targetParentNode->_right;
		moveTargetChildNode = moveTargetNode->_left;
		moveTargetParentToChildConnection = &(targetParentNode->_right);
		moveTargetToChildConnection = &(moveTargetNode->_left);
		break;
	}

	if (moveTargetNode == NULL || moveTargetChildNode == NULL ||
		moveTargetParentToChildConnection == NULL || moveTargetToChildConnection == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Invalid Red-Black Tree"));

	if (moveTargetNode == dummyBlackTerminalNode)
		throw std::logic_error(std::string(__func__) + std::string(" : Wrong Tree Rotation cond"));

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