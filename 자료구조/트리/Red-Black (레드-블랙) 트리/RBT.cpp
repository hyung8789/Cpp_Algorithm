﻿#include "RBT_Core.h"

/// <summary>
/// 새로운 노드 생성 및 생성 된 노드 반환
/// </summary>
/// <param name="srcData">노드의 데이터</param>
/// <param name="color">노드의 색</param>
/// <returns>생성 된 노드</returns>
NODE* RBT_CreateNode(DATA_TYPE srcData)
{
	NODE* retVal = (NODE*)malloc(sizeof(NODE));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_data = srcData;
	retVal->_color = COLOR::RED; //항상 새 노드는 빨간색
	retVal->_parent = retVal->_left = retVal->_right = NULL;

	return retVal;
}

/// <summary>
/// 대상 노드에 할당 된 메모리 해제
/// </summary>
/// <param name="srcNode">대상 노드</param>
void RBT_DeallocateNode(NODE** srcNode)
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
void RBT_DeallocateTree(NODE** srcRootNode)
{
	if ((*srcRootNode) != NULL) //후위 순회로 왼쪽 끝 노드부터 해제 
	{
		if ((*srcRootNode)->_left != NULL)
			RBT_DeallocateTree(&((*srcRootNode)->_left));

		if ((*srcRootNode)->_right != NULL)
			RBT_DeallocateTree(&((*srcRootNode)->_right));

		RBT_DeallocateNode(srcRootNode);
		(*srcRootNode) = NULL;
	}
}

