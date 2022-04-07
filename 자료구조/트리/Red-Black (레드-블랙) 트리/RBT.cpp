#include "RBT_Core.h"

NODE* dummyBlackTerminalNode = NULL; //검은색 더미 단말 노드

/// <summary>
/// 새로운 노드 생성 및 생성 된 노드 반환
/// </summary>
/// <param name="srcData">노드의 데이터</param>
/// <param name="color">노드의 색</param>
/// <returns>생성 된 노드</returns>
NODE* RBT_CreateNode(RBT_DATA_TYPE srcData)
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
		if ((*srcRootNode)->_left != dummyBlackTerminalNode)
			RBT_DeallocateTree(&((*srcRootNode)->_left));

		if ((*srcRootNode)->_right != dummyBlackTerminalNode)
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
/// <param name="rootNodeDepth">대상 트리의 최상위 루트 노드의 깊이</param>
void RBT_DispOrderedTree(NODE* srcRootNode, TRAVERSAL_METHOD traversalMethod, TREE_DEPTH_TYPE rootNodeDepth)
{
	if (srcRootNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	switch (traversalMethod)
	{
	case TRAVERSAL_METHOD::PREORDER:
		for (TREE_DEPTH_TYPE i = 0; i < rootNodeDepth; i++)
		{
			std::cout << "\t";
		}
		std::cout << "|- ";

#ifdef COLOR_VISUALIZATION
		CONSOLE_SCREEN_MANAGER::GetInstance().SetConsoleTextColor((const unsigned)(srcRootNode->_color));
#endif

		std::cout << srcRootNode->_data;

#ifdef COLOR_VISUALIZATION
		CONSOLE_SCREEN_MANAGER::GetInstance().UnsetConsoleTextColor();
#endif
		if (srcRootNode->_parent != NULL)
			std::cout << " (Parent : " << srcRootNode->_parent->_data << ")";

		if (rootNodeDepth == 0)
			std::cout << " (Root, ";
		else if (srcRootNode->_left == dummyBlackTerminalNode && srcRootNode->_right == dummyBlackTerminalNode)
			std::cout << "(Terminal, ";
		else
			std::cout << "(Branch, ";

		std::cout <<
			"Depth : " << rootNodeDepth <<
			", " << ((srcRootNode->_color == COLOR::RED) ? "RED" : "BLACK") << ")\n";

		if (srcRootNode->_left != dummyBlackTerminalNode)
			RBT_DispOrderedTree(srcRootNode->_left, traversalMethod, rootNodeDepth + 1);

		if (srcRootNode->_right != dummyBlackTerminalNode)
			RBT_DispOrderedTree(srcRootNode->_right, traversalMethod, rootNodeDepth + 1);
		break;

	case TRAVERSAL_METHOD::INORDER:
		if (srcRootNode->_left != dummyBlackTerminalNode)
			RBT_DispOrderedTree(srcRootNode->_left, traversalMethod, rootNodeDepth + 1);

		for (TREE_DEPTH_TYPE i = 0; i < rootNodeDepth; i++)
		{
			std::cout << "\t";
		}
		std::cout << "|- ";

#ifdef COLOR_VISUALIZATION
		CONSOLE_SCREEN_MANAGER::GetInstance().SetConsoleTextColor((const unsigned)(srcRootNode->_color));
#endif

		std::cout << srcRootNode->_data;

#ifdef COLOR_VISUALIZATION
		CONSOLE_SCREEN_MANAGER::GetInstance().UnsetConsoleTextColor();
#endif
		if (srcRootNode->_parent != NULL)
			std::cout << " (Parent : " << srcRootNode->_parent->_data << ")";

		if (rootNodeDepth == 0)
			std::cout << " (Root, ";
		else if (srcRootNode->_left == dummyBlackTerminalNode && srcRootNode->_right == dummyBlackTerminalNode)
			std::cout << "(Terminal, ";
		else
			std::cout << "(Branch, ";

		std::cout <<
			"Depth : " << rootNodeDepth <<
			", " << ((srcRootNode->_color == COLOR::RED) ? "RED" : "BLACK") << ")\n";

		if (srcRootNode->_right != dummyBlackTerminalNode)
			RBT_DispOrderedTree(srcRootNode->_right, traversalMethod, rootNodeDepth + 1);
		break;

	case TRAVERSAL_METHOD::POSTORDER:
		if (srcRootNode->_left != dummyBlackTerminalNode)
			RBT_DispOrderedTree(srcRootNode->_left, traversalMethod, rootNodeDepth + 1);

		if (srcRootNode->_right != dummyBlackTerminalNode)
			RBT_DispOrderedTree(srcRootNode->_right, traversalMethod, rootNodeDepth + 1);

		for (TREE_DEPTH_TYPE i = 0; i < rootNodeDepth; i++)
		{
			std::cout << "\t";
		}
		std::cout << "|- ";

#ifdef COLOR_VISUALIZATION
		CONSOLE_SCREEN_MANAGER::GetInstance().SetConsoleTextColor((const unsigned)(srcRootNode->_color));
#endif

		std::cout << srcRootNode->_data;

#ifdef COLOR_VISUALIZATION
		CONSOLE_SCREEN_MANAGER::GetInstance().UnsetConsoleTextColor();
#endif
		if (srcRootNode->_parent != NULL)
			std::cout << " (Parent : " << srcRootNode->_parent->_data << ")";

		if (rootNodeDepth == 0)
			std::cout << " (Root, ";
		else if (srcRootNode->_left == dummyBlackTerminalNode && srcRootNode->_right == dummyBlackTerminalNode)
			std::cout << "(Terminal, ";
		else
			std::cout << "(Branch, ";

		std::cout <<
			"Depth : " << rootNodeDepth <<
			", " << ((srcRootNode->_color == COLOR::RED) ? "RED" : "BLACK") << ")\n";
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

		1) 새 노드가 루트 노드일 경우
		: do nothing

		2) 삽입 된 새 노드와 새 노드의 부모 노드 간에 DEF2)를 위반하는지 판별

			2-1) 새 노드의 부모 노드의 색 == 검은색
			: do nothing

			2-2) 새 노드의 부모 노드의 색 == 빨간색
			: 삽입 된 새 노드와 새 노드의 부모 노드 간 DEF2) 위반

				2-1-1) 새 노드의 부모 노드가 새 노드의 부모의 부모의 왼쪽 자식 혹은 오른쪽 자식 여부에 따라,
				새 노드의 부모의 형제 노드 (삼촌) 판별

				2-1-2) DEF2)의 위반사항을 해결하기 위해, 새 노드의 부모의 형제 노드 (삼촌)의 색에 따라,

					2-1-2-1) 새 노드의 부모의 형제 노드 (삼촌)의 색 == 검은색
					: goto Restructuring Proc

					2-1-2-2) 새 노드의 부모의 형제 노드 (삼촌)의 색 == 빨간색
					: goto Recoloring Proc

		ex)
		DEF2)를 만족하기 위해, 삽입되는 새 노드의 부모가 빨간색이면 삽입되는 새 노드를 단순히 검은색으로 변경만 할 경우,
		DEF4)를 위반 할 수 있음 (즉, 이진 탐색 트리가 한 쪽 방향으로 기형적으로 성장하는 것처럼 균형이 꺠질 수 있음)
	***/

	if (srcNewNode == (*srcRootNode) || srcNewNode->_parent->_color == COLOR::BLACK)
		return;

	uncleNode = (srcNewNode->_parent->_parent->_left == srcNewNode->_parent) ?
		srcNewNode->_parent->_parent->_right : srcNewNode->_parent->_parent->_left;

	switch (uncleNode->_color)
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

		! DEF2)에 따라, 트리 재구성이 발생되기 시작 한 시점에
		새 노드와 새 노드의 부모 노드 간의 연속 된 빨간 노드 발생애 대한 처리 중이므로
		새 노드의 부모의 부모 노드는 항상 검은색

		1) NPD : 새 노드의 부모 노드에 대한 새 노드의 부모의 부모로부터의 자식 방향 (L : 왼쪽 자식, R : 오른쪽 자식)
		2) ND : 새 노드에 대한 새 노드의 부모로부터의 자식 방향 (L : 왼쪽 자식, R : 오른쪽 자식)
		3) P : 이에 따른 수행 작업

		NPD	|	ND	|	P
		L		L		1) 새 노드의 부모의 부모 노드 기준 우회전
						: 새 노드 < 새 노드의 부모 노드 < 새 노드의 부모의 부모 노드

		L		R		1) 새 노드의 부모 노드 기준 좌회전
						: 새 노드의 부모의 부모 노드 < 새 노드 < 새 노드의 부모 노드

						2) 새 노드의 부모의 부모 노드 (회전 발생 전) 기준 우회전

		R		L		1) 새 노드의 부모 노드 기준 우회전
						: 새 노드의 부모 노드 < 새 노드 < 새 노드의 부모의 부모 노드

						2) 새 노드의 부모의 부모 노드 (회전 발생 전) 기준 좌회전

		R		R		1) 새 노드의 부모의 부모 노드 기준 좌회전
						: 새 노드의 부모의 부모 노드 < 새 노드의 부모 노드 < 새 노드

		( 공통 처리 )

		1) 새 노드의 부모 노드와 새 노드의 부모의 부모 노드 (회전 발생 전) 간의 색 SWAP
		: 이에 따라, 새 노드와 새 노드의 부모 노드 간에 DEF2)의 위반사항을 수정하였으며,
		새 노드의 부모는 검은색이고, 새 노드의 부모의 부모 노드 (회전 발생 전)은 빨간색이므로, DEF3)도 위반하지 않음

		2) 새 노드를 기준으로 다시 DEF2)의 위반사항이 존재하는지 판별
		: goto Determining Proc

		ex)
		- LR : 단순히 새 노드의 부모의 부모 노드를 기준으로 우회전만 수행하지 않고,
		왜 새 노드의 부모 노드를 기준으로 좌회전을 먼저 수행하고,
		이어서, 새 노드의 부모의 부모 노드 (회전 발생 전)를 기준으로 우회전을 수행하여야 하는가?

		- RL : 단순히 새 노드의 부모의 부모 노드를 기준으로 좌회전만 수행하지 않고,
		왜 새 노드의 부모 노드를 기준으로 우회전을 먼저 수행하고,
		이어서, 새 노드의 부모의 부모 노드 (회전 발생 전)를 기준으로 좌회전을 수행하여야 하는가?

		=> 트리의 높이 균형을 위해 2회의 회전 수행

		LR에 대해, 다음의 트리 가정,

							4 (Root, 오른쪽 하위 트리 생략)
					3 (오른쪽 하위 트리 생략)
			1 (Red)
				2 (newNode, Red)

		1) 단순히, 새 노드의 부모의 부모 노드 3를 기준으로 우회전만 수행 할 경우

							4 (Root)
					1 (Red)
							3
						2 (newNode, Red)

		2) 새 노드의 부모 노드 1를 기준으로 좌회전을 먼저 수행하고,
		이어서, 새 노드의 부모의 부모 노드 3를 기준으로 우회전을 수행 할 경우

			2-1) 새 노드의 부모 노드 1를 기준으로 좌회전을 먼저 수행

							4 (Root)
						3 (회전 발생 전)
					2 (newNode, Red)
				1 (Red)

			2-2) 이어서, 새 노드의 부모의 부모 노드 (회전 발생 전) 3를 기준으로 우회전을 수행

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
		RBT_RotateTree(srcRootNode, srcNewNode->_parent->_parent, ROTATE_DIRECTION::LEFT);
	}

	//새 노드의 부모 노드와 새 노드의 부모의 부모 노드 (회전 발생 전) 간의 색 SWAP
	SWAP(srcNewNode->_color, grandParentNode->_color, tmpColor);

	//새 노드를 기준으로 다시 DEF2)의 위반사항이 존재하는지 판별
	goto DETERMINING_PROC;

