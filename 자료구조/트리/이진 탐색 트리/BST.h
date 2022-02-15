#ifndef _BST_H_
#define _BST_H_

/***
	< 이진 탐색 트리 >

	루트 노드 혹은 서브 트리 내의 루트 노드는 항상 중앙값을 가짐
	루트 노드의 왼쪽 서브 트리 < 루트 노드 < 루트 노드의 오른쪽 서브 트리
***/

void BST_InsertNode(Node**, Node*);
void BST_RemoveNode(Node**, ...);
Node* BST_SearchNode(Node*, DataType);


#endif