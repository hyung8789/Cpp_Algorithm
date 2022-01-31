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

typedef char* DataType; //노드의 데이터 타입
typedef int TreeDepthType; //트리 깊이 타입

typedef struct NodeType
{
	DataType data; //노드의 데이터
	NodeType* left; //왼쪽 노드
	NodeType* right; //오른쪽 노드
}Node;

enum class TRAVERSAL_MODE : const int
{
	PREORDER = 0, //전위 순회 (Root -> Left -> Right)
	INORDER, //중위 순회 (Left -> Root -> Right)
	POSTORDER //후위 순회 (Left -> Right -> Root)
};

double CalcOperation(double, char, double);
double CalcOperation(double, SYMBOL_TYPE, double);

Node* EXPRT_CreateNode(DataType);
void EXPRT_DeallocateNode(Node**);
void EXPRT_DeallocateTree(Node**);

void EXPRT_BulidTreeFromPostfixExpr(Node**, char[]);
double EXPRT_EvaluateTree(Node*);

void EXPRT_DispOrderedTree(Node*, TRAVERSAL_MODE, std::ostream& = std::cout);
#endif