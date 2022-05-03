﻿#ifndef _DS_H_
#define _DS_H_

// https://www.geeksforgeeks.org/union-find/

/// <summary>
/// 분리 집합의 노드
/// </summary>
typedef struct NODE_TYPE
{
	void* _data; //노드의 데이터

	NODE_TYPE* _parent; //한 단계 상위 부모 노드
}NODE;

NODE* DS_CreateNode(void*);
void DS_DeallocateNode(NODE**);

void DS_UnionSet(NODE**, NODE*);
NODE* DS_FindSet(NODE*);
#endif