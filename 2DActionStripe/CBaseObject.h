#pragma once
class CBaseObject
{
public:

	
	CBaseObject();

	~CBaseObject();
	
	int GetX();

	int GetY();

	// KeyProcess() 에서 입력값을 받습니다.
	void ActionInput(DWORD dwAction);

	void SetSprite(DWORD dwSpriteStart, DWORD dwSpriteMax, DWORD dwFrameDelay);

	int NextFrame();

	//int GetObjectID();

	//int GetObjectType();

	//int GetSprite();

	//int IsEndFrame();
	
	virtual void Render() = 0;

	virtual void Update() = 0;

	void SetPosition(int iXpos,int iYpos);


public:

	// 해당 프레임이 true일 경우 다시 처음 스프라이트로 돌아갑니다.
	bool m_bEndFrame;


	// 사용자 입력값 저장하는 멤버변수.
	DWORD m_ActionInput;

	int m_iXpos;

	int m_iYpos;

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
	
	DWORD m_dwObjectID;

	//
	DWORD m_ObjectType;
};

// 백 버퍼의 가로 세로 길이 그리고 32 컬러비트
extern CScreenDib ScreenDib;

// 배열 최대개수와 컬러키 값 ARGB  -> 리틀 엔디안으로 BGRA
extern CSpriteDib SpriteDib;

