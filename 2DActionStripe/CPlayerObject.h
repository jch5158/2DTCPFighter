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

	
	// 지금 입력받은 메시지
	DWORD m_dwActionCur;

};

