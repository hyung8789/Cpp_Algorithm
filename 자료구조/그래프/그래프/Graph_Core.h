#ifndef _GRAPH_CORE_H_
#define _GRAPH_CORE_H_

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <stdio.h>
#include <iostream>

#include <stdexcept>

//해당 프로젝트 속성에서 정적 라이브러리 (lib)로 빌드 할 것
#include "../../../common_lib/common_lib/Common_LIB_Core.h"

#include "Graph_Adj_Matrix.h"
#include "Graph_Adj_List.h"

/***
	- 정점 (Vertex) : 그래프가 형성되는 기본 단위로서, 하나의 개체
	- 간선 (Edge) : 정점과 정점 간의 연결 (선)
	- 인접 (Adjacent) : 간선으로 연결되어 있는 두 정점
	- 경로 (Path) : 정점과 정점 간에 연결 된 길
	- 사이클 : 순환적인 경로 (출발점으로 되돌아 오는 경로가 존재 할 경우)
	- 연결성 : 두 정점 사이에 경로가 존재 할 경우 두 정점 간에 연결성이 존재 혹은
	그래프 내의 각 정점들이 다른 모든 정점들과 연결되어 있으면 그래프는 연결성이 존재
***/

// https://en.wikipedia.org/wiki/Graph_(abstract_data_type)
#endif