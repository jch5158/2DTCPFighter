#pragma once

class CPlayerObject : public CBaseObject
{
public:
		
	void Update();

	void Render();

	void InputActionProc();

	void ActionProc();

	void SetHp(BYTE Hp);

	CPlayerObject();

	~CPlayerObject();

	
	// ���� �Է¹��� �޽���
	DWORD m_dwActionCur;

};