/// <summary>
/// 순회 방법에 따른 대상 트리 출력
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <param name="traversalMethod">순회 방법</param>
void RBT_DispOrderedTree(NODE* srcRootNode, TRAVERSAL_METHOD traversalMethod)
{
	if (srcRootNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	switch (traversalMethod)
	{
	case TRAVERSAL_METHOD::PREORDER:
		std::cout << srcRootNode->_data << " ";

		if (srcRootNode->_left != NULL)
			RBT_DispOrderedTree(srcRootNode->_left, traversalMethod);

		if (srcRootNode->_right != NULL)
			RBT_DispOrderedTree(srcRootNode->_right, traversalMethod);
		break;

	case TRAVERSAL_METHOD::INORDER:
		if (srcRootNode->_left != NULL)
			RBT_DispOrderedTree(srcRootNode->_left, traversalMethod);

		std::cout << srcRootNode->_data << " ";

		if (srcRootNode->_right != NULL)
			RBT_DispOrderedTree(srcRootNode->_right, traversalMethod);
		break;

	case TRAVERSAL_METHOD::POSTORDER:
		if (srcRootNode->_left != NULL)
			RBT_DispOrderedTree(srcRootNode->_left, traversalMethod);

		if (srcRootNode->_right != NULL)
			RBT_DispOrderedTree(srcRootNode->_right, traversalMethod);

		std::cout << srcRootNode->_data << " ";
		break;
	}
}

/// <summary>
/// 대상 트리에 새 노드 삽입
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <param name="srcNewNode">새 노드</param>
void RBT_InsertNode(NODE** srcRootNode, NODE* srcNewNode)
{
	if (dummyBlackTerminalNode == NULL) //초기 생성 혹은 트리 할당 해제 후 재 생성 시
	{
		dummyBlackTerminalNode = RBT_CreateNode(DUMMY_BLACK_TERMINAL_NODE_DATA);
		dummyBlackTerminalNode->_color = COLOR::BLACK;
	}

	if (dummyBlackTerminalNode->_parent != NULL) //검은색 더미 단말 노드에서 부모로의 연결은 허용하지 않음 
		throw std::logic_error(std::string(__func__) + std::string(" : Not allowed parent connection from dummy"));

	RBT_InsertNodeHelper(srcRootNode, srcNewNode);

	/***
		< 새 노드 삽입 발생에 따른 후속 처리 >

		! 레드 블랙 트리 정의인 루트 노드 및 단말 노드는 항상 검은색를 만족하기 위해,
		별도의 검은색 더미 노드를 단말 노드로 이용하며, 이에 따라 어떠한 노드의 왼쪽 자식 노드 혹은 오른쪽 자식 노드는 항상 NULL이 아님

		! 항상 삽입되는 새 노드는 빨간색
		
		1) 새 노드 삽입 시 위반될 수 있는 정의와 수정 사항은,

			DEF1) 루트 노드는 항상 검은색
			: 삽입되는 새 노드는 항상 빨간색이므로 위반, 삽입되는 새 노드가 루트 노드일 시 검은색으로 변경

			DEF2) 빨간 노드의 한 단계 하위 자식 노드는 항상 검은색
			: 삽입되는 새 노드는 항상 빨간색이므로, 빨간 노드 다음에 빨간 노드가 삽입 될 경우 위반,
			새 노드의 부모의 형제 노드 (삼촌)의 색에 따라 처리
	***/

	NODE* uncleNode = NULL; //새 노드의 부모의 형제 노드 (삼촌)
	NODE* grandParentNode = NULL; //새 노드의 부모의 부모 노드 (할아버지)
	COLOR tmpColor;

DETERMINING_PROC: //다음 작업 판별 처리 루틴
	/***
		< Determining Proc >

		1) 새 노드가 루트 노드이거나, 새 노드의 부모 노드가 검은색인 경우
		: do nothing

		2) 새 노드의 부모 노드가 새 노드의 부모의 부모의 왼쪽 자식 혹은 오른쪽 자식 여부에 따라, 
		새 노드의 부모의 형제 노드 (삼촌) 판별

		3) DEF2)의 위반 사항을 해결하기 위해, 새 노드의 부모의 형제 노드 (삼촌)의 색에 따라,

			3-1) 새 노드의 부모의 형제 노드 (삼촌)가 검은색일 경우
			: goto Restructuring Proc

			3-2) 새 노드의 부모의 형제 노드 (삼촌)가 빨간색일 경우
			: goto Recoloring Proc

		ex) 
		DEF2)를 만족하기 위해, 삽입되는 새 노드의 부모가 빨간색이면 삽입되는 새 노드를 단순히 검은색으로 변경만 할 경우, 
		DEF4)를 위반 할 수 있음 (즉, 이진 탐색 트리가 한 쪽 방향으로 기형적으로 성장하는 것처럼 균형이 꺠질 수 있음)
	***/

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
	/***
		< Restructuring Proc >

		! 새 노드의 부모의 형제 노드 (삼촌)가 검은색인 상황에서 트리 재구성 (회전 발생)

		! DEF2)에 따라, 트리 재구성이 발생되기 시작 한 시점의 새 노드의 부모의 부모 노드는 항상 검은색
		(새 노드와 새 노드의 부모 노드 간의 연속 된 빨간 노드 발생애 대한 처리 중이므로)

		1) NPD : 새 노드의 부모 노드에 대한 새 노드의 부모의 부모로부터의 자식 방향 (L : 왼쪽 자식, R : 오른쪽 자식)
		2) ND : 새 노드에 대한 새 노드의 부모로부터의 자식 방향 (L : 왼쪽 자식, R : 오른쪽 자식)
		3) P : 이에 따른 수행 작업

		NPD	|	ND	|	P
		L		L		1) 새 노드의 부모의 부모 노드를 기준으로 오른쪽 회전
						: 새 노드 < 새 노드의 부모 노드 < 새 노드의 부모의 부모 노드

		L		R		1) 새 노드의 부모 노드를 기준으로 왼쪽 회전
						: 새 노드의 부모의 부모 노드 < 새 노드 < 새 노드의 부모 노드

						2) 새 노드의 부모의 부모 노드 (회전 발생 전)를 기준으로 오른쪽 회전

		R		L		1) 새 노드의 부모 노드를 기준으로 오른쪽 회전
						: 새 노드의 부모 노드 < 새 노드 < 새 노드의 부모의 부모 노드

						2) 새 노드의 부모의 부모 노드 (회전 발생 전)를 기준으로 왼쪽 회전

		R		R		1) 새 노드의 부모의 부모 노드를 기준으로 왼쪽 회전
						: 새 노드의 부모의 부모 노드 < 새 노드의 부모 노드 < 새 노드

		( 공통 처리 )

		1) 새 노드의 부모 노드와 새 노드의 부모의 부모 노드 (회전 발생 전) 간의 색상 SWAP
		: 이에 따라, 새 노드와 새 노드의 부모 노드 간에 DEF2)의 위반사항을 수정하였으며,
		새 노드의 부모는 검은색이고, 새 노드의 부모의 부모 노드 (회전 발생 전)은 빨간색이므로, DEF3)도 위반하지 않음

		2) 새 노드를 기준으로 다시 DEF2)의 위반 사항이 존재하는지 판별
		: goto Determining Proc

		ex)
		- LR : 단순히 새 노드의 부모의 부모 노드를 기준으로 오른쪽으로 회전만 수행하지 않고,
		왜 새 노드의 부모 노드를 기준으로 왼쪽 회전을 먼저 수행하고,
		이어서, 새 노드의 부모의 부모 노드 (회전 발생 전)를 기준으로 오른쪽 회전을 수행하여야 하는가?

		- RL : 단순히 새 노드의 부모의 부모 노드를 기준으로 왼쪽으로 회전만 수행하지 않고,
		왜 새 노드의 부모 노드를 기준으로 오른쪽 회전을 먼저 수행하고,
		이어서, 새 노드의 부모의 부모 노드 (회전 발생 전)를 기준으로 왼쪽 회전을 수행하여야 하는가?

		=> 트리의 높이 균형을 위해 2회의 회전 수행
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

		2) 새 노드의 부모 노드 1를 기준으로 왼쪽 회전을 먼저 수행하고,
		이어서, 새 노드의 부모의 부모 노드 3를 기준으로 오른쪽 회전을 수행 할 경우

			2-1) 새 노드의 부모 노드 1를 기준으로 왼쪽 회전을 먼저 수행

							4 (Root)
						3 (회전 발생 전)
					2 (newNode, Red)
				1 (Red)

			2-2) 이어서, 새 노드의 부모의 부모 노드 (회전 발생 전) 3를 기준으로 오른쪽 회전을 수행

							4 (Root)
						2 (newNode, Red)
				1 (Red)		3 (회전 발생 전)
	***/

	grandParentNode = srcNewNode->_parent->_parent; //회전 발생 전 새 노드의 부모의 부모 노드 (할아버지)

	if (srcNewNode->_parent->_parent->_left == srcNewNode->_parent &&
		srcNewNode->_parent->_left == srcNewNode) //LL
	{
		RBT_RotateTree(srcRootNode, srcNewNode->_parent->_parent, ROTATE_DIRECTION::RIGHT);
	}
	else if (srcNewNode->_parent->_parent->_left == srcNewNode->_parent &&
		srcNewNode->_parent->_right == srcNewNode) //LR
	{
		RBT_RotateTree(srcRootNode, srcNewNode->_parent, ROTATE_DIRECTION::LEFT);
		RBT_RotateTree(srcRootNode, grandParentNode, ROTATE_DIRECTION::RIGHT);
	}
	else if (srcNewNode->_parent->_parent->_right == srcNewNode->_parent &&
		srcNewNode->_parent->_left == srcNewNode) //RL
	{
		RBT_RotateTree(srcRootNode, srcNewNode->_parent, ROTATE_DIRECTION::RIGHT);
		RBT_RotateTree(srcRootNode, grandParentNode, ROTATE_DIRECTION::LEFT);
	}
	else //RR
	{
		RBT_RotateTree(srcRootNode, srcNewNode->_parent, ROTATE_DIRECTION::LEFT);
	}

	//새 노드의 부모 노드와 새 노드의 부모의 부모 노드 (회전 발생 전) 간의 색상 SWAP
	SWAP(srcNewNode->_color, grandParentNode->_color, tmpColor);

	//새 노드를 기준으로 다시 DEF2)의 위반 사항이 존재하는지 판별
	goto DETERMINING_PROC;

RECOLORING_PROC: //색 변경 처리 루틴
	/***
		< Recoloring Proc >

		! 새 노드의 부모의 형제 노드 (삼촌)가 빨간색인 상황에서 색 변경

		1) 새 노드의 부모와 새 노드의 부모의 형제 노드 (삼촌)를 검은색으로 변경

		2) 새 노드의 부모의 부모 노드의 루트 노드 여부에 따라,

			2-1) 새 노드의 부모의 부모 노드 == 루트 노드
			: do nothing (새 노드의 부모의 부모 노드는 검은색)

			2-2) 새 노드의 부모의 부모 노드 != 루트 노드

				2-2-1) 새 노드의 부모의 부모 노드를 빨간색으로 변경

				2-2-2) 새 노드의 부모의 부모 노드를 삽입이 발생하는 새 노드로 간주
				: goto Determining Proc (새 노드의 부모의 부모 노드와 새 노드의 부모의 부모의 부모 노드 간에 DEF2)의 위반 사항이 존재하는지 다시 판별)
	***/

	srcNewNode->_parent->_color = uncleNode->_color = COLOR::BLACK; //새 노드의 부모와 새 노드의 부모의 형제 노드(삼촌)를 검은색으로 변경

	if (srcNewNode->_parent->_parent == (*srcRootNode)) //새 노드의 부모의 부모 노드가 루트 노드인 경우
		return;

	//새 노드의 부모의 부모 노드가 루트 노드가 아닌 경우, 새 노드의 부모의 부모 노드를 빨간색으로 변경
	srcNewNode->_parent->_parent->_color = COLOR::RED;

	//새 노드의 부모의 부모 노드를 삽입이 발생하는 새 노드로 간주
	srcNewNode = srcNewNode->_parent->_parent;

	//새 노드의 부모의 부모 노드와 새 노드의 부모의 부모의 부모 노드 간에 DEF2)의 위반 사항이 존재하는지 판별
	goto DETERMINING_PROC;
}

/// <summary>
/// 대상 트리에 삭제하고자 하는 대상 데이터가 포함 된 노드 삭제
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <param name="targetData">삭제하고자 하는 대상 데이터</param>
/// <param name="deallocateAfterRemove">삭제 대상 노드에 대한 메모리 해제 수행 여부</param>
/// <summary>
void RBT_RemoveNode(NODE** srcRootNode, const DATA_TYPE& targetData, bool deallocateAfterRemove)
{
	/***
		< 삭제하고자 하는 대상 데이터가 포함 된 노드 (이하, 삭제 대상 노드) 삭제를 위한 삭제 대상 노드의 자리를 대체 할 노드 (이하, 이동 대상 노드) 선택 >

		! 검은색 더미 단말 노드는 이동 대상 노드로 선택되지 않음

		1) 삭제 대상 노드의 왼쪽 혹은 오른쪽 자식의 검은색 더미 단말 노드 여부에 따라,

			1-1) 삭제 대상 노드의 왼쪽 자식 노드 == 검은색 더미 단말 노드 && 삭제 대상 노드의 오른쪽 자식 노드 != 검은색 더미 단말 노드
			: 삭제 대상 노드의 오른쪽 하위 트리에서 최솟값을 이동 대상 노드로 선택

			1-2) 삭제 대상 노드의 왼쪽 자식 노드 != 검은색 더미 단말 노드 && 삭제 대상 노드의 오른쪽 자식 노드 == 검은색 더미 단말 노드
			: 삭제 대상 노드의 왼쪽 하위 트리에서 최대값을 이동 대상 노드로 선택

			1-3) 삭제 대상 노드의 왼쪽 자식 노드 != 검은색 더미 단말 노드 && 삭제 대상 노드의 오른쪽 자식 노드 != 검은색 더미 단말 노드
			: 삭제 대상 노드의 왼쪽 하위 트리에서 최대값을 이동 대상 노드로 선택 (임의)

			1-4) 삭제 대상 노드의 왼쪽 자식 노드 == 검은색 더미 단말 노드 && 삭제 대상 노드의 오른쪽 자식 노드 == 검은색 더미 단말 노드
			: do nothing (삭제 대상 노드가 단말 노드일 경우 발생)

		---

		< 삭제 대상 노드 및 인접 노드의 색에 따른 수행 작업 판별 >
		
		! 루트 노드에 대한 삭제가 발생 할 경우, 별도의 처리 요구

		1) RPC : 삭제 대상 노드 부모 노드의 색 (RED, BLACK)
		2) RC : 삭제 대상 노드의 색 (RED, BLACK)
		3) MC : 이동 대상 노드의 색 (RED, BLACK)
		4) P : 이에 따른 수행 작업

		RPC	|	RC	|	MC	|	P
		RED		RED		-		throw exception : RC 및 RPC 간 DEF2) 위반

		RED		BLACK	RED		1) 삭제 대상 노드를 삭제 및 이동 대상 노드를 이동 후 DEF2) 위반

								2) 이동 대상 노드를 검은색으로 변경
								: 삭제 대상 노드가 검은색이었으므로, 이동 대상 노드가 검은색으로 변경되어도 DEF4) 를 위반하지 않음
		
		RED		BLACK	BLACK	1) 삭제 대상 노드를 삭제 및 이동 대상 노드를 이동 후 DEF4) 위반

								2) 이동 대상 노드를 임시로 이중 검은색 노드로 가정
								: DEF4) 위반에서 DEF1) 위반으로 변경

								3) 이중 검은색 노드의 DEF1) 위반에 대한 처리를 위해 별도의 처리 루틴으로 이동

		BLACK	RED		RED		throw exception : RC 및 MC 간 DEF2) 위반

		BLACK	RED		BLACK	1) 삭제 대상 노드를 삭제 및 이동 대상 노드를 이동 후 DEF4) 위반
								: 삭제 대상 노드가 검은색 더미 단말 노드를 제외 한 두 개의 자식 노드를 갖고 있을 경우 발생
								(검은색 더미 단말 노드는 이동 대상 노드로 선택되지 않음)


		BLACK	BLACK	RED
		BLACK	BLACK	BLACK

		---

		< 이중 검은색 노드에 대해 DEF1) 위반 처리를 위한 수행 작업 판별 >

	
		이중 검은색 노드의 반대쪽 노드 (이하, 이중 검은색 노드의 형제 노드)의 색
		이중 검은색 노드의 형제 노드의 왼쪽 자식 노드의 색
		이중 검은색 노드의 형제 노드의 오른쪽 자식 노드의 색


		TODO : 
		이하, 수정, 삭제
		

		---
		2) 삭제 대상 노드의 색에 따라,
			
			2-1) 삭제 대상 노드의 색 == 빨간색
			: 위반될 수 있는 정의와 수정 사항은,
				
				DEF4) 루트 노드에서 검은색 더미 단말 노드 간의 각 연결 과정에 존재하는 검은 노드의 수는 모두 동일
				: 이동 대상 노드가 검은 노드이면서 삭제 대상 노드를 삭제 및 이동 대상 노드를 이동 후 발생하며, 
				트리의 높이 균형이 깨졌음을 의미, 이동 대상 노드의 부모 노드로부터 이동 대상 노드로의 방향에 따른 트리 회전 수행

				2-1-1) 이동 대상 노드의 부모 노드의 왼쪽 자식 == 이동 대상 노드
				: DEF4)의 위반사항을 수정하기 위해 이동 대상 노드 (moveTarget)의 부모 노드를 기준으로 오른쪽으로 트리 회전
				(이동 대상 노드의 왼쪽 자식 노드 < 이동 대상 노드 (moveTarget) < 이동 대상 노드의 부모 노드)

				2-1-2) 이동 대상 노드의 부모 노드의 오른쪽 자식 == 이동 대상 노드


			2-2) 삭제 대상 노드의 색 == 검은색

			DEF4) 루트 노드에서 검은색 더미 단말 노드 간의 각 연결 과정에 존재하는 검은 노드의 수는 모두 동일
			: 위반될 수 있는 정의와 수정 사항은,

		
		삭제 대상 노드의 부모 노드 및 이동 대상 노드가 모두 빨간색일 경우
		: DEF2)를 위반하므로, 이동 대상 노드를 검은색으로 변경

		삭제 대상 노드의 부모 노드가 빨간색이고, 이동 대상 노드가 검은색일 경우
		: DEF4)를 위반, 임시로 이동 대상 노드를 이중 검은 노드로 간주



		---
		
		ex)
		- R : Red
		- B : Black
		- 검은색 더미 단말 노드 생략

		1) 빨간색 노드 (removeTarget)를 삭제 할 경우

								B (root)
					R (removeTarget)		B
				B		B (moveTarget)	R		R
		
			=> 삭제 및 이동 대상 노드 (moveTarget) 이동
		
								B (root)
					B (moveTarget)		B
				B					R		R

			=> DEF4)의 위반사항을 수정하기 위해 이동 대상 노드 (moveTarget)의 부모 노드 (root)를 기준으로 오른쪽으로 트리 회전
			: 이동 대상 노드 (moveTarget)의 왼쪽 자식 노드 < 이동 대상 노드 (moveTarget) < root

							B (moveTarget, newRoot)
					B 				B (oldRoot)
								R		R

		2) 검은색 노드 (removeTarget)를 삭제 할 경우

							B (root)
					R					B  (removeTarget)
				B		B 		R (moveTarget)		R

			=> 삭제 및 이동 대상 노드 (moveTarget) 이동

								B (root)
					R					R (moveTarget)
				B		B 					R

			=> 
			: root < 이동 대상 노드 (moveTarget) < 이동 대상 노드 (moveTarget)의 오른쪽 자식 노드

	***/

	if ((*srcRootNode) == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	NODE* removeTargetNode = RBT_SearchNode((*srcRootNode), targetData); //삭제 대상 노드
	NODE** removeTargetParentToChildConnection = 
		(removeTargetNode->_parent->_left == removeTargetNode) ? 
		&(removeTargetNode->_parent->_left) : &(removeTargetNode->_parent->_right); //삭제 대상 노드의 부모 노드에서 삭제 대상 노드로의 연결
	NODE* moveTargetNode = NULL; //이동 대상 노드

	if (removeTargetNode->_left == dummyBlackTerminalNode && removeTargetNode->_right != dummyBlackTerminalNode)
	{
		//삭제 대상 노드의 오른쪽 하위 트리에서 최솟값을 이동 대상 노드로 선택
		moveTargetNode = RBT_SearchMinNode(removeTargetNode->_right);
	}
	else if ((removeTargetNode->_left != dummyBlackTerminalNode && removeTargetNode->_right == dummyBlackTerminalNode) ||
		(removeTargetNode->_left != dummyBlackTerminalNode && removeTargetNode->_right != dummyBlackTerminalNode))
	{
		//삭제 대상 노드의 왼쪽 하위 트리에서 최대값을 이동 대상 노드로 선택
		moveTargetNode = RBT_SearchMaxNode(removeTargetNode->_left);
	}
	else
	{
		//do nothing (삭제 대상 노드가 단말 노드일 경우 발생)
	}



END_PROC:
	if (deallocateAfterRemove)
		RBT_DeallocateNode(&removeTargetNode);
}

/// <summary>
/// 대상 트리에 찾고자 하는 대상 데이터가 포함 된 노드 반환
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <param name="targetData">찾고자 하는 대상 데이터</param>
/// <returns>찾고자 하는 대상 데이터가 포함 된 노드</returns>
NODE* RBT_SearchNode(NODE* srcRootNode, const DATA_TYPE& targetData)
{
	if (srcRootNode == NULL || srcRootNode == dummyBlackTerminalNode)
		throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

	if (srcRootNode->_data == targetData) //현재 노드가 찾고자 하는 대상 데이터와 일치할 경우
		return srcRootNode;
	else if (srcRootNode->_data > targetData) //현재 노드 > 찾고자 하는 대상 데이터인 경우
		return RBT_SearchNode(srcRootNode->_left, targetData); //현재 노드 기준 왼쪽으로 탐색 수행
	else //현재 노드 < 찾고자 하는 대상 데이터인 경우
		return RBT_SearchNode(srcRootNode->_right, targetData); //현재 노드 기준 오른쪽으로 탐색 수행
}

/// <summary>
/// 대상 트리의 최대값인 데이터가 포함 된 노드 반환
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <returns>최소값인 데이터가 포함 된 노드</returns>
NODE* RBT_SearchMaxNode(NODE* srcRootNode)
{
	if (srcRootNode == NULL)
		throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

	if (srcRootNode->_right != NULL && srcRootNode->_right != dummyBlackTerminalNode) //현재 노드의 오른쪽 하위 트리가 존재하며, 검은색 더미 단말 노드가 아닐 경우
		return RBT_SearchMaxNode(srcRootNode->_right);
	else //현재 노드의 오른쪽 하위 트리가 존재하지 않거나, 검은색 더미 단말 노드일 경우
		return srcRootNode;
}

/// <summary>
/// 대상 트리의 최소값인 데이터가 포함 된 노드 반환
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <returns>최소값인 데이터가 포함 된 노드</returns>
NODE* RBT_SearchMinNode(NODE* srcRootNode)
{
	if (srcRootNode == NULL)
		throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

	if (srcRootNode->_left != NULL && srcRootNode->_left != dummyBlackTerminalNode) //현재 노드의 왼쪽 하위 트리가 존재하며, 검은색 더미 단말 노드가 아닐 경우
		return RBT_SearchMinNode(srcRootNode->_left);
	else //현재 노드의 왼쪽 하위 트리가 존재하지 않거나, 검은색 더미 단말 노드일 경우
		return srcRootNode;
}

/// <summary>
/// 대상 트리에 직접적인 새 노드 삽입 수행
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <param name="srcNewNode">새 노드</param>
void RBT_InsertNodeHelper(NODE** srcRootNode, NODE* srcNewNode)
{
	if (srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if ((*srcRootNode) == NULL) //초기 루트가 존재하지 않을 경우
	{
		(*srcRootNode) = srcNewNode;
		srcNewNode->_color = COLOR::BLACK; //DEF1) 루트 노드는 항상 검은색
		srcNewNode->_left = srcNewNode->_right = dummyBlackTerminalNode;
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
				srcNewNode->_left = srcNewNode->_right = dummyBlackTerminalNode;
			}
			else
			{
				//현재 노드의 왼쪽 자식이 검은색 단말 노드일 때까지 탐색 수행
				RBT_InsertNode(&((*srcRootNode)->_left), srcNewNode);
			}
		}
		else //현재 노드의 데이터 < 삽입하고자 하는 노드의 데이터인 경우
		{
			if ((*srcRootNode)->_right == dummyBlackTerminalNode) //현재 노드의 오른쪽 자식이 검은색 더미 단말 노드일 경우
			{
				//현재 노드의 오른쪽에 새 노드 삽입
				(*srcRootNode)->_right = srcNewNode;
				srcNewNode->_parent = (*srcRootNode);
				srcNewNode->_left = srcNewNode->_right = dummyBlackTerminalNode;
			}
			else
			{
				//현재 노드의 오른쪽 자식이 검은색 단말 노드일 때까지 탐색 수행
				RBT_InsertNode(&((*srcRootNode)->_right), srcNewNode);
			}
		}
	}
}

/// <summary>
/// 대상 트리의 대상 부모 노드와 대상 부모 노드의 자식 노드 간에 회전 방향에 따른 회전 수행
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <param name="srcTargetParentNode">대상 부모 노드</param>
/// <param name="rotateDirection">회전 방향</param>
void RBT_RotateTree(NODE** srcRootNode, NODE* srcTargetParentNode, ROTATE_DIRECTION rotateDirection)
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

	if ((*srcRootNode) == NULL || srcTargetParentNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	NODE* moveTargetNode = NULL; //회전을 위해 이동 될 노드
	NODE* moveTargetChildNode = NULL; //회전을 위해 이동 될 노드의 자식 노드

	NODE** moveTargetParentToChildConnection = NULL; //회전을 위해 이동 될 노드의 부모 노드에서 회전을 위해 이동 될 노드로의 연결
	NODE** moveTargetToChildConnection = NULL; //회전을 위해 이동 될 노드에서 회전을 위해 이동 될 노드의 자식 노드로의 연결

	switch (rotateDirection)
	{
	case ROTATE_DIRECTION::RIGHT:
		moveTargetNode = srcTargetParentNode->_left;
		moveTargetChildNode = moveTargetNode->_right;
		moveTargetParentToChildConnection = &(srcTargetParentNode->_left);
		moveTargetToChildConnection = &(moveTargetNode->_right);
		break;

	case ROTATE_DIRECTION::LEFT:
		moveTargetNode = srcTargetParentNode->_right;
		moveTargetChildNode = moveTargetNode->_left;
		moveTargetParentToChildConnection = &(srcTargetParentNode->_right);
		moveTargetToChildConnection = &(moveTargetNode->_left);
		break;
	}

	if (moveTargetNode == NULL || moveTargetChildNode == NULL ||
		moveTargetParentToChildConnection == NULL || moveTargetToChildConnection == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Invalid Red-Black Tree"));

	if (moveTargetNode == dummyBlackTerminalNode)
		throw std::logic_error(std::string(__func__) + std::string(" : Wrong Tree Rotation cond"));

	if (moveTargetChildNode != dummyBlackTerminalNode)
		moveTargetChildNode->_parent = srcTargetParentNode;

	if (srcTargetParentNode == (*srcRootNode)) //대상 부모 노드가 루트 노드일 경우
	{
		(*srcRootNode) = moveTargetNode;
	}
	else //대상 부모 노드가 루트 노드가 아닐 경우
	{
		//우회전 : 대상 부모 노드의 왼쪽 노드에서 부모를 대상 부모 노드의 부모 노드로 연결
		moveTargetNode->_parent = srcTargetParentNode->_parent;

		//우회전 : 대상 부모 노드의 부모에서 대상 부모 노드로의 연결을 대상 부모 노드의 왼쪽 자식 노드로 연결
		(srcTargetParentNode->_parent->_left == srcTargetParentNode) ?
			srcTargetParentNode->_parent->_left = moveTargetNode : srcTargetParentNode->_parent->_right = moveTargetNode;
	}

	//우회전 : 대상 부모 노드의 왼쪽 자식 노드에서 오른쪽 자식 노드로의 연결을 대상 부모 노드로 연결
	(*moveTargetToChildConnection) = srcTargetParentNode;

	//우회전 : 대상 부모 노드의 부모를 대상 부모 노드의 왼쪽 자식 노드로 연결
	srcTargetParentNode->_parent = moveTargetNode;

	//우회전 : 대상 부모 노드에서 왼쪽을 대상 부모 노드의 왼쪽 자식 노드의 오른쪽 자식 노드로 연결
	(*moveTargetParentToChildConnection) = moveTargetChildNode;
}