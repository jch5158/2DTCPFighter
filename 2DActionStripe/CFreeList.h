//#pragma once
//
//template <class DATA>
//class CFreeList
//{
//private:
//	struct stBlockNode
//	{		
//		stBlockNode()
//		{
//			this->stpNextBlock = nullptr;
//		}
//
//		// 언더플로우를 체크할 체크섬입니다.
//		int underCheckSum;
//
//		// 오브젝트의 데이터입니다.
//		DATA data;
//
//		// 오버플로우를 체크할 체크섬입니다.
//		int overCheckSum;
//
//		// ~CFreeList() 소멸자에서 해당 리스트를 순회해서 Relese 
//		stBlockNode* stpNextAlloc;
//
//		// 다음 노드를 체크할 멤버변수입니다. 
//		stBlockNode* stpNextBlock;
//	};
//
//
//public:
//
//	CFreeList(int iBlockNum, bool bPlacementNew = false)
//	{
//		// ~CFreeList() 소멸자에서 해당 리스트를 순회해서 Relese 
//		this->m_pAllocNodeTop = nullptr;
//
//		// Alloc()을 위한 리스트
//		this->m_pFreeNode = nullptr;
//	
//		this->m_bPlaceMentNew = bPlacementNew;
//
//		this->m_iBlockCount = iBlockNum;
//
//		for (int iCnt = 0; iCnt < iBlockNum; ++iCnt)
//		{
//			stBlockNode* blockPtr = (stBlockNode*)malloc(sizeof(stBlockNode));
//
//			blockPtr->stpNextBlock = this->m_pFreeNode;
//
//			blockPtr->stpNextAlloc = this->m_pAllocNodeTop;
//
//			this->m_pAllocNodeTop = blockPtr;
//
//			this->m_pFreeNode = blockPtr;
//		}
//	}
//
//	virtual ~CFreeList()
//	{
//		while (1)
//		{
//			if (this->m_pAllocNodeTop == nullptr)
//			{
//				break;
//			}
//			// 해제할 포인터를 ptrBuffer에 담는다.
//			stBlockNode* ptrBuffer = this->m_pAllocNodeTop;
//
//			// ptrBuffer에 다음 노드 값을 top 노드로 바꾼다.
//			this->m_pAllocNodeTop = ptrBuffer->stpNextAlloc;
//
//			// 해제
//			free(ptrBuffer);
//		}
//	}
//
//	DATA* Alloc(void)
//	{
//
//	}
//
//	bool Free(DATA* tData)
//	{
//
//	}
//
//	// 현재 확보 된 블럭의 전체 개수
//	int GetAllocCount(void)
//	{
//		return this->m_iAllocCount;
//	}
//
//	// 현재 사용중인 블럭의 개수
//	int	GetUseCount(void)
//	{
//		return this->m_iBlockCount;
//	}
//
//	// placement new를 호출할지 말지를 셋팅
//	bool m_bPlaceMentNew;
//
//	// 총 확보된 노드 수
//	int m_iAllocCount
//
//	// 사용중인 노드 수
//	int m_iBlockCount;
//
//	// ~CFreeList() 소멸자에서 해당 리스트를 순회해서 Relese 
//	stBlockNode* m_pAllocNodeTop;
//
//	// 스택 방식으로 반환된 (미사용) 오브젝트 블럭을 관리. 탑 노드역할을 할 프리노드
//	stBlockNode* m_pFreeNode;
//
//};
//
