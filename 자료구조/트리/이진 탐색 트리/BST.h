#ifndef _BST_H_
#define _BST_H_

/***
	< 이진 탐색 트리 >

	1) 루트 노드 혹은 서브 트리 내의 루트 노드는 항상 중앙값을 가짐
	: 루트 노드의 왼쪽 서브 트리 < 루트 노드 < 루트 노드의 오른쪽 서브 트리

	2) 이진 탐색 트리의 정의에 따라, 중복 된 데이터를 허용하지 않음
***/

void BST_InsertNode(Node**, Node*);
void BST_RemoveNode(Node**, const DataType&, bool = true);

Node* BST_SearchNode(Node*, const DataType&);
Node* BST_SearchTargetParentNode(Node*, const DataType&, Node* = NULL);
Node* BST_SearchMinNode(Node*);
#endif