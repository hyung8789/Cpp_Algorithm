#include "BST_Core.h"

/// <summary>
/// 새로운 노드 생성 및 생성 된 노드 반환
/// </summary>
/// <param name="srcData">노드의 데이터</param>
/// <returns>생성 된 노드</returns>
NODE* BST_CreateNode(DATA_TYPE srcData)
{
	NODE* retVal = (NODE*)malloc(sizeof(NODE));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_data = srcData;
	retVal->_left = retVal->_right = NULL;

	return retVal;
}

/// <summary>
/// 대상 노드에 할당 된 메모리 해제
/// </summary>
/// <param name="srcNode">대상 노드</param>
void BST_DeallocateNode(NODE** srcNode)
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
void BST_DeallocateTree(NODE** srcRootNode)
{
	if ((*srcRootNode) != NULL) //후위 순회로 왼쪽 끝 노드부터 해제 
	{
		if ((*srcRootNode)->_left != NULL)
			BST_DeallocateTree(&((*srcRootNode)->_left));

		if ((*srcRootNode)->_right != NULL)
			BST_DeallocateTree(&((*srcRootNode)->_right));

		BST_DeallocateNode(srcRootNode);
		(*srcRootNode) = NULL;
	}
}

/// <summary>
/// 순회 방법에 따른 대상 트리 출력
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <param name="traversalMethod">순회 방법</param>
void BST_DispOrderedTree(NODE* srcRootNode, TRAVERSAL_METHOD traversalMethod)
{
	if (srcRootNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	switch (traversalMethod)
	{
	case TRAVERSAL_METHOD::PREORDER:
		std::cout << srcRootNode->_data << " ";

		if (srcRootNode->_left != NULL)
			BST_DispOrderedTree(srcRootNode->_left, traversalMethod);

		if (srcRootNode->_right != NULL)
			BST_DispOrderedTree(srcRootNode->_right, traversalMethod);
		break;

	case TRAVERSAL_METHOD::INORDER:
		if (srcRootNode->_left != NULL)
			BST_DispOrderedTree(srcRootNode->_left, traversalMethod);

		std::cout << srcRootNode->_data << " ";

		if (srcRootNode->_right != NULL)
			BST_DispOrderedTree(srcRootNode->_right, traversalMethod);
		break;

	case TRAVERSAL_METHOD::POSTORDER:
		if (srcRootNode->_left != NULL)
			BST_DispOrderedTree(srcRootNode->_left, traversalMethod);

		if (srcRootNode->_right != NULL)
			BST_DispOrderedTree(srcRootNode->_right, traversalMethod);

		std::cout << srcRootNode->_data << " ";
		break;
	}
}

/// <summary>
/// 대상 트리에 새 노드 삽입
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <param name="srcNewNode">새 노드</param>
void BST_InsertNode(NODE** srcRootNode, NODE* srcNewNode)
{
	if (srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if ((*srcRootNode) == NULL) //삽입 할 적절한 위치인 경우
	{
		(*srcRootNode) = srcNewNode;
	}
	else
	{
		if ((*srcRootNode)->_data == srcNewNode->_data) //중복 데이터가 존재 할 경우
			throw myexception::NOT_ALLOWED_DUPLICATE_DATA_EXCEPTION(std::string(__func__) + std::string(" : Not allowed duplicate data"));

		if ((*srcRootNode)->_data > srcNewNode->_data) //현재 노드의 데이터 > 삽입하고자 하는 노드의 데이터인 경우
			BST_InsertNode(&((*srcRootNode)->_left), srcNewNode); //현재 노드 기준 왼쪽으로 탐색 수행
		else //현재 노드의 데이터 < 삽입하고자 하는 노드의 데이터인 경우
			BST_InsertNode(&((*srcRootNode)->_right), srcNewNode); //현재 노드 기준 오른쪽으로 탐색 수행
	}
}

/// <summary>
/// 대상 트리에 삭제하고자 하는 대상 데이터가 포함 된 노드 삭제
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <param name="targetData">삭제하고자 하는 대상 데이터</param>
/// <param name="deallocateAfterRemove">삭제 대상 노드에 대한 메모리 해제 수행 여부</param>
/// <summary>
void BST_RemoveNode(NODE** srcRootNode, const DATA_TYPE& targetData, bool deallocateAfterRemove)
{
	/***
		< 삭제하고자 하는 대상 데이터가 포함 된 노드 (삭제 대상 노드)의 자식 노드 유무에 따른 처리 >

		! 루트 노드에 대한 삭제가 발생 할 경우, 별도의 처리 요구

		1) 삭제 대상 노드가 자식 노드를 갖고 있지 않을 경우

			1-1) 삭제 대상 노드가 루트 노드일 경우
			: do nothing

			1-2) 삭제 대상 노드가 루트 노드가 아닐 경우
			: 삭제 대상 노드의 부모 노드로부터 연결 해제

		2) 삭제 대상 노드가 왼쪽, 오른쪽 자식 노드 중 하나를 갖고 있을 경우

			2-1) 삭제 대상 노드가 루트 노드일 경우
			: 삭제 대상 노드가 가지고 있던 자식 노드를 최상위 루트 노드로 변경

			2-2) 삭제 대상 노드가 루트 노드가 아닐 경우
			: 삭제 대상 노드가 가지고 있던 자식 노드를 삭제 대상 노드가 연결 되었던 상위 부모 노드로 연결

		3) 삭제 대상 노드가 왼쪽, 오른쪽 자식 노드를 모두 갖고 있을 경우

			3-1) 삭제 대상 노드가 루트 노드일 경우

				3-1-1) 삭제 대상 노드의 오른쪽 하위 트리의 노드 중 가장 작은 노드 (이하, 이동 대상 노드) 를 최상위 루트 노드로 변경
				: 이동 대상 노드는 해당 하위 트리에서 가장 작은 노드이므로, 왼쪽 자식 노드가 존재하지 않음

			3-2) 삭제 대상 노드가 루트 노드가 아닐 경우

				3-2-1) 이동 대상 노드를 삭제 대상 노드가 연결 되었던 상위 부모 노드로 연결
				: 이동 대상 노드는 해당 하위 트리에서 가장 작은 노드이므로, 왼쪽 자식 노드가 존재하지 않음

			3-3) 이동 대상 노드의 남아있는 오른쪽 하위 트리 존재 여부에 따라,

				3-3-1) 이동 대상 노드의 오른쪽 하위 트리가 존재 할 경우
				: 이동 대상 노드의 오른쪽 하위 트리의 루트 노드를 이동 대상 노드가 이동이 발생하기 전 연결되었던 상위 부모 노드로 연결

				3-3-2) 이동 대상 노드의 오른쪽 하위 트리가 존재하지 않을 경우
				: 이동 대상 노드가 이동이 발생하기 전 연결 되었던 상위 부모 노드로부터의 연결 해제

			3-4) 이동 대상 노드의 왼쪽 및 오른쪽 자식 노드를 삭제 대상 노드의 왼쪽 및 오른쪽 자식 노드로 연결

		4) 삭제 대상 노드 삭제

		---
		ex) 이진 탐색 트리의 삭제 연산

										23 (Root)
					10 (Remove Target)					100
			1				16					50
		0		9		12		20

		10이 삭제 대상 노드일 경우, 10의 부모 노드인 23의 왼쪽에는 23보다 작은 노드가 와야하며,
		하위 트리도 이진 탐색 트리의 정의를 만족해야 함

		1) 삭제 대상 노드를 삭제 후 하위 트리의 노드 중 가장 작은 노드인 0을 삭제 대상 노드의 위치로 옮길 경우

					0
			1				16
				9		12		20

		=> 이진 탐색 트리의 정의를 만족하지 않음

		2) 삭제 대상 노드를 삭제 후 하위 트리의 노드 중 가장 큰 노드인 20을 삭제 대상 노드의 위치로 옮길 경우

					20
			1				16
		0		9		12

		=> 이진 탐색 트리의 정의를 만족하지 않음

		3) 삭제 대상 노드를 삭제 후 왼쪽 하위 트리의 노드 중 가장 큰 노드인 9을 삭제 대상 노드의 위치로 옮길 경우

					9
			1				16
		0				12		20

		=> 이진 탐색 트리의 정의를 만족

		4) 삭제 대상 노드를 삭제 후 오른쪽 하위 트리의 노드 중 가장 작은 노드인 12을 삭제 대상 노드의 위치로 옮길 경우


					12
			1				16
		0		9				20

		=> 이진 탐색 트리의 정의를 만족
	***/

	if ((*srcRootNode) == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	NODE* removeTargetNode; //삭제 대상 노드
	NODE* removeTargetParentNode; //삭제 대상 노드의 부모 노드
	NODE** removeTargetParentToChildConnection; //삭제 대상 노드의 부모 노드에서 삭제 대상 노드로의 연결
	std::tie(removeTargetNode, removeTargetParentNode, removeTargetParentToChildConnection) = 
		BST_SearchNodeWithParentNode((*srcRootNode), targetData);

	if (removeTargetNode->_left == NULL && removeTargetNode->_right == NULL) //삭제 대상 노드가 자식 노드를 갖고 있지 않을 경우
	{
		if (removeTargetNode != (*srcRootNode)) //삭제 대상 노드가 루트 노드가 아닐 경우
			(*removeTargetParentToChildConnection) = NULL; //삭제 대상 노드의 부모 노드로부터 연결 해제
	}
	else if ((removeTargetNode->_left != NULL && removeTargetNode->_right == NULL) ||
		(removeTargetNode->_left == NULL && removeTargetNode->_right != NULL)) //삭제 대상 노드가 왼쪽, 오른쪽 자식 노드 중 하나를 갖고 있을 경우
	{
		if (removeTargetNode == (*srcRootNode)) //삭제 대상 노드가 루트 노드일 경우
		{
			//삭제 대상 노드가 가지고 있던 자식 노드를 최상위 루트 노드로 변경
			(removeTargetNode->_left != NULL) ?
				(*srcRootNode) = removeTargetNode->_left :
				(*srcRootNode) = removeTargetNode->_right;
		}
		else //삭제 대상 노드가 루트 노드가 아닐 경우
		{
			//삭제 대상 노드가 가지고 있던 자식 노드를 삭제 대상 노드가 연결 되었던 상위 부모 노드로 연결
			(removeTargetNode->_left != NULL) ?
				(*removeTargetParentToChildConnection) = removeTargetNode->_left :
				(*removeTargetParentToChildConnection) = removeTargetNode->_right;
		}
	}
	else //삭제 대상 노드가 왼쪽, 오른쪽 자식 노드를 모두 갖고 있을 경우
	{
		NODE* moveTargetNode; //이동 대상 노드 (삭제 대상 노드의 오른쪽 하위 트리의 노드 중 가장 작은 노드)
		NODE* moveTargetParentNode; //이동 대상 노드의 부모 노드
		NODE** moveTargetParentToChildConnection; //이동 대상 노드의 부모 노드에서 이동 대상 노드로의 연결
		std::tie(moveTargetNode, moveTargetParentNode, moveTargetParentToChildConnection) = 
			BST_SearchMinNodeWithParentNode(removeTargetNode->_right);

		if (removeTargetNode == (*srcRootNode)) //삭제 대상 노드가 루트 노드일 경우
		{
			//이동 대상 노드를 최상위 루트 노드로 변경
			(*srcRootNode) = moveTargetNode;
		}
		else //삭제 대상 노드가 루트 노드가 아닐 경우
		{
			//이동 대상 노드를 삭제 대상 노드가 연결 되었던 상위 부모 노드로 연결
			(*removeTargetParentToChildConnection) = moveTargetNode;
		}

		if (moveTargetNode->_right != NULL) //이동 대상 노드의 오른쪽 하위 트리가 존재 할 경우
		{
			//이동 대상 노드의 오른쪽 하위 트리의 루트 노드를 이동 대상 노드가 이동이 발생하기 전 연결되었던 상위 부모 노드로 연결
			(*moveTargetParentToChildConnection) = moveTargetNode->_right;
		}
		else //이동 대상 노드의 오른쪽 하위 트리가 존재하지 않을 경우
		{
			//이동 대상 노드가 이동이 발생하기 전 연결 되었던 상위 부모 노드로부터의 연결 해제
			(*moveTargetParentToChildConnection) = NULL;
		}

		//이동 대상 노드의 왼쪽 및 오른쪽 자식 노드를 삭제 대상 노드의 왼쪽 및 오른쪽 자식 노드로 연결
		moveTargetNode->_left = removeTargetNode->_left;
		moveTargetNode->_right = removeTargetNode->_right;
	}

	if (deallocateAfterRemove)
		BST_DeallocateNode(&removeTargetNode);
}

/// <summary>
/// 대상 트리에 찾고자 하는 대상 데이터가 포함 된 노드 반환
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <param name="targetData">찾고자 하는 대상 데이터</param>
/// <returns>찾고자 하는 대상 데이터가 포함 된 노드</returns>
NODE* BST_SearchNode(NODE* srcRootNode, const DATA_TYPE& targetData)
{
	if (srcRootNode == NULL)
		throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

	if (srcRootNode->_data == targetData) //현재 노드가 찾고자 하는 대상 데이터와 일치할 경우
		return srcRootNode;
	else if (srcRootNode->_data > targetData) //현재 노드 > 찾고자 하는 대상 데이터인 경우
		return BST_SearchNode(srcRootNode->_left, targetData); //현재 노드 기준 왼쪽으로 탐색 수행
	else //현재 노드 < 찾고자 하는 대상 데이터인 경우
		return BST_SearchNode(srcRootNode->_right, targetData); //현재 노드 기준 오른쪽으로 탐색 수행
}

/// <summary>
/// 대상 트리에 찾고자 하는 대상 데이터가 포함 된 노드와 해당 노드의 상위 부모 노드 및 자식 노드로의 연결이 포함 된 튜플 반환
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <param name="targetData">찾고자 하는 대상 데이터</param>
/// <param name="optionalTargetParentNode">찾고자 하는 대상 데이터가 포함 된 노드의 상위 부모 노드</param>
/// <returns>대상 데이터가 포함 된 노드와 해당 노드의 상위 부모 노드가 포함 된 튜플
/// <para>tuple arg 0 : 대상 데이터가 포함 된 노드</para>
/// <para>tuple arg 1 : 해당 노드의 상위 부모 노드 (존재하지 않을 경우 NULL)</para>
/// <para>tuple arg 2 : 해당 노드의 상위 부모 노드에서 대상 데이터가 포함 된 노드로의 연결 (존재하지 않을 경우 NULL)</para></returns>
std::tuple<NODE*, NODE*, NODE**> BST_SearchNodeWithParentNode(NODE* srcRootNode, const DATA_TYPE& targetData, NODE* optionalTargetParentNode)
{
	if (srcRootNode == NULL)
		throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

	if (srcRootNode->_data == targetData) //현재 노드가 찾고자 하는 대상 데이터와 일치할 경우
	{
		NODE** targetParentToChildConnection =
			(optionalTargetParentNode == NULL) ? NULL :
			(optionalTargetParentNode->_left == srcRootNode) ? 
			&(optionalTargetParentNode->_left) : &(optionalTargetParentNode->_right); //상위 부모 노드에서 자식 노드로의 연결

		return std::make_tuple(srcRootNode, optionalTargetParentNode, targetParentToChildConnection);
	}

	if (srcRootNode->_data > targetData) //현재 노드 > 찾고자 하는 대상 데이터인 경우
		return BST_SearchNodeWithParentNode(srcRootNode->_left, targetData, srcRootNode); //현재 노드 기준 왼쪽으로 탐색 수행
	else //현재 노드 < 찾고자 하는 대상 데이터인 경우
		return BST_SearchNodeWithParentNode(srcRootNode->_right, targetData, srcRootNode); //현재 노드 기준 오른쪽으로 탐색 수행
}

/// <summary>
/// 대상 트리의 최소값인 데이터가 포함 된 노드 반환
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <returns>최소값인 데이터가 포함 된 노드</returns>
NODE* BST_SearchMinNode(NODE* srcRootNode)
{
	if (srcRootNode == NULL)
		throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

	if (srcRootNode->_left != NULL) //현재 노드의 왼쪽 하위 트리가 존재 할 경우
		return BST_SearchMinNode(srcRootNode->_left);
	else //현재 노드의 왼쪽 하위 트리가 존재하지 않을 경우
		return srcRootNode;
}

/// <summary>
/// 대상 트리의 최소값인 데이터가 포함 된 노드와 해당 노드의 상위 부모 노드 및 자식 노드로의 연결이 포함 된 튜플 반환
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <param name="optionalTargetParentNode">대상 트리의 최소값인 데이터가 포함 된 노드의 상위 부모 노드</param>
/// <returns>최소값인 데이터가 포함 된 노드와 해당 노드의 상위 부모 노드가 포함 된 튜플
/// <para>tuple arg 0 : 최소값인 데이터가 포함 된 노드</para>
/// <para>tuple arg 1 : 해당 노드의 상위 부모 노드 (존재하지 않을 경우 NULL)</para>
/// <para>tuple arg 2 : 해당 노드의 상위 부모 노드에서 대상 데이터가 포함 된 노드로의 연결 (존재하지 않을 경우 NULL)</para></returns>
std::tuple<NODE*, NODE*, NODE**> BST_SearchMinNodeWithParentNode(NODE* srcRootNode, NODE* optionalTargetParentNode)
{
	if (srcRootNode == NULL)
		throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

	if (srcRootNode->_left != NULL) //현재 노드의 왼쪽 하위 트리가 존재 할 경우
	{
		return BST_SearchMinNodeWithParentNode(srcRootNode->_left, srcRootNode);
	}
	else //현재 노드의 왼쪽 하위 트리가 존재하지 않을 경우
	{
		NODE** targetParentToChildConnection =
			(optionalTargetParentNode == NULL) ? NULL :
			(optionalTargetParentNode->_left == srcRootNode) ? 
			&(optionalTargetParentNode->_left) : &(optionalTargetParentNode->_right); //상위 부모 노드에서 자식 노드로의 연결

		return std::make_tuple(srcRootNode, optionalTargetParentNode, targetParentToChildConnection);
	}
}