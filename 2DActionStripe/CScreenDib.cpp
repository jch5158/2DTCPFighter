
#include <Windows.h>
#include <iostream>
#include "CScreenDib.h"

// 백버퍼를 셋팅해준다.
CScreenDib::CScreenDib(int iWidth, int iHeight, int iColorBit) :m_iWidth(iWidth), m_iHeight(iHeight), m_iColorBit(iColorBit)
{
	// 32비트 기반의 피치를 계산합니다.
	this->m_iPitch = ((this->m_iWidth * (m_iColorBit / 8)) + 3) & ~3;

	// 구조체의 크기만큼 셋팅해준다.
	this->m_stDibInfoh.biSize = sizeof(BITMAPINFOHEADER);

	// 가로 길이
	this->m_stDibInfoh.biWidth = iWidth;
	
	// 세로 길이 
	this->m_stDibInfoh.biHeight = -iHeight;
		
	this->m_stDibInfoh.biPlanes = 1;
	
	// 몇비트 컬러인지 확인
	this->m_stDibInfoh.biBitCount = iColorBit;

	// 압축유형 bmp파일은 압축을 하지않음 
	this->m_stDibInfoh.biCompression = BI_RGB;

	// 피지 * 높이만큼 동적할당해준다.
	CreateDibBuffer(m_iWidth, m_iHeight, m_iColorBit);
}

CScreenDib::~CScreenDib() {

	// 버퍼 사이즈를 해제합니다.
	ReleaseDibBuffer();

}

void CScreenDib::CreateDibBuffer(int iWidth, int iHeight, int iColorBit) {


	// 버퍼 사이즈만큼 동적할당을 합니다.
	this->m_bypBuffer = new BYTE[this->m_iPitch * this->m_iHeight];

}

void CScreenDib::ReleaseDibBuffer(void) {

	// 버퍼사이즈 동적할당 해제
	delete[] m_bypBuffer;
}

// 플립 호출시 백버퍼에 그린 값을 출력한다.
void CScreenDib::Filp(HWND hWnd, int iX, int iY) {

	// 윈도우 핸들로 dc
	HDC hdc = GetDC(hWnd);

	// dib출력하는 함수입니다.
	StretchDIBits
	(
		hdc,                                     // 목적지 DC
		iX,                                      // x 좌표
		iY,                                      // y 좌표
		this->m_iPitch,                          // 목적지 너비
		this->m_iHeight,						 // 목적지 높이
		0,                                       // 출력소스 x 좌표
		0,                                       // 출력소스 y 좌표
		this->m_iPitch,                          // 출력소스 너비
		this->m_iHeight,					     // 출력소스 높이
		this->m_bypBuffer,                       // 이미지가 위치한 포인터
		(BITMAPINFO*)(&this->m_stDibInfoh),      // BITMAPINFO = (BITMAPINFOHEADER + RGBQUAD) -> type캐스팅 해야한다. 
		DIB_RGB_COLORS,                          // DIB_RGB_COLORS , RGB 빨레트로 별로 쓸모가 없당.
		SRCCOPY                                  // 출력모드
	);
	
	static DWORD frameCheck = 50;

	static DWORD timeCheck = timeGetTime();

	static DWORD frameNow = 0;

	frameNow += 1;

	if (timeGetTime()- timeCheck >= 1000) {

		frameCheck = frameNow;

		timeCheck = timeGetTime();
	
		frameNow = 0;
	}

	WCHAR text[10];

	wsprintf(text, L"%d", frameCheck);

	TextOut(hdc, 0, 0, text, 2);

	ReleaseDC(hWnd, hdc);
}

// 백 버퍼의 포인터를 return 한다.
BYTE* CScreenDib::GetDibBuffer()
{
	return this->m_bypBuffer;
}

// 백 버퍼의 가로 길이를 return한다.
int CScreenDib::GetWidth(void) 
{
	return this->m_iWidth;
}

// 백 버퍼의 세로 길이를 return 한다.
int CScreenDib::GetHeight(void) 
{
	return this->m_iHeight;
}

// 백 버퍼의 pitch를 return한다.
int CScreenDib::GetPitch(void) 
{
	return this->m_iPitch;
}
