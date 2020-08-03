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
	
	// 지금 시간 - 셋팅된 시간.
	this->m_dwOneFrameTime += (nowTime - oldTime);

	//dwMaxFPS == 20 이다.	
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
		//	this->m_dwOneFrameTime -= this->m_dwMaxFPS;
			//oldTime = nowTime;
		//	return false;
		//}	

		this->m_dwOneFrameTime -= this->m_dwMaxFPS;

		oldTime = timeGetTime();

		if (this->m_dwOneFrameTime >=this->m_dwMaxFPS)
		{
			this->m_dwOneFrameTime -= this->m_dwMaxFPS;
			
			return false;
		}	

	}

	return true;
}

/*
bool CFrameSkip::FrameSkip(HWND hWnd)
{
	// Logic FPS 확인하기 위한 변수들
	static DWORD logicOldTime = timeGetTime();
	static DWORD logicFrame = 0;
	logicFrame++;

	static DWORD oldTime = timeGetTime();      // Render 처음 시작 시간 설정
	DWORD       curTime = timeGetTime();      // Render 현재 시간 설정

	_OneFrameTime += curTime - oldTime;         // 1 frame 걸리는 시간

	// 1초마다 LogicFrame 출력
	if (curTime - logicOldTime > 1000)
	{
		WCHAR szFrame[TEXT_SIZE];
		wsprintf(szFrame, L"LogicFPS = %d", logicFrame);
		SetWindowText(hWnd, szFrame);
		logicOldTime = timeGetTime();
		logicFrame = 0;
	}

	// 1 Frame 속도가 목표 속도 보다 빨랐을 경우와 느렸을 경우를 나누어서 로직을 처리한다.
	if (_OneFrameTime < _MaxFrameTime)
	{
		// 1 Frame 시간이 목표치 보다 빠르면 목표치가 될때까지 쉬어 준다.
		Sleep(_MaxFrameTime - _OneFrameTime);
		// ! 주의 : oldTime 에서 timeGetTime를 사용하면 안됨 !
		// Sleep 함수 작동 시간이 있기때문에 oldTime을 timeGetTime으로 셋팅한다면 정확한 값을 알 수 없다.
		// 계산으로 아까 구한 현재 시간으로 부터 쉰만큼 더해준다.
		oldTime = curTime + (_MaxFrameTime - _OneFrameTime);
		_OneFrameTime = 0;
	}
	else
	{
		// OneFrameTime이 _MaxFrameTime를 초과했다면 초과한 값 만큼 남겨둔다.
		_OneFrameTime -= _MaxFrameTime;

		oldTime = timeGetTime();

		// _OneFrameTime이 초과한 값이 누적되어 _MaxFrameTime를 달성했다면 Render 한번을 Skip하여 LoginFPS를 맞춰준다.
		if (_OneFrameTime >= _MaxFrameTime)
		{
			_OneFrameTime -= _MaxFrameTime;
			return false;
		}
	}

	return true;
}
*/