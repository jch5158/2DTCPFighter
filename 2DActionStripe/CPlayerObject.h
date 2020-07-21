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

	// 캐릭터 hp
	unsigned char m_chHP;
	
	// 지금 입력받은 메시지
	DWORD m_dwActionCur;


	// 방향 오른쪽 디폴트
	DWORD m_dwDirCur;

	// 예전 방향
	DWORD m_dwDirOld;

};

