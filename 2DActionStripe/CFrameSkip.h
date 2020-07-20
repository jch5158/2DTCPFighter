#pragma once
class CFrameSkip
{
public:

	CFrameSkip();

	~CFrameSkip();
	
	//20
	DWORD m_dwMaxFPS;


	// 업데이트 지연될 때 마다 더해지는 멤버변수
	DWORD m_dwOneFrameTime;	


	
	bool FrameSkip();



	bool UpdateCheck(HWND hWnd);

};