RECOLORING_PROC: //색 변경 처리 루틴
	/***
		< Recoloring Proc >

		! 새 노드의 부모의 형제 노드 (삼촌)가 빨간색인 상황에서 색 변경

		1) 새 노드의 부모와 새 노드의 부모의 형제 노드 (삼촌)를 검은색으로 변경
		: 이 시점에, 새 노드의 부모의 부모 노드를 기준으로 양 측 경로 간 전체 검은 노드의 수는 변동없으므로,
		DEF4)를 위반하지 않음

		2) 새 노드의 부모의 부모 노드의 루트 노드 여부에 따라,

			2-1) 새 노드의 부모의 부모 노드 == 루트 노드
			: do nothing (새 노드의 부모의 부모 노드는 검은색이며, DEF2)의 위반사항이 모두 해결되었으므로, 처리 중지)

			2-2) 새 노드의 부모의 부모 노드 != 루트 노드

				2-2-1) 새 노드의 부모의 부모 노드를 빨간색으로 변경
				: 이에 따라, 새 노드의 부모의 부모 노드와 새 노드의 부모의 부모의 부모 노드 간에 DEF2)의 위반사항 발생 가능

				2-2-2) 새 노드의 부모의 부모 노드를 삽입이 발생하는 새 노드로 간주
				: goto Determining Proc (새 노드의 부모의 부모 노드와 새 노드의 부모의 부모의 부모 노드 간에 DEF2)의 위반사항이 존재하는지 다시 판별)
	***/

	srcNewNode->_parent->_color = uncleNode->_color = COLOR::BLACK; //새 노드의 부모와 새 노드의 부모의 형제 노드 (삼촌)를 검은색으로 변경

	if (srcNewNode->_parent->_parent == (*srcRootNode)) //새 노드의 부모의 부모 노드가 루트 노드인 경우
		return; //DEF2)의 위반사항이 모두 해결되었으므로, 처리 중지

	//새 노드의 부모의 부모 노드가 루트 노드가 아닌 경우, 새 노드의 부모의 부모 노드를 빨간색으로 변경
	srcNewNode->_parent->_parent->_color = COLOR::RED;

	//새 노드의 부모의 부모 노드를 삽입이 발생하는 새 노드로 간주
	srcNewNode = srcNewNode->_parent->_parent;

	//새 노드의 부모의 부모 노드와 새 노드의 부모의 부모의 부모 노드 간에 DEF2)의 위반사항이 존재하는지 판별
	goto DETERMINING_PROC;
}

