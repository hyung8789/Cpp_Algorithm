#include "RBT_Core.h"

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

		1) 대상 부모 노드의 왼쪽 자식 노드의 오른쪽 자식 노드를 대상 부모 노드의 왼쪽 자식으로 연결
		: 대상 부모 노드의 왼쪽 자식 노드 < 대상 부모 노드의 왼쪽 자식 노드의 오른쪽 자식 노드 < 대상 부모 노드

		2) 대상 부모 노드의 왼쪽 자식 노드의 부모를 대상 부모 노드의 부모로 연결

		3) 대상 부모 노드의 부모를 대상 부모 노드의 왼쪽 자식 노드로 연결

		---

		< 트리 회전 - 좌회전>
		
		1) 대상 부모 노드의 오른쪽 자식 노드의 왼쪽 자식 노드를 대상 부모 노드의 오른쪽 자식으로 연결
		: 대상 부모 노드 < 대상 부모 노드의 오른쪽 자식 노드의 왼쪽 자식 노드 < 대상 부모 노드의 오른쪽 자식 노드

		2) 대상 부모 노드의 오른쪽 자식 노드의 부모를 대상 부모 노드의 부모로 연결

		3) 대상 부모 노드의 부모를 대상 부모 노드의 오른쪽 자식 노드로 연결

		---

					8
			5				9
		3		6

		<= 5를 기준으로 좌회전 후,
		=> 8을 기준으로 우회전 후,

					5
			3				8
						6		9
	***/


	Node* moveTargetChildNode = NULL; //회전을 위해 이동 될 자식 노드
	Node** moveTargetParentToChildConnection = NULL;

}