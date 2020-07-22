#pragma once
class CBaseObject
{
public:

	
	CBaseObject();

	~CBaseObject();
	
	unsigned int GetX();

	unsigned int GetY();

	// KeyProcess() ���� �Է°��� �޽��ϴ�.
    // � �׼��� ���� ĳ���� ��ü ��������� �������ش�. 
	void ActionInput(DWORD dwAction);

	// ĳ������ ��ġ�� �������ִ� �Լ��Դϴ�.
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

	// �ش� �������� true�� ��� �ٽ� ó�� ��������Ʈ�� ���ư��ϴ�.
	bool m_bEndFrame;


	// ����� �Է°� �����ϴ� �������.
	DWORD m_ActionInput;

	unsigned int m_iXpos;

	unsigned int m_iYpos;

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
	
	// ������Ʈ ID
	DWORD m_dwObjectID;

	// �������� �Ʊ����� 
	DWORD m_ObjectType;

	// ���� ������ ����Ʈ
	DWORD m_dwDirCur;

	// ���� ����
	DWORD m_dwDirOld;

	// ĳ���� hp
	unsigned char m_chHP;

};

// �� ������ ���� ���� ���� �׸��� 32 �÷���Ʈ
extern CScreenDib ScreenDib;

// �迭 �ִ밳���� �÷�Ű �� ARGB  -> ��Ʋ ��������� BGRA
extern CSpriteDib SpriteDib;