/// <summary>
/// 대상 트리에 삭제하고자 하는 대상 데이터가 포함 된 노드 삭제
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <param name="targetData">삭제하고자 하는 대상 데이터</param>
/// <param name="deallocateAfterRemove">삭제 대상 노드에 대한 메모리 해제 수행 여부</param>
void RBT_RemoveNode(NODE** srcRootNode, RBT_DATA_TYPE targetData, bool deallocateAfterRemove)
{
	/***
		< 삭제하고자 하는 대상 데이터가 포함 된 노드 (이하, 삭제 대상 노드) 및
		삭제 대상 노드의 자리를 대체 할 노드 (이하, 이동 대상 노드) 선택 >

		! 연산 과정의 단순화를 위해 이동 대상 노드가 검은색 더미 단말 노드일 경우에도, 임시로 부모로의 연결과 자식 간의 연결 허용

		1) 삭제 대상 노드의 자식 노드 보유 상태에 따라,

			1-1) 삭제 대상 노드가 검은색 더미 단말 노드가 아닌 왼쪽, 오른쪽 자식 노드를 모두 갖고 있을 경우
			: 표준 BST의 삭제 처리 루틴에서 삭제 대상 노드가 왼쪽, 오른쪽 자식 노드를 모두 갖고 있을 경우와 동일

				1-1-1) 삭제 대상 노드의 오른쪽 하위 트리에서 최소 값을 이동 대상 노드로 선택

				1-1-2) 삭제 대상 노드의 데이터를 이동 대상 노드의 데이터로 변경
				: 이 시점에, 삭제 대상 노드의 색을 이동 대상 노드의 색으로 변경 할 경우, DEF3) 혹은 DEF4)를 위반하므로,
				삭제 대상 노드를 대체 할 이동 대상 노드의 색은 삭제 대상 노드의 색을 따름

				1-1-3) 현재 이동 대상 노드를 삭제 대상 노드로 선택

				1-1-4) 이동 대상 노드의 오른쪽 자식 노드를 이동 대상 노드로 선택
				: 이 시점에, 기존 이동 대상 노드는 해당 하위 트리에서 가장 작은 노드이므로,
				기존 이동 대상 노드의 왼쪽 자식 노드는 항상 검은색 더미 단말 노드

			1-2) 삭제 대상 노드가 검은색 더미 단말 노드가 아닌 하나의 자식 노드만 갖고 있거나 자식 노드를 갖고 있지 않을 경우
			: 표준 BST의 삭제 처리 루틴에서 삭제 대상 노드가 하나의 자식만 갖고 있거나 자식 노드를 갖고 있지 않는 경우와 동일

				1-2-1) 이동 대상 노드를 삭제 대상 노드가 가지고 있던 한 단계 하위 자식 노드로 선택
				: 이 시점에, 삭제 대상 노드는 최대 하나의 자식 노드만 갖고 있으므로,
				삭제 대상 노드의 한 단계 하위 자식 노드가 검은색 더미 단말 노드가 아닌 검은색 노드일 경우,
				삭제 대상 노드 기준 왼쪽 및 오른쪽 경로에 대해 DEF4)를 위반
				(하나의 자식 노드만 갖고 있을 경우 - 빨간색 노드, 자식 노드를 갖고 있지 않을 경우 - 검은색 더미 단말 노드)

		2) 이동 대상 노드의 부모를 삭제 대상 노드의 부모로 연결
		: 이 시점에, 이동 대상 노드는 삭제 대상 노드의 한 단계 하위 자식 노드

		3) 이동 대상 노드의 부모 (삭제 대상 노드)에서 자식으로의 연결에 대해 자기 자신으로의 연결을 제외 한 반대편 연결을 삭제 대상 노드의 반대편 연결로 연결

		4) 삭제 대상 노드의 루트 노드 여부에 따라,

			4-1) 삭제 대상 노드 == 루트 노드
			: 이동 대상 노드를 루트 노드로 변경

			4-2) 삭제 대상 노드 != 루트 노드
			: 삭제 대상 노드의 부모에서 자식으로의 연결을 이동 대상 노드로 연결

		5) 현재 삭제 대상 노드의 색에 따라,

			5-1) 삭제 대상 노드의 색 == 빨간색

				5-1-1) do nothing
				: 삭제 대상 노드가 빨간색이었으므로, 이동 대상 노드가 검은색이고 이동 대상 노드가 상위 트리로 위치가 이동되어도,
				트리의 각 경로에 대한 검은 노드의 수는 변동없으므로, DEF4)를 위반하지 않음

			5-2) 삭제 대상 노드의 색 == 검은색
			: DEF4) 위반에 대해 후속 처리를 위해 이동 대상 노드의 인접 노드의 색에 따른 처리로 이동
	***/

	if ((*srcRootNode) == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	NODE* removeTargetNode = RBT_SearchNode((*srcRootNode), targetData); //삭제 대상 노드
	NODE* moveTargetNode = NULL; //이동 대상 노드
	NODE* moveTargetSiblingNode = NULL; //이동 대상 노드의 반대쪽 형제 노드

	COLOR tmpColor;

	if (removeTargetNode->_left != dummyBlackTerminalNode && removeTargetNode->_right != dummyBlackTerminalNode) //삭제 대상 노드가 검은색 더미 단말 노드가 아닌 왼쪽, 오른쪽 자식 노드를 모두 갖고 있을 경우
	{
		//삭제 대상 노드의 오른쪽 하위 트리에서 최소 값을 이동 대상 노드로 선택
		moveTargetNode = RBT_SearchMinNode(removeTargetNode->_right);

		//삭제 대상 노드의 데이터를 이동 대상 노드의 데이터로 변경
		removeTargetNode->_data = moveTargetNode->_data;

		//현재 이동 대상 노드를 삭제 대상 노드로 선택
		removeTargetNode = moveTargetNode;

		//이동 대상 노드의 오른쪽 자식 노드를 이동 대상 노드로 선택
		moveTargetNode = moveTargetNode->_right;
	}
	else //삭제 대상 노드가 검은색 더미 단말 노드가 아닌 하나의 자식 노드만 갖고 있거나 자식 노드를 갖고 있지 않을 경우
	{
		//이동 대상 노드를 삭제 대상 노드가 가지고 있던 한 단계 하위 자식 노드로 선택
		(removeTargetNode->_left != dummyBlackTerminalNode) ?
			moveTargetNode = removeTargetNode->_left : moveTargetNode = removeTargetNode->_right;
	}

	//이동 대상 노드의 부모를 삭제 대상 노드의 부모로 연결
	moveTargetNode->_parent = removeTargetNode->_parent;

	//이동 대상 노드의 부모 (삭제 대상 노드)에서 자식으로의 연결에 대해 자기 자신으로의 연결을 제외 한 반대편 연결을 삭제 대상 노드의 반대편 연결로 연결
	(removeTargetNode->_left == moveTargetNode) ?
		moveTargetNode->_right = removeTargetNode->_right : moveTargetNode->_left = removeTargetNode->_left;

	if (removeTargetNode->_parent == NULL) //삭제 대상 노드가 루트 노드일 경우
	{
		(*srcRootNode) = moveTargetNode;
	}
	else //삭제 대상 노드가 루트 노드가 아닐 경우
	{
		//삭제 대상 노드의 부모에서 자식으로의 연결을 이동 대상 노드로 연결
		(removeTargetNode->_parent->_left == removeTargetNode) ?
			removeTargetNode->_parent->_left = moveTargetNode : removeTargetNode->_parent->_right = moveTargetNode;
	}

	switch (removeTargetNode->_color) //현재 삭제 대상 노드의 색에 따라
	{
	case COLOR::RED:
		goto END_PROC;

	case COLOR::BLACK:
		goto ADJ_MOVE_TARGET_PROC;
	}

ADJ_MOVE_TARGET_PROC: //이동 대상 노드의 인접 노드의 색에 따른 처리 루틴
	/***
		< 이동 대상 노드의 인접 노드의 색에 따른 처리 >

		! 삭제 된 노드의 색이 검은색 노드인 경우

		! 삭제 시 회전 및 색 변경이 발생되어 수행하고자 하는 목적 및 판별 과정에서의 '해당 수행 작업이 수행되어야만 하는 근본적인 판별 방법'은 무엇인가?
		: 이는 각 경로 간 검은 노드의 수의 균형을 유지하기 위함이며,
		이중 검은색 노드 발생에 따른 이를 없애기 위한 색 변경 시 발생되는 Side Effect에 따라, 회전 및 추가적인 색 변경 수행

		1) MPC : 이동 대상 노드의 부모 노드의 색 (R : Red, B : Black)
		2) MC : 이동 대상 노드의 색 (R : Red, B : Black)
		3) MSC : 이동 대상 노드의 반대쪽 형제 노드의 색 (R : Red, B : Black)
		4) MSCC : 이동 대상 노드의 반대쪽 형제 노드의 자식 노드의 색
		(BB : 모든 자식 노드 검은색, RR : 모든 자식 노드 빨간색, RB : 왼쪽 자식 노드 빨간색 - 오른쪽 자식 노드 검은색, BR : 왼쪽 자식 노드 검은색 - 오른쪽 자식 노드 빨간색)
		5) P : 이에 따른 수행 작업

		MPC	|	MC	|	MSC	|	MSCC	|	P
		R		B		R		-		throw exception
										: DEF2) 위반 (Invalid Red-Black Tree)

		B		B		R		!BB		throw exception
										: DEF2) 위반 (Invalid Red-Black Tree)

		R		R		-		-		이동 대상 노드와 이동 대상 노드의 부모 노드 간 DEF2) 위반 수정 및
										이동 대상 노드가 존재하는 경로의 검은 노드의 수 증가를 통한 복구를 위해,
										이동 대상 노드의 색 변경 (빨간색 => 검은색)

		B		R		-		-		이동 대상 노드가 존재하는 경로의 검은 노드의 수 증가를 통한 복구를 위해,
										이동 대상 노드의 색 변경 (빨간색 => 검은색)

		B		B		R		BB		1) 이동 대상 노드를 이중 검은색 노드로 간주
										: DEF4) 위반에서 DEF1) 위반으로 변경

										2) DEF1)을 위반하는 이중 검은색 노드 (이동 대상 노드)의 한 검은색을 없애기 위해,
										이중 검은색 노드 (이동 대상 노드)의 형제 노드로 자신의 한 검은색을 전달
										: 이에 따라, 이중 검은색 노드 (이동 대상 노드)의 형제 노드가 존재하는 경로의 검은 노드의 수 증가 및
										이중 검은색 노드 (이동 대상 노드)가 존재하는 경로의 검은 노드의 수 감소

										3) 이중 검은색 노드 (이동 대상 노드)의 형제 노드가 존재하는 경로의 검은 노드의 수 감소를 통한 복구 및
										이중 검은색 노드 (이동 대상 노드)가 존재하는 경로의 검은 노드의 수 증가를 통한 복구를 위해
										이중 검은색 노드 (이동 대상 노드)의 부모 노드 기준 이중 검은색 노드 (이동 대상 노드)가 존재하는 방향으로 회전

		-		B		B		BB		1) 이동 대상 노드를 이중 검은색 노드로 간주
										: DEF4) 위반에서 DEF1) 위반으로 변경

										2) 이중 검은색 노드 (이동 대상 노드)의 부모 노드의 색에 따라,

											2-1) 이중 검은색 노드 (이동 대상 노드)의 부모 노드의 색 == 빨간색

												2-1-1) DEF1)을 위반하는 이중 검은색 노드 (이동 대상 노드)의 한 검은색을 없애기 위해,
												이중 검은색 노드 (이동 대상 노드)의 부모 노드로 자신의 한 검은색을 전달
												: 이에 따라, 이중 검은색 노드 (이동 대상 노드)의 형제 노드가 존재하는 경로의 검은 노드의 수 증가

												2-1-2) 이중 검은색 노드 (이동 대상 노드)의 형제 노드가 존재하는 경로의 검은 노드의 수 감소를 통한 복구를 위해,
												이중 검은색 노드 (이동 대상 노드)의 형제 노드의 색 변경 (검은색 => 빨간색)

											2-2) 이중 검은색 노드 (이동 대상 노드)의 부모 노드의 색 == 검은색

												2-2-1) DEF1)을 위반하는 이중 검은색 노드 (이동 대상 노드)의 한 검은색을 없애기 위해,
												이중 검은색 노드 (이동 대상 노드)의 부모 노드로 자신의 한 검은색을 전달
												: 이에 따라, 이중 검은색 노드 (이동 대상 노드)의 형제 노드가 존재하는 경로의 검은 노드의 수 증가

												2-2-2) 이중 검은색 노드 (이동 대상 노드)의 형제 노드가 존재하는 경로의 검은 노드의 수 감소를 통한 복구를 위해,
												이중 검은색 노드 (이동 대상 노드)의 형제 노드의 색 변경 (검은색 => 빨간색)

												2-2-3) 아직 존재하는 이중 검은색 노드를 이동 대상 노드로 간주하여 이에 대해 판별 및 후속 처리 수행

		-		B		B		RB		1) 이동 대상 노드를 이중 검은색 노드로 간주
										: DEF4) 위반에서 DEF1) 위반으로 변경

										2) 이중 검은색 노드 (이동 대상 노드)의 부모 노드로부터의 방향에 따라,

											2-1) 이중 검은색 노드 (이동 대상 노드)의 부모 노드의 왼쪽 자식 == 이중 검은색 노드 (이동 대상 노드)

												2-1-1) DEF2), DEF4)를 위반하지 않으면서, -BB BR 형태로 변경 위해,
												이중 검은색 노드 (이동 대상 노드)의 형제 노드 기준 우회전 및
												이중 검은색 노드 (이동 대상 노드)의 형제 노드와 이중 검은색 노드 (이동 대상 노드)의 형제 노드의 왼쪽 빨간색 자식 노드 간 색 SWAP

												2-1-2) 이중 검은색 노드 (이동 대상 노드)의 형제 노드의 오른쪽 자식이 빨간색 노드로 변경되었으므로,
												-BB BR의 이중 검은색 노드 (이동 대상 노드)의 부모 노드의 왼쪽 자식 == 이중 검은색 노드 (이동 대상 노드)와 처리 동일

											2-2) 이중 검은색 노드 (이동 대상 노드)의 부모 노드의 오른쪽 자식 == 이중 검은색 노드 (이동 대상 노드)

												2-2-1) DEF1)을 위반하는 이중 검은색 노드 (이동 대상 노드)의 한 검은색을 없애기 위해,
												이중 검은색 노드 (이동 대상 노드)가 존재하는 경로의 검은 노드의 수를 증가시켜야 하므로,
												이중 검은색 노드 (이동 대상 노드)의 부모 노드 기준 이중 검은색 노드 (이동 대상 노드)가 존재하는 방향으로 회전 및
												이중 검은색 노드 (이동 대상 노드)의 형제 노드의 왼쪽 빨간색 자식 노드의 색 변경 (빨간색 => 검은색)

												2-2-2) 이중 검은색 노드 (이동 대상 노드)가 존재하는 경로의 검은 노드의 수 감소를 통한 복구를 위해,
												이중 검은색 노드 (이동 대상 노드)의 한 검은색 제거,
												현재 하위 트리의 루트 노드의 색을 이전 색으로 복구를 위해,
												이중 검은색 노드 (이동 대상 노드)의 형제 노드 (회전 발생 전)와 회전 발생 전 하위 트리의 루트 노드 간의 색 SWAP

		-		B		B		BR		1) 이동 대상 노드를 이중 검은색 노드로 간주
										: DEF4) 위반에서 DEF1) 위반으로 변경

										2) 이중 검은색 노드 (이동 대상 노드)의 부모 노드로부터의 방향에 따라,

											2-1) 이중 검은색 노드 (이동 대상 노드)의 부모 노드의 왼쪽 자식 == 이중 검은색 노드 (이동 대상 노드)

												2-1-1) DEF1)을 위반하는 이중 검은색 노드 (이동 대상 노드)의 한 검은색을 없애기 위해,
												이중 검은색 노드 (이동 대상 노드)가 존재하는 경로의 검은 노드의 수를 증가시켜야 하므로,
												이중 검은색 노드 (이동 대상 노드)의 부모 노드 기준 이중 검은색 노드 (이동 대상 노드)가 존재하는 방향으로 회전 및
												이중 검은색 노드 (이동 대상 노드)의 형제 노드의 오른쪽 빨간색 자식 노드의 색 변경 (빨간색 => 검은색)

												2-1-2) 이중 검은색 노드 (이동 대상 노드)가 존재하는 경로의 검은 노드의 수 감소를 통한 복구를 위해,
												이중 검은색 노드 (이동 대상 노드)의 한 검은색 제거,
												현재 하위 트리의 루트 노드의 색을 이전 색으로 복구를 위해,
												이중 검은색 노드 (이동 대상 노드)의 형제 노드 (회전 발생 전)와 회전 발생 전 하위 트리의 루트 노드 간의 색 SWAP

											2-2) 이중 검은색 노드 (이동 대상 노드)의 부모 노드의 오른쪽 자식 == 이중 검은색 노드 (이동 대상 노드)

												2-2-1) DEF2), DEF4)를 위반하지 않으면서, -BB RB 형태로 변경 위해,
												이중 검은색 노드 (이동 대상 노드)의 형제 노드 기준 좌회전 및
												이중 검은색 노드 (이동 대상 노드)의 형제 노드와 이중 검은색 노드 (이동 대상 노드)의 형제 노드의 오른쪽 빨간색 자식 노드 간 색 SWAP

												2-2-2) 이중 검은색 노드 (이동 대상 노드)의 형제 노드의 왼쪽 자식이 빨간색 노드로 변경되었으므로,
												-BB RB의 이중 검은색 노드 (이동 대상 노드)의 부모 노드의 오른쪽 자식 == 이중 검은색 노드 (이동 대상 노드)와 처리 동일

		-		B		B		RR		1) 이동 대상 노드를 이중 검은색 노드로 간주
										: DEF4) 위반에서 DEF1) 위반으로 변경

										2) 이중 검은색 노드 (이동 대상 노드)의 부모 노드로부터의 방향에 따라,

											2-1) 이중 검은색 노드 (이동 대상 노드)의 부모 노드의 왼쪽 자식 == 이중 검은색 노드 (이동 대상 노드)
											: -BB BR의 2-1)과 같이 처리 할 경우 DEF2) 위반사항이 발생하며,
											이를 해결하기 위해 색 변경을 수행 할 경우, DEF4) 위반사항이 발생하므로,
											-BB RB의 2-1)와 처리 과정 동일

											2-2) 이중 검은색 노드 (이동 대상 노드)의 부모 노드의 오른쪽 자식 == 이중 검은색 노드 (이동 대상 노드)
											: -BB RB의 2-2)과 같이 처리 할 경우 DEF2) 위반사항이 발생하며,
											이를 해결하기 위해 색 변경을 수행 할 경우, DEF4) 위반사항이 발생하므로,
											-BB BR의 2-2)와 처리 과정 동일

		( 공통 처리 )

		1) 각 이동 대상 노드의 인접 노드의 색에 따른 처리를 위한 판별 과정 시, 현재 이동 대상 노드가 트리의 최상위 루트 노드 여부에 따라,

			1-1) 이동 대상 노드 == 트리의 최상위 루트 노드
			: 현재 이동 대상 노드를 검은색으로 변경 및 처리 중지 (트리의 최상위 루트 노드 기준, 양 방향의 검은 노드의 수는 변함없음)

			1-2) 이동 대상 노드 != 트리의 최상위 루트 노드
			: 이동 대상 노드의 인접 노드의 색에 따른 처리 수행

		---

		ex)

		- R : Red
		- B : Black
		- DB : Double Black
		- 검은색 더미 단말 노드 및 인접한 상위, 하위 트리 생략

		1) BBR BB
		: 이중 검은색 노드 (이동 대상 노드)의 부모 노드의 왼쪽 자식 == 이중 검은색 노드 (이동 대상 노드),
		이중 검은색 노드 (이동 대상 노드)의 부모 노드의 오른쪽 자식 == 이중 검은색 노드 (이동 대상 노드)

							B
				R (sibling)			DB (moveTarget)
			B		B


			=> DEF1)을 위반하는 이중 검은색 노드 (이동 대상 노드)의 한 검은색을 없애기 위해,
			이중 검은색 노드 (이동 대상 노드)가 존재하는 경로의 검은 노드의 수를 증가시켜야 하므로,
			이중 검은색 노드 (이동 대상 노드)의 형제 노드로 자신의 한 검은색을 전달

			- 이중 검은색 노드 (이동 대상 노드)의 형제 노드가 존재하는 경로의 검은 노드의 수 증가
			- 이중 검은색 노드 (이동 대상 노드)가 존재하는 경로의 검은 노드의 수 감소

							B
				B (sibling)			B (moveTarget)
			B		B

			=> 이중 검은색 노드 (이동 대상 노드)의 형제 노드가 존재하는 경로의 검은 노드의 수 감소를 통한 복구 및
			이중 검은색 노드 (이동 대상 노드)가 존재하는 경로의 검은 노드의 수 증가를 통한 복구를 위해



			=> 이중 검은색 노드 (이동 대상 노드)의 형제 노드가 존재하는 경로의 검은 노드의 수 감소를 통한 복구 및
			이중 검은색 노드 (이동 대상 노드)가 존재하는 경로의 검은 노드의 수 증가를 통한 복구를 위해
			이중 검은색 노드 (이동 대상 노드)의 부모 노드 기준 이중 검은색 노드 (이동 대상 노드)가 존재하는 방향으로 회전

			- 이중 검은색 노드 (이동 대상 노드)의 형제 노드 (회전 발생 전) 기준 왼쪽 경로의 검은 노드의 수 감소를 통한 복구
			- 이중 검은색 노드 (이동 대상 노드)가 존재하는 경로의 검은 노드의 수 증가를 통한 복구

							B (oldSibling)
					B				B
								B		B (moveTarget)

		2) -BB BB
		: 이중 검은색 노드 (이동 대상 노드)의 부모 노드의 왼쪽 자식 == 이중 검은색 노드 (이동 대상 노드),
		이중 검은색 노드 (이동 대상 노드)의 부모 노드의 오른쪽 자식 == 이중 검은색 노드 (이동 대상 노드)

			2-1) 이중 검은색 노드 (이동 대상 노드)의 부모 노드의 색 == 빨간색

							R
				B (sibling)			DB (moveTarget)
			B		B

			=> DEF1)을 위반하는 이중 검은색 노드 (이동 대상 노드)의 한 검은색을 없애기 위해,
			이중 검은색 노드 (이동 대상 노드)의 부모 노드로 자신의 한 검은색을 전달

			- 이중 검은색 노드 (이동 대상 노드)의 형제 노드가 존재하는 경로의 검은 노드의 수 증가

							B
				B (sibling)			B (moveTarget)
			B		B

			=> 이중 검은색 노드 (이동 대상 노드)의 형제 노드가 존재하는 경로의 검은 노드의 수 감소를 통한 복구를 위해,
			이중 검은색 노드 (이동 대상 노드)의 형제 노드의 색 변경 (검은색 => 빨간색)

			- 이중 검은색 노드 (이동 대상 노드)의 형제 노드가 존재하는 경로의 검은 노드의 수 감소를 통한 복구

							B
				R (sibling)			B (moveTarget)
			B		B

			2-2) 이중 검은색 노드 (이동 대상 노드)의 부모 노드의 색 == 검은색

							B
				B (sibling)			DB (moveTarget)
			B		B

			=> DEF1)을 위반하는 이중 검은색 노드 (이동 대상 노드)의 한 검은색을 없애기 위해,
			이중 검은색 노드 (이동 대상 노드)의 부모 노드로 자신의 한 검은색을 전달

			- 이중 검은색 노드 (이동 대상 노드)의 형제 노드가 존재하는 경로의 검은 노드의 수 증가

							DB
				B (sibling)			B (moveTarget)
			B		B

			=> 이중 검은색 노드 (이동 대상 노드)의 형제 노드가 존재하는 경로의 검은 노드의 수 감소를 통한 복구를 위해,
			이중 검은색 노드 (이동 대상 노드)의 형제 노드의 색 변경 (검은색 => 빨간색)

			- 이중 검은색 노드 (이동 대상 노드)의 형제 노드가 존재하는 경로의 검은 노드의 수 감소를 통한 복구

							DB
				R (sibling)			B (moveTarget)
			B		B

			=> 아직 존재하는 이중 검은색 노드를 이동 대상 노드로 간주하여 이에 대해 판별 및 후속 처리 수행
			: 트리의 최상위 루트 노드가 이중 검은색 노드가 될 경우, 이를 단일 검은색 노드로 변경시켜도,
			트리의 최상위 루트 노드 기준 왼쪽 및 오른쪽 경로의 검은 노드의 수는 변함없으므로 단일 검은색으로 변경시킬 것

		3) -BB RB

			3-1) 이중 검은색 노드 (이동 대상 노드)의 부모 노드의 왼쪽 자식 == 이중 검은색 노드 (이동 대상 노드)

								B (or R)
				DB (moveTarget)			B (sibling)
									R		B

			=> DEF2), DEF4)를 위반하지 않으면서, -BB BR 형태로 변경 위해,
			이중 검은색 노드 (이동 대상 노드)의 형제 노드 기준 우회전 및
			이중 검은색 노드 (이동 대상 노드)의 형제 노드와 이중 검은색 노드 (이동 대상 노드)의 형제 노드의 왼쪽 빨간색 자식 노드 간 색 SWAP

									B (or R)
				DB (moveTarget)			B (newSibling)
											R (oldSibling)
												B

			=> 이중 검은색 노드 (이동 대상 노드)의 형제 노드의 오른쪽 자식이 빨간색 노드로 변경되었으므로,
			-BB BR의 이중 검은색 노드 (이동 대상 노드)의 부모 노드의 왼쪽 자식 == 이중 검은색 노드 (이동 대상 노드)와 처리 동일

			3-2) 이중 검은색 노드 (이동 대상 노드)의 부모 노드의 오른쪽 자식 == 이중 검은색 노드 (이동 대상 노드)

							B (or R)
				B (sibling)			DB (moveTarget)
			R		B

			=> DEF1)을 위반하는 이중 검은색 노드 (이동 대상 노드)의 한 검은색을 없애기 위해,
			이중 검은색 노드 (이동 대상 노드)가 존재하는 경로의 검은 노드의 수를 증가시켜야 하므로,
			이중 검은색 노드 (이동 대상 노드)의 부모 노드 기준 이중 검은색 노드 (이동 대상 노드)가 존재하는 방향으로 회전 및
			이중 검은색 노드 (이동 대상 노드)의 형제 노드의 왼쪽 빨간색 자식 노드의 색 변경 (빨간색 => 검은색)

			- 이중 검은색 노드 (이동 대상 노드)가 존재하는 경로의 검은 노드의 수 증가

						B (oldSibling)
				B					B (or R)
								B		DB (moveTarget)

			=> 이중 검은색 노드 (이동 대상 노드)가 존재하는 경로의 검은 노드의 수 감소를 통한 복구를 위해,
			이중 검은색 노드 (이동 대상 노드)의 한 검은색 제거,
			현재 하위 트리의 루트 노드의 색을 이전 색으로 복구를 위해,
			이중 검은색 노드 (이동 대상 노드)의 형제 노드 (회전 발생 전)와 회전 발생 전 하위 트리의 루트 노드 간의 색 SWAP

						B (or R, oldSibling)
				B					B
								B		B (moveTarget)

		4) -BB BR

			4-1) 이중 검은색 노드 (이동 대상 노드)의 부모 노드의 왼쪽 자식 == 이중 검은색 노드 (이동 대상 노드)

								B (or R)
				DB (moveTarget)			B (sibling)
									B		R

			=> DEF1)을 위반하는 이중 검은색 노드 (이동 대상 노드)의 한 검은색을 없애기 위해,
			이중 검은색 노드 (이동 대상 노드)가 존재하는 경로의 검은 노드의 수를 증가시켜야 하므로,
			이중 검은색 노드 (이동 대상 노드)의 부모 노드 기준 이중 검은색 노드 (이동 대상 노드)가 존재하는 방향으로 회전 및
			이중 검은색 노드 (이동 대상 노드)의 형제 노드의 오른쪽 빨간색 자식 노드의 색 변경 (빨간색 => 검은색)

			- 이중 검은색 노드 (이동 대상 노드)가 존재하는 경로의 검은 노드의 수 증가

							B (oldSibling)
					B (or R)			B
			DB (moveTarget)	B

			=> 이중 검은색 노드 (이동 대상 노드)가 존재하는 경로의 검은 노드의 수 감소를 통한 복구를 위해,
			이중 검은색 노드 (이동 대상 노드)의 한 검은색 제거,
			현재 하위 트리의 루트 노드의 색을 이전 색으로 복구를 위해,
			이중 검은색 노드 (이동 대상 노드)의 형제 노드 (회전 발생 전)와 회전 발생 전 하위 트리의 루트 노드 간의 색 SWAP

							B (or R, oldSibling)
					B					B
			B (moveTarget)	B

			4-2) 이중 검은색 노드 (이동 대상 노드)의 부모 노드의 오른쪽 자식 == 이중 검은색 노드 (이동 대상 노드)

							B (or R)
				B (sibling)			DB (moveTarget)
			B		R

			=> DEF2), DEF4)를 위반하지 않으면서, -BB RB 형태로 변경 위해,
			이중 검은색 노드 (이동 대상 노드)의 형제 노드 기준 좌회전 및
			이중 검은색 노드 (이동 대상 노드)의 형제 노드와 이중 검은색 노드 (이동 대상 노드)의 형제 노드의 오른쪽 빨간색 자식 노드 간 색 SWAP

							B (or R)
					B (newSibling)	DB (moveTarget)
				R (oldSibling)
			B

			=> 이중 검은색 노드 (이동 대상 노드)의 형제 노드의 왼쪽 자식이 빨간색 노드로 변경되었으므로,
			-BB RB의 이중 검은색 노드 (이동 대상 노드)의 부모 노드의 오른쪽 자식 == 이중 검은색 노드 (이동 대상 노드)와 처리 동일

		5) -BB RR

			5-1) 이중 검은색 노드 (이동 대상 노드)의 부모 노드의 왼쪽 자식 == 이중 검은색 노드 (이동 대상 노드)

								B (or R)
				DB (moveTarget)			B (sibling)
									R		R


			=> -BB BR의 4-1)과 같이 처리 할 경우 DEF2) 위반사항이 발생하며,
			이를 해결하기 위해 색 변경을 수행 할 경우,
			DEF4) 위반사항이 발생하므로, -BB RB의 3-1)와 처리 과정 동일

								B (or R)
					B 				B (oldSibling)
			B (moveTarget)				R

			5-2) 이중 검은색 노드 (이동 대상 노드)의 부모 노드의 오른쪽 자식 == 이중 검은색 노드 (이동 대상 노드)

							B (or R)
				B (sibling)			DB (moveTarget)
			R		R

			=> -BB RB의 3-2)과 같이 처리 할 경우 DEF2) 위반사항이 발생하며,
			이를 해결하기 위해 색 변경을 수행 할 경우,
			DEF4) 위반사항이 발생하므로, -BB BR의 4-2)와 처리 과정 동일

							B (or R)
				B (oldSibling)			B
			R								B (moveTarget)
	***/

	while (moveTargetNode->_parent != NULL)
	{
		moveTargetSiblingNode =
			(moveTargetNode->_parent->_left == moveTargetNode) ?
			moveTargetNode->_parent->_right : moveTargetNode->_parent->_left;

		if ((moveTargetNode->_parent->_color == COLOR::RED &&
			moveTargetNode->_color == COLOR::BLACK &&
			moveTargetSiblingNode->_color == COLOR::RED)) //RBR -
			throw std::logic_error(std::string(__func__) + std::string(" : DEF2) violation (Invalid Red-Black Tree : RBR -)"));

		if (moveTargetNode->_color == COLOR::RED) //RR- - or BR- -
		{
			moveTargetNode->_color = COLOR::BLACK;
			goto END_PROC;
		}
		else //moveTargetNode->_color == COLOR::BLACK
		{
			switch (moveTargetSiblingNode->_color) //이동 대상 노드의 형제 노드의 색에 따라
			{
			case COLOR::RED: //BBR -
				if (!(moveTargetSiblingNode->_left->_color == COLOR::BLACK && moveTargetSiblingNode->_right->_color == COLOR::BLACK)) //BBR !BB
					throw std::logic_error(std::string(__func__) + std::string(" : DEF2) violation (Invalid Red-Black Tree : BBR !BB)"));

				moveTargetSiblingNode->_color = moveTargetNode->_color;

				//이중 검은색 노드 (이동 대상 노드)의 부모 노드 기준 이중 검은색 노드 (이동 대상 노드)가 존재하는 방향으로 회전
				RBT_RotateTree(srcRootNode, moveTargetNode->_parent,
					(moveTargetNode->_parent->_left == moveTargetNode) ?
					ROTATE_DIRECTION::LEFT : ROTATE_DIRECTION::RIGHT);

				goto END_PROC;

			case COLOR::BLACK:
				if (moveTargetSiblingNode->_left->_color == COLOR::BLACK && moveTargetSiblingNode->_right->_color == COLOR::BLACK) //-BB BB
				{
					switch (moveTargetNode->_parent->_color) //이동 대상 노드의 부모 노드의 색에 따라,
					{
					case COLOR::RED:
						moveTargetNode->_parent->_color = moveTargetNode->_color;
						moveTargetSiblingNode->_color = COLOR::RED;
						goto END_PROC;

					case COLOR::BLACK:
						moveTargetNode->_parent->_color = moveTargetNode->_color;
						moveTargetSiblingNode->_color = COLOR::RED;
						moveTargetNode = moveTargetNode->_parent; //아직 존재하는 이중 검은색 노드를 이동 대상 노드로 간주
						break;
					}
				}
				else if (moveTargetNode->_parent->_right == moveTargetNode) //(moveTargetNode->_parent->_right == moveTargetNode) : -BB RB or -BB BR or -BB RR
				{
					if (moveTargetSiblingNode->_right->_color == COLOR::RED) //-BB BR or -BB RR
					{
						SWAP(moveTargetSiblingNode->_color, moveTargetSiblingNode->_right->_color, tmpColor);

						//이중 검은색 노드 (이동 대상 노드)의 형제 노드 기준 좌회전
						RBT_RotateTree(srcRootNode, moveTargetSiblingNode, ROTATE_DIRECTION::LEFT);
						moveTargetSiblingNode =
							(moveTargetNode->_parent->_left == moveTargetNode) ?
							moveTargetNode->_parent->_right : moveTargetNode->_parent->_left;
					}

					moveTargetSiblingNode->_left->_color = COLOR::BLACK;
					SWAP(moveTargetSiblingNode->_color, moveTargetSiblingNode->_parent->_color, tmpColor);

					//이중 검은색 노드 (이동 대상 노드)의 부모 노드 기준 이중 검은색 노드 (이동 대상 노드)가 존재하는 방향으로 회전
					RBT_RotateTree(srcRootNode, moveTargetNode->_parent, ROTATE_DIRECTION::RIGHT);
					goto END_PROC;
				}
				else if (moveTargetNode->_parent->_left == moveTargetNode) //(moveTargetNode->_parent->_left == moveTargetNode) : -BB RB or -BB BR or -BB RR
				{
					if (moveTargetSiblingNode->_left->_color == COLOR::RED) //-BB RB or -BB RR
					{
						SWAP(moveTargetSiblingNode->_color, moveTargetSiblingNode->_left->_color, tmpColor);

						//이중 검은색 노드 (이동 대상 노드)의 형제 노드 기준 우회전
						RBT_RotateTree(srcRootNode, moveTargetSiblingNode, ROTATE_DIRECTION::RIGHT);
						moveTargetSiblingNode =
							(moveTargetNode->_parent->_left == moveTargetNode) ?
							moveTargetNode->_parent->_right : moveTargetNode->_parent->_left;
					}

					moveTargetSiblingNode->_right->_color = COLOR::BLACK;
					SWAP(moveTargetSiblingNode->_color, moveTargetSiblingNode->_parent->_color, tmpColor);

					//이중 검은색 노드 (이동 대상 노드)의 부모 노드 기준 이중 검은색 노드 (이동 대상 노드)가 존재하는 방향으로 회전
					RBT_RotateTree(srcRootNode, moveTargetNode->_parent, ROTATE_DIRECTION::LEFT);
					goto END_PROC;
				}
				else
				{
					VAR_DUMP(moveTargetNode->_data);
					VAR_DUMP(moveTargetNode->_parent == NULL);
					VAR_DUMP(moveTargetNode->_left == NULL && moveTargetNode->_right == NULL);
					VAR_DUMP(static_cast<const unsigned>(moveTargetNode->_color));

					VAR_DUMP(moveTargetNode->_parent->_left == moveTargetNode);
					VAR_DUMP(moveTargetNode->_parent->_right == moveTargetNode);

					VAR_DUMP(static_cast<const unsigned>(moveTargetSiblingNode->_left->_color));
					VAR_DUMP(static_cast<const unsigned>(moveTargetSiblingNode->_right->_color));

					throw std::logic_error(std::string(__func__) + std::string(" : Unknown Logic Error"));
				}

				break;
			}
		}

	}

	goto END_PROC;

END_PROC:
	(*srcRootNode)->_color = COLOR::BLACK;

	if ((*srcRootNode) == dummyBlackTerminalNode)
		(*srcRootNode) = NULL;

	dummyBlackTerminalNode->_parent = dummyBlackTerminalNode->_left = dummyBlackTerminalNode->_right = NULL;

	if (deallocateAfterRemove)
		RBT_DeallocateNode(&removeTargetNode);
}

/// <summary>
/// 대상 트리에 찾고자 하는 대상 데이터가 포함 된 노드 반환
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <param name="targetData">찾고자 하는 대상 데이터</param>
/// <returns>찾고자 하는 대상 데이터가 포함 된 노드</returns>
NODE* RBT_SearchNode(NODE* srcRootNode, RBT_DATA_TYPE targetData)
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
	if (srcRootNode == NULL || srcRootNode == dummyBlackTerminalNode)
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
	if (srcRootNode == NULL || srcRootNode == dummyBlackTerminalNode)
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
				RBT_InsertNodeHelper(&((*srcRootNode)->_left), srcNewNode);
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
				RBT_InsertNodeHelper(&((*srcRootNode)->_right), srcNewNode);
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

		2) 대상 부모 노드의 왼쪽 자식 노드의 부모를 대상 부모 노드의 부모로 연결

		3) 대상 부모 노드와 루트 노드 일치 여부에 따라,

			3-1) 대상 부모 노드 == 루트 노드
			: 루트 노드를 대상 부모 노드의 왼쪽 자식 노드로 변경

			3-2) 대상 부모 노드 != 루트 노드
			: 대상 부모 노드의 부모의 자식 (왼쪽 혹은 오른쪽)을 대상 부모 노드의 왼쪽 자식 노드로 연결

		4) 대상 부모 노드의 왼쪽 자식 노드의 오른쪽을 대상 부모 노드로 연결

		5) 대상 부모 노드의 부모를 대상 부모 노드의 왼쪽 자식 노드로 연결

		6) 대상 부모 노드의 왼쪽을 대상 부모 노드의 왼쪽 자식의 오른쪽으로 연결

		---

		ex)
					8 (대상 부모 노드)
			5				9
		3		6

		<= 5를 기준으로 좌회전 후,
		=> 8을 기준으로 우회전 후,

					5 (대상 부모 노드)
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

	if (moveTargetNode == dummyBlackTerminalNode || moveTargetNode == NULL)
		throw std::logic_error(std::string(__func__) + std::string(" : Wrong Tree Rotation cond"));

	//우회전 : 대상 부모 노드의 왼쪽 자식 노드의 오른쪽 자식 노드의 부모를 대상 부모 노드로 연결
	if (moveTargetChildNode != dummyBlackTerminalNode)
		moveTargetChildNode->_parent = srcTargetParentNode;

	//우회전 : 대상 부모 노드의 왼쪽 노드에서 부모를 대상 부모 노드의 부모 노드로 연결
	moveTargetNode->_parent = srcTargetParentNode->_parent;

	if (srcTargetParentNode == (*srcRootNode)) //대상 부모 노드가 루트 노드일 경우
	{
		(*srcRootNode) = moveTargetNode;
	}
	else //대상 부모 노드가 루트 노드가 아닐 경우
	{
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

/// <summary>
/// 대상 트리의 특정 경로 방향에 대해 특정 색을 가진 노드 개수 반환
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <param name="color">노드의 색</param>
/// <param name="pathDirection">경로 방향</param>
/// <returns>특정 색을 가진 노드 개수</returns>
size_t RBT_GetColorCount(NODE* srcRootNode, COLOR color, PATH_DIRECTION pathDirection)
{
	size_t retVal = 0; //특정 색을 가진 노드 개수

	if (srcRootNode == NULL)
		return 0;

	if (srcRootNode->_color == color)
		retVal++;

	switch (pathDirection)
	{
	case PATH_DIRECTION::RIGHT:
		if (srcRootNode->_right != NULL)
			retVal += RBT_GetColorCount(srcRootNode->_right, color, pathDirection);
		break;

	case PATH_DIRECTION::LEFT:
		if (srcRootNode->_left != NULL)
			retVal += RBT_GetColorCount(srcRootNode->_left, color, pathDirection);
		break;

	case PATH_DIRECTION::BOTH:
		if (srcRootNode->_right != NULL)
			retVal += RBT_GetColorCount(srcRootNode->_right, color, pathDirection);
		if (srcRootNode->_left != NULL)
			retVal += RBT_GetColorCount(srcRootNode->_left, color, pathDirection);
		break;
	}

	return retVal;
}

#ifdef DEBUG_MODE
/// <summary>
/// 대상 트리에 대한 유효성 검사 수행
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
void RBT_ValidateTree(NODE* srcRootNode)
{
	if (srcRootNode == NULL)
		return;

	if (srcRootNode->_color == COLOR::RED || dummyBlackTerminalNode == NULL ||
		dummyBlackTerminalNode->_color == COLOR::RED) //DEF1) validation
		throw std::logic_error(std::string(__func__) + std::string(" : DEF1) violation (Invalid Red-Black Tree)"));

	if (dummyBlackTerminalNode->_parent != NULL) //검은색 더미 단말 노드에서 부모로의 연결은 허용하지 않음 
		throw std::logic_error(std::string(__func__) + std::string(" : Not allowed parent connection from dummy"));

	NODE* currentNode = srcRootNode; //현재 노드
	char execBranchSingleFlag = (0x0); //실행 분기 단일 플래그

	/***
		< 실행 분기 단일 플래그 >

		0000(2) : 현재 노드 기준 DEF2), DEF4), DEF5) 검증
		0001(2) : 현재 노드 기준 DEF5) 검증 및 왼쪽 노드 방문
		0010(2) : 현재 노드 기준 DEF5) 검증 및 오른쪽 노드 방문
		0011(2) : pop 및 필요 할 경우 해당 요소에 대한 마지막 작업 수행
	***/

	std::stack<std::tuple<NODE*, char>> callStack; //Call Stack
	callStack.push(std::make_tuple(currentNode, execBranchSingleFlag));

	size_t leftPathBlackNodeCount = 0;
	size_t rightPathBlackNodeCount = 0;

	size_t leftNodeLeftPathBlackNodeCount = 0;
	size_t leftNodeRightPathBlackNodeCount = 0;
	size_t rightNodeLeftPathBlackNodeCount = 0;
	size_t rightNodeRightPathBlackNodeCount = 0;

	while (!callStack.empty())
	{
		std::tie(currentNode, execBranchSingleFlag) = callStack.top();

		switch (execBranchSingleFlag)
		{
		case (0x0): //현재 노드 기준 DEF2), DEF4) 검증
			if (currentNode->_color == COLOR::RED &&
				(currentNode->_left->_color == COLOR::RED ||
					currentNode->_right->_color == COLOR::RED)) //DEF2) validation
				throw std::logic_error(std::string(__func__) + std::string(" : DEF2) violation (Invalid Red-Black Tree)"));

			leftPathBlackNodeCount = 0;
			rightPathBlackNodeCount = 0;

			leftNodeLeftPathBlackNodeCount = 0;
			leftNodeRightPathBlackNodeCount = 0;
			rightNodeLeftPathBlackNodeCount = 0;
			rightNodeRightPathBlackNodeCount = 0;

			try //DEF4) validation
			{
				if (srcRootNode->_color == COLOR::BLACK)
				{
					leftPathBlackNodeCount += 1;
					rightPathBlackNodeCount += 1;
				}

				leftNodeLeftPathBlackNodeCount = RBT_GetColorCount(srcRootNode->_left, COLOR::BLACK, PATH_DIRECTION::LEFT);
				leftNodeRightPathBlackNodeCount = RBT_GetColorCount(srcRootNode->_left, COLOR::BLACK, PATH_DIRECTION::RIGHT);

				if (leftNodeLeftPathBlackNodeCount == leftNodeRightPathBlackNodeCount)
					leftPathBlackNodeCount += leftNodeLeftPathBlackNodeCount;
				else
					throw std::logic_error(std::string(__func__) + std::string(" : DEF4) violation (Invalid Red-Black Tree)"));

				rightNodeLeftPathBlackNodeCount = RBT_GetColorCount(srcRootNode->_right, COLOR::BLACK, PATH_DIRECTION::LEFT);
				rightNodeRightPathBlackNodeCount = RBT_GetColorCount(srcRootNode->_right, COLOR::BLACK, PATH_DIRECTION::RIGHT);

				if (rightNodeLeftPathBlackNodeCount == rightNodeRightPathBlackNodeCount)
					rightPathBlackNodeCount += rightNodeLeftPathBlackNodeCount;
				else
					throw std::logic_error(std::string(__func__) + std::string(" : DEF4) violation (Invalid Red-Black Tree)"));

				if (leftPathBlackNodeCount != rightPathBlackNodeCount)
					throw std::logic_error(std::string(__func__) + std::string(" : DEF4) violation (Invalid Red-Black Tree)"));

			}
			catch (const std::logic_error& ex)
			{
				VAR_DUMP(srcRootNode->_data);
				VAR_DUMP(srcRootNode->_left->_data);
				VAR_DUMP(srcRootNode->_right->_data);

				VAR_DUMP(leftPathBlackNodeCount);
				VAR_DUMP(rightPathBlackNodeCount);
				VAR_DUMP(leftNodeLeftPathBlackNodeCount);
				VAR_DUMP(leftNodeRightPathBlackNodeCount);
				VAR_DUMP(rightNodeLeftPathBlackNodeCount);
				VAR_DUMP(rightNodeRightPathBlackNodeCount);

				throw ex;
			}

			std::get<1>(callStack.top()) = (0x1);

		case (0x1): //현재 노드 기준 DEF5) 검증 및 왼쪽 노드 방문
			if (currentNode->_left != dummyBlackTerminalNode)
			{
				if (currentNode->_data <= currentNode->_left->_data) //DEF5) validation
				{
					VAR_DUMP(currentNode->_data);
					VAR_DUMP(currentNode->_left->_data);

					throw std::logic_error(std::string(__func__) + std::string(" : DEF5) violation (Invalid Red-Black Tree)"));
				}

				std::get<1>(callStack.top()) = (0x2);
				callStack.push(std::make_tuple(currentNode->_left, (0x0)));
				continue;
			}

		case (0x2): //현재 노드 기준 DEF5) 검증 및 오른쪽 노드 방문
			if (currentNode->_right != dummyBlackTerminalNode)
			{
				if (currentNode->_data >= currentNode->_right->_data) //DEF5) validation
				{
					VAR_DUMP(currentNode->_data);
					VAR_DUMP(currentNode->_right->_data);

					throw std::logic_error(std::string(__func__) + std::string(" : DEF5) violation (Invalid Red-Black Tree)"));
				}

				std::get<1>(callStack.top()) = (0x3);
				callStack.push(std::make_tuple(currentNode->_right, (0x0)));
				continue;
			}

		case (0x3): //pop 및 필요 할 경우 해당 요소에 대한 마지막 작업 수행
			callStack.pop();
			break;

		default:
			throw std::logic_error(std::string(__func__) + std::string(" : Invalid Flag"));
		}
	}
}
#endif