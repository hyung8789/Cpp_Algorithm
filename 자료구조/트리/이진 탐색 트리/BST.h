#ifndef _BST_H_
#define _BST_H_

/***
	< 이진 탐색 트리 >

	1) 루트 노드 혹은 서브 트리 내의 루트 노드는 항상 중앙값을 가짐
	: 루트 노드의 왼쪽 서브 트리 < 루트 노드 < 루트 노드의 오른쪽 서브 트리

	2) 이진 탐색 트리의 정의에 따라, 중복 된 데이터를 허용하지 않음

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

void BST_InsertNode(Node**, Node*);
void BST_RemoveNode(Node**, const DataType&, bool = true);

Node* BST_SearchNode(Node*, const DataType&);
std::tuple<Node*, Node*> BST_SearchNodeWithParentNode(Node*, const DataType&, Node* = NULL);

Node* BST_SearchMinNode(Node*);
std::tuple<Node*, Node*> BST_SearchMinNodeWithParentNode(Node*, Node* = NULL);
#endif