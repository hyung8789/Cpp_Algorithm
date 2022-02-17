# My Library

## < Directory Structure Information >
    ./LIB_Core.h : 코어 헤더 파일
    ./Macro.h : 전처리 매크로 정의부
    ./MyException.h : 사용자 정의 예외 (myexception 네임스페이스로 접근)
    ./Utils.h : 기타 유틸 함수 선언부
    ./Utils.cpp : 기타 유틸 함수 정의부

---

## < 사용법 >

    1) 현재 프로젝트 (lib)의 속성에서 정적 라이브러리 (lib)로 빌드 할 것
    2) 사용하고자 하는 프로젝트에 현재 프로젝트 (lib)를 참조로 추가 할 것 
	3) 사용하고자 하는 프로젝트에 현재 프로젝트 (lib) 코어 헤더 파일 (LIB_Core.h)를 포함시킬 것