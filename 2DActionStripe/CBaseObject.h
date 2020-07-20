#pragma once
class CBaseObject
{
public:

	
	CBaseObject();

	~CBaseObject();
	
	int GetX();

	int GetY();

	// KeyProcess() ���� �Է°��� �޽��ϴ�.
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

	// �ش� �������� true�� ��� �ٽ� ó�� ��������Ʈ�� ���ư��ϴ�.
	bool m_bEndFrame;


	// ����� �Է°� �����ϴ� �������.
	DWORD m_ActionInput;

	int m_iXpos;

	int m_iYpos;

	// ������ ī��Ʈ�� ������� �Ǿ����� ������ �����̿��� üũ
	DWORD m_dwDelayCount;

	// �����Ӻ��� ������ ���� �־��ݴϴ�.
	DWORD m_dwFrameDelay;


	// ��������Ʈ ���� ��
	DWORD m_dwSpriteStart;

	// ���� ��������Ʈ �ε��� ��
	DWORD m_dwSpriteNow;

	// �� ��������Ʈ ��
	DWORD m_dwSpriteEnd;
	
	DWORD m_dwObjectID;

	//
	DWORD m_ObjectType;
};

// �� ������ ���� ���� ���� �׸��� 32 �÷���Ʈ
extern CScreenDib ScreenDib;

// �迭 �ִ밳���� �÷�Ű �� ARGB  -> ��Ʋ ��������� BGRA
extern CSpriteDib SpriteDib;

