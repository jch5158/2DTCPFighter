#pragma once

class CPlayerObject : public CBaseObject
{
public:
		
	void Update();

	void Render();

	void InputActionProc();

	void ActionProc();

	void SetHp(DWORD Hp);

	CPlayerObject();

	~CPlayerObject();

	// ĳ���� hp
	unsigned char m_chHP;
	
	// ���� �Է¹��� �޽���
	DWORD m_dwActionCur;


	// ���� ������ ����Ʈ
	DWORD m_dwDirCur;

	// ���� ����
	DWORD m_dwDirOld;

};

