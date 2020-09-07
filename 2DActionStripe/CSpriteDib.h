#pragma once


//StripteDib 는 이미지 정보를 받아서 백버퍼에 그리기 위한 클래스입니다.
class CSpriteDib
{

public:

	// DIB 스프라이트 구조체
	struct st_SPRITE {
		
		// 이미지 크기 포인터
		BYTE* bytImage;

		// 가로길이
		int iWidth;

		// 세로길이
		int iHeight;

		// 피치길이
		int iPitch;

		// 중점 X
		int iCenterPointX;
		
		// 중점 Y
		int iCenterPointY;
	};

	CSpriteDib(int m_iMaxSprite,DWORD dwColorKey);

	virtual ~CSpriteDib();

	BOOL LoadDibSprite(int iSpriteIndex, const char* szFileName, int iCenterPointX, int iCenterPointY);

	void ReleaseStrite(int iSpriteIndex);

	void DrawSprite(int iSpriteIndex,  short iDrawX,  short iDrawY, BYTE* bytDest, int iDestWidth, int iDestHeight, int iDestPitch, int iDrawLen = 100, void* playerPtr = nullptr);

	void DrawImage(int iSpriteIndex,  short iDrawX,  short iDrawY, BYTE* bytDest, int iDestWidth, int iDestHeight, int iDestPitch, int iDrawLen = 100);


protected:

	// 스프라이트 배열 정보
	int m_iMaxSprite;

	// 이미지 정보 배열
	st_SPRITE* m_stpSprite;

	// 컬러키입니다.
	DWORD m_dwColorKey;
};

extern CSpriteDib SpriteDib;
