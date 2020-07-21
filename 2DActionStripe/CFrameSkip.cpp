#include <Windows.h>
#include "CFrameSkip.h"

CFrameSkip::CFrameSkip()
{
	this->m_dwMaxFPS = 20;

	this->m_dwOneFrameTime = 0;


}

CFrameSkip::~CFrameSkip()
{

}

// 로직 프레임 텍스트 출력
bool CFrameSkip::UpdateCheck(HWND hWnd) {

	static DWORD frameCheck = 50;
	
	static DWORD timeCheck = timeGetTime();

	static DWORD frameNow = 0;

	frameNow += 1;

	if (timeGetTime() - timeCheck >= 1000) {

		frameCheck = frameNow;

		timeCheck = timeGetTime();

		frameNow = 0;
	}

	WCHAR text[20];

	wsprintf(text, L"Logic : %d", frameCheck);

	SetWindowTextW(hWnd, text);

	return true;
}

bool CFrameSkip::FrameSkip()
{
	static DWORD oldTime = timeGetTime();

	DWORD nowTime = timeGetTime();
	
	this->m_dwOneFrameTime += (nowTime - oldTime);

	if (this->m_dwOneFrameTime < this->m_dwMaxFPS)
	{	
		Sleep(this->m_dwMaxFPS - this->m_dwOneFrameTime);

		oldTime = nowTime+(this->m_dwMaxFPS-this->m_dwOneFrameTime);
	
		this->m_dwOneFrameTime = 0;

		return true;
	}
	else 
	{
		/*this->m_dwOneFrameTime -= this->m_dwMaxFPS;

		if (this->m_dwOneFrameTime >=this->m_dwMaxFPS)
		{*/
			this->m_dwOneFrameTime -= this->m_dwMaxFPS;
			oldTime = nowTime;
			return false;
		//}	
	}

//	return true;
}