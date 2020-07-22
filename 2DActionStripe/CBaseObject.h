#pragma once
class CBaseObject
{
public:

	
	CBaseObject();

	~CBaseObject();
	
	unsigned int GetX();

	unsigned int GetY();

	// KeyProcess() 에서 입력값을 받습니다.
    // 어떤 액션을 할지 캐릭터 객체 멤버변수에 셋팅해준다. 
	void ActionInput(DWORD dwAction);

	// 캐릭터의 위치를 셋팅해주는 함수입니다.
	void SetPosition(int iXpos, int iYpos);

	void SetSprite(DWORD dwSpriteStart, DWORD dwSpriteMax, DWORD dwFrameDelay);

	int NextFrame();

	//int GetObjectID();

	//int GetObjectType();

	//int GetSprite();

	//int IsEndFrame();
	
	virtual void Render() = 0;

	virtual void Update() = 0;



public:

	// 해당 프레임이 true일 경우 다시 처음 스프라이트로 돌아갑니다.
	bool m_bEndFrame;


	// 사용자 입력값 저장하는 멤버변수.
	DWORD m_ActionInput;

	unsigned int m_iXpos;

	unsigned int m_iYpos;

	// 딜레이 카운트가 어느정도 되었는지 프레임 딜레이와의 체크
	DWORD m_dwDelayCount;

	// 프레임별로 딜레이 값을 넣어줍니다.
	DWORD m_dwFrameDelay;


	// 스프라이트 시작 값
	DWORD m_dwSpriteStart;

	// 현재 스프라이트 인덱스 값
	DWORD m_dwSpriteNow;

	// 끝 스프라이트 값
	DWORD m_dwSpriteEnd;
	
	// 오브젝트 ID
	DWORD m_dwObjectID;

	// 적군일지 아군일지 
	DWORD m_ObjectType;

	// 방향 오른쪽 디폴트
	DWORD m_dwDirCur;

	// 예전 방향
	DWORD m_dwDirOld;

	// 캐릭터 hp
	unsigned char m_chHP;

};

// 백 버퍼의 가로 세로 길이 그리고 32 컬러비트
extern CScreenDib ScreenDib;

// 배열 최대개수와 컬러키 값 ARGB  -> 리틀 엔디안으로 BGRA
extern CSpriteDib SpriteDib;

