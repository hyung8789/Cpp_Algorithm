#ifndef _EXPRT_H_
#define _EXPRT_H_

/***
	! 이하, 후위 표현식으로부터 수식 트리를 구축하는 과정의 기능 단위 분류에 따라,

		F1) 후위 표현식에 대한 순차적인 토큰 생성
		F2) 생성 된 토큰으로부터 순차적인 수식 트리 구축
		F3) 구축 된 수식 트리로부터 계산 수행

	=> 구축 된 수식 트리로부터 계산이 수행되는 기능은 수식 트리에 의존적이므로,
	수식 트리 구축과 계산이 수행되는 모듈과 후위 표현식으로부터 순차적인 토큰이 생성되는 모듈을 별도로 분리 할 것
***/

typedef char* DATA_TYPE; //노드의 데이터 타입
typedef int TREE_DEPTH_TYPE; //트리 깊이 타입

typedef struct NODE_TYPE
{
	DATA_TYPE _data; //노드의 데이터

	NODE_TYPE* _left; //왼쪽 노드
	NODE_TYPE* _right; //오른쪽 노드
}NODE;

enum class TRAVERSAL_METHOD : const int
{
	PREORDER = 0, //전위 순회 (Root -> Left -> Right)
	INORDER, //중위 순회 (Left -> Root -> Right)
	POSTORDER //후위 순회 (Left -> Right -> Root)
};

double CalcOperation(double, char, double);
double CalcOperation(double, SYMBOL_TYPE, double);

NODE* EXPRT_CreateNode(DATA_TYPE);
void EXPRT_DeallocateNode(NODE**);
void EXPRT_DeallocateTree(NODE**);

void EXPRT_BulidTreeFromPostfixExpr(NODE**, char[]);
double EXPRT_EvaluateTree(NODE*);

void EXPRT_DispOrderedTree(NODE*, TRAVERSAL_METHOD, std::ostream& = std::cout);
#endif