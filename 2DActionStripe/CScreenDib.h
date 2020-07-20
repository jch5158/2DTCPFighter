#pragma once

class CScreenDib
{
public:

	CScreenDib(int iWidth, int iHeight, int iColorBit);

	virtual ~CScreenDib();

public:
	
	void Filp(HWND hWnd, int iX = 0, int iY = 0);

	BYTE* GetDibBuffer();
	
	int GetWidth(void);

	int GetHeight(void);

	int GetPitch(void);

protected:

	void CreateDibBuffer(int iWidth, int iHeight, int iColorBit);

	void ReleaseDibBuffer(void);


	BITMAPINFOHEADER m_stDibInfoh = { 0, };

	// 멥 버퍼를 위한 byte단위의 포인터입니다.
	BYTE* m_bypBuffer;

	// 백 버퍼의 가로길이 입니다.
	int m_iWidth;

	// 백 버퍼의 세로길이 입니다.
	int m_iHeight;
	
	// 4바이트 정렬된 한 줄의 바이트 수
	int m_iPitch;  	
	
	// 컬러 비트입니다.
	int m_iColorBit;

	// 버퍼 전체 사이즈입니다. ( 피치 X m_iHeight )
	int m_iBufferSize;

};


