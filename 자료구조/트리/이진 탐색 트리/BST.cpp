#include "BST_Core.h"

/// <summary>
/// 대상 이진 탐색 트리에 새 노드 삽입
/// </summary>
/// <param name="srcRootNode">대상 이진 탐색 트리의 최상위 루트 노드</param>
/// <param name="srcNewNode">새 노드</param>
void BST_InsertNode(Node** srcRootNode, Node* srcNewNode)
{
	if (srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if ((*srcRootNode) == NULL) //삽입 할 적절한 위치인 경우
	{
		(*srcRootNode) = srcNewNode;
	}
	else
	{
		if ((*srcRootNode)->data == srcNewNode->data) //중복 데이터가 존재 할 경우
			throw myexception::NOT_ALLOWED_DUPLICATE_DATA_EXCEPTION(std::string(__func__) + std::string(" : Not allowed duplicate data"));

		if ((*srcRootNode)->data > srcNewNode->data) //현재 노드의 데이터 > 삽입하고자 하는 노드의 데이터인 경우
			BST_InsertNode(&((*srcRootNode)->left), srcNewNode); //현재 노드 기준 왼쪽으로 탐색 수행
		else //현재 노드의 데이터 < 삽입하고자 하는 노드의 데이터인 경우
			BST_InsertNode(&((*srcRootNode)->right), srcNewNode); //현재 노드 기준 오른쪽으로 탐색 수행
	}
}

/// <summary>
/// 대상 이진 탐색 트리에 삭제하고자 하는 대상 데이터가 포함 된 노드 삭제
/// </summary>
/// <param name="srcRootNode">대상 이진 탐색 트리의 최상위 루트 노드</param>
/// <param name="targetData">삭제하고자 하는 대상 데이터</param>
/// <param name="deallocateAfterRemove">삭제 대상 노드에 대한 메모리 해제 수행 여부</param>
/// <summary>
void BST_RemoveNode(Node** srcRootNode, const DataType& targetData, bool deallocateAfterRemove)
{
	/***
		< 삭제하고자 하는 대상 데이터가 포함 된 노드 (삭제 대상 노드)의 자식 노드 유무에 따른 처리 >

		1) 삭제 대상 노드가 자식 노드를 갖고 있지 않을 경우

			1-1) 삭제 대상 노드의 부모 노드로부터 연결 해제

			1-2) 삭제 대상 노드 삭제

		2) 삭제 대상 노드가 왼쪽, 오른쪽 자식 노드 중 하나를 갖고 있을 경우

			2-1) 삭제 대상 노드의 부모 노드로부터 연결 해제

			2-2) 삭제 대상 노드가 가지고 있던 자식 노드를 삭제 대상 노드가 연결 되었던 상위 부모 노드로 연결

			2-3) 삭제 대상 노드 삭제

		3) 삭제 대상 노드가 왼쪽, 오른쪽 자식 노드를 모두 갖고 있을 경우

			3-1) 삭제 대상 노드의 부모 노드로부터 연결 해제

			3-2) 삭제 대상 노드의 오른쪽 하위 트리의 노드 중 가장 작은 노드 (이하, moveTarget) 를 삭제 대상 노드가 연결 되었던 상위 부모 노드로 연결
			: moveTarget은 해당 하위 트리에서 가장 작은 노드이므로, 왼쪽 자식 노드가 존재하지 않음

				3-2-1) moveTarget의 오른쪽 하위 트리가 존재 할 경우
				: moveTarget의 오른쪽 하위 트리의 루트 노드를 moveTarget의 이동 발생 전 부모 노드로 연결

				3-2-2) moveTarget의 오른쪽 하위 트리가 존재하지 않을 경우
				: do nothing
			
			3-3) 삭제 대상 노드 삭제

		---

		ex)
										23 (Root)
					10 (Remove Target)					100
			1				16					50
		0		9		12		20

		10이 삭제 대상 노드 일 경우, 20의 부모 노드인 50의 왼쪽에는 50보다 작은 노드가 와야하며,
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

	Node* removeTargetParentNode = BST_SearchTargetParentNode((*srcRootNode), targetData); //삭제 대상 노드의 부모 노드
	Node* removeTargetNode = 
		(removeTargetParentNode->left != NULL && removeTargetParentNode->left->data == targetData) ? removeTargetParentNode->left :
		(removeTargetParentNode->right != NULL && removeTargetParentNode->right->data == targetData) ? removeTargetParentNode->right : 
		throw std::logic_error(std::string(__func__) + std::string(" : Unknown err")); //삭제 대상 노드
	Node* moveTarget = BST_SearchMinNode(removeTargetNode->right); //삭제 대상 노드의 오른쪽 하위 트리의 노드 중 가장 작은 노드


	//TODO : 이하 수정 혹은 삭제 할 것
	if ((*srcRootNode)->data > targetData) //현재 노드의 데이터 > 삭제하고자 하는 대상 데이터인 경우
	{ 
		BST_RemoveNode(&((*srcRootNode)->left), targetData, (*srcRootNode), deallocateAfterRemove); //현재 노드 기준 왼쪽으로 탐색 수행
	} 
	else if((*srcRootNode)->data < targetData) //현재 노드의 데이터 < 삭제하고자 하는 대상 데이터인 경우
	{
		BST_RemoveNode(&((*srcRootNode)->right), targetData, (*srcRootNode), deallocateAfterRemove); //현재 노드 기준 오른쪽으로 탐색 수행
	}
	else //현재 노드의 데이터가 삭제하고자 하는 대상 데이터와 일치할 경우
	{
		Node* removeTargetNode = (*srcRootNode); //삭제 대상 노드
		//Node* removeTargetNodePrevParentChildNode = NULL; //삭제 대상 노드의 이전에 부모 노드에서 연결 되었던 자식 노드
		//TODO : 삭제 발생 후 트리의 루트 노드 변경 될 경우
		// 
		//삭제 대상 노드의 부모 노드로부터 연결 해제
		removeTargetNodePrevParentChildNode = 
			(removeTargetParentNode->left == removeTargetNode) ? removeTargetParentNode->left : removeTargetParentNode->right;

		if (removeTargetParentNode->left == removeTargetNode)
			removeTargetParentNode->left = NULL;
		else //right == removeTargetNode
			removeTargetParentNode->right = NULL;

		if (removeTargetNode->left != NULL && removeTargetNode->right != NULL) //삭제 대상 노드가 왼쪽, 오른쪽 자식 노드를 모두 갖고 있을 경우
		{
			Node* moveTarget = BST_SearchMinNode(removeTargetNode); //삭제 대상 노드의 오른쪽 하위 트리의 노드 중 가장 작은 노드
			//삭제 대상 노드가 연결 되었던 상위 부모 노드로 연결
				

		}

		//삭제 대상 노드 삭제
		if (deallocateAfterRemove)
			BT_DeallocateNode(&removeTargetNode);
	}
}

/// <summary>
/// 대상 이진 탐색 트리에 찾고자 하는 대상 데이터가 포함 된 노드 반환
/// </summary>
/// <param name="srcRootNode">대상 이진 탐색 트리의 최상위 루트 노드</param>
/// <param name="targetData">찾고자 하는 대상 데이터</param>
/// <returns>찾고자 하는 대상 데이터가 포함 된 노드</returns>
Node* BST_SearchNode(Node* srcRootNode, const DataType& targetData)
{
	if (srcRootNode == NULL)
		throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

	if (srcRootNode->data == targetData) //현재 노드가 찾고자 하는 대상 데이터와 일치할 경우
		return srcRootNode;
	else if (srcRootNode->data > targetData) //현재 노드 > 찾고자 하는 대상 데이터인 경우
		return BST_SearchNode(srcRootNode->left, targetData); //현재 노드 기준 왼쪽으로 탐색 수행
	else //현재 노드 < 찾고자 하는 대상 데이터인 경우
		return BST_SearchNode(srcRootNode->right, targetData); //현재 노드 기준 오른쪽으로 탐색 수행
}

/// <summary>
/// 대상 이진 탐색 트리에 찾고자 하는 대상 데이터가 포함 된 노드의 상위 부모 노드 반환
/// </summary>
/// <param name="srcRootNode">대상 이진 탐색 트리의 최상위 루트 노드</param>
/// <param name="targetData">찾고자 하는 대상 데이터</param>
/// <param name="targetParentNode">찾고자 하는 대상 데이터가 포함 된 노드의 상위 부모 노드</param>
/// <returns>찾고자 하는 대상 데이터가 포함 된 노드의 상위 부모 노드</returns>
Node* BST_SearchTargetParentNode(Node* srcRootNode, const DataType& targetData, Node* targetParentNode)
{
	if (srcRootNode == NULL)
		throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

	if (srcRootNode->data == targetData) //현재 노드가 찾고자 하는 대상 데이터와 일치할 경우
		return targetParentNode;
	else if (srcRootNode->data > targetData) //현재 노드 > 찾고자 하는 대상 데이터인 경우
		return BST_SearchTargetParentNode(srcRootNode->left, targetData, srcRootNode); //현재 노드 기준 왼쪽으로 탐색 수행
	else //현재 노드 < 찾고자 하는 대상 데이터인 경우
		return BST_SearchTargetParentNode(srcRootNode->right, targetData, srcRootNode); //현재 노드 기준 오른쪽으로 탐색 수행
}

/// <summary>
/// 대상 이진 탐색 트리의 최소값인 데이터가 포함 된 노드 반환
/// </summary>
/// <param name="srcRootNode">대상 이진 탐색 트리의 최상위 루트 노드</param>
/// <returns>최소값인 데이터가 포함 된 노드</returns>
Node* BST_SearchMinNode(Node* srcRootNode)
{
	if (srcRootNode == NULL)
		throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

	if (srcRootNode->left != NULL) //현재 노드의 왼쪽 하위 트리가 존재 할 경우
		return BST_SearchMinNode(srcRootNode->left);
	else //현재 노드의 왼쪽 하위 트리가 존재하지 않을 경우
		return srcRootNode;
}
