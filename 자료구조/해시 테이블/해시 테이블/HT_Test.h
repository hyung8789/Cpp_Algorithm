#ifndef _HT_TEST_H_
#define _HT_TEST_H_

class HASH_TABLE_TEST
{
public:
	//TODO : test function
	//TODO : HT_OpenAddr_DispIndexListBy, void HT_Chaining_DispNodeList(CHAINING_HASH_TABLE*),
	//HT_Chaining_DispEmptyIndexList(CHAINING_HASH_TABLE*), HT_Chaining_DispOccupiedIndexList(CHAINING_HASH_TABLE*); 에 대해 통합할 것


	HASH_TABLE_TEST();
	void Run();

private:
	CHAINING_HASH_TABLE* _chainingHashTable;
	OPEN_ADDR_HASH_TABLE* _openAddrHashTable;

private:
	bool _testChainingHashTable;
	bool _testOpenAddrHashTable;

	bool _dispNodeList;

	bool _dispIndexList;
	//NODE_STATE _dispIndexListBy;
};

#endif