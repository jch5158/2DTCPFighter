#include "framework.h"

#include "CSpriteDib.h"



CSpriteDib::CSpriteDib(int m_iMaxSprite,DWORD dwColorKey) 
{
	// 배열 최대 개수
	this->m_iMaxSprite = m_iMaxSprite;

	// 등록할 컬러 `
	this->m_dwColorKey = dwColorKey;

	// 최대 읽어올 개수 만큼 미리 할당 받는다.
	this->m_stpSprite = new st_SPRITE[m_iMaxSprite];

	// 0 으로 memset~
	memset(m_stpSprite, 0, sizeof(st_SPRITE) * m_iMaxSprite);
}

CSpriteDib::~CSpriteDib() 
{

	for(int iCnt = 0; iCnt < this->m_iMaxSprite; iCnt++)
	{	
		// 배열 개수만큼 릴리즈
		ReleaseStrite(iCnt);
	}


	delete[] this->m_stpSprite;
}

// 스프라이트 파일을 가져오는 함수입니다.
BOOL CSpriteDib::LoadDibSprite(int iSpriteIndex, const char* szFileName, int iCenterPointX, int iCenterPointY) 
{
	// bmp 파일 오픈할 FILE 변수입니다.
	FILE* fp;
	
	// 피치 길이입니다.
	int iPitch;

	// 이미지 길이입니다.
	int iImageSize;

	BITMAPFILEHEADER stFileHeader;

	// 비트맵 인포 헤더입니다.
	BITMAPINFOHEADER stInfoHeader;

	fopen_s(&fp, szFileName, "rb");
	
	if (fp == nullptr) {
		return false;
	}

	// 해당 스프라이트에 이미 데이터가 있다면 이를 지우자. 혹시 있을끼바
	ReleaseStrite(iSpriteIndex);

	
	fread(&stFileHeader, sizeof(stFileHeader), 1,fp);

	if (0x4d42 == stFileHeader.bfType) 
	{
		fread(&stInfoHeader, sizeof(stInfoHeader), 1, fp);

		if (32 == stInfoHeader.biBitCount)
		{
			// 읽은 이미지의 한줄 피치값
			iPitch = ((stInfoHeader.biWidth * 4) + 3) & ~3;

			// 비트맵 인포 헤더로 읽은 후 해당 정보를 m_stpSprite 배열 구조체 저장
			this->m_stpSprite[iSpriteIndex].iWidth = stInfoHeader.biWidth;
			this->m_stpSprite[iSpriteIndex].iHeight = stInfoHeader.biHeight;
			this->m_stpSprite[iSpriteIndex].iPitch = iPitch;
			this->m_stpSprite[iSpriteIndex].iCenterPointX = iCenterPointX;
			this->m_stpSprite[iSpriteIndex].iCenterPointY = iCenterPointY;

			// 이미지 사이즈를 구한다.
			iImageSize = iPitch * stInfoHeader.biHeight;


			// 이미지 크기만큼 동적할당
			this->m_stpSprite[iSpriteIndex].bytImage = new BYTE[iImageSize];

			// 이미지 크기 만큼 동적할당
			BYTE* bytTempBuffer = new BYTE[iImageSize];

			// 이미지 스프라이트 배열의 byImage를 가리키는 포인터
			BYTE* bytSpriteTemp = this->m_stpSprite[iSpriteIndex].bytImage;

			// 이미지를 뒤집기 위한 포인터이다.
			BYTE* bytTurnTemp;

			fread(bytTempBuffer, iImageSize, 1, fp);

			// 한줄 뒤집자.
			// biHeight -1을 한 이유는 마지막 첫줄의 시작점 부터 읽어야 하기 때문이다.
			bytTurnTemp = bytTempBuffer + iPitch * (stInfoHeader.biHeight - 1);

			for (int iCnt = 0; iCnt < stInfoHeader.biHeight; iCnt++)
			{
				// 피치 길이만큼 bytStripteTemp에 뒤집은 포인터를 복사한다.
				memcpy(bytSpriteTemp, bytTurnTemp, iPitch);

				// 다음줄에 복사
				bytSpriteTemp += iPitch;

				// 아래에서 윗줄에 복사
				bytTurnTemp -= iPitch;
			}

			fclose(fp);

			delete[] bytTempBuffer;
			return true;
		}
	}
	
	return false;
}

// 스프라이트 파일을 삭제하는 함수입니다.
void CSpriteDib::ReleaseStrite(int iSpriteIndex)
{
	if (this->m_iMaxSprite <= iSpriteIndex)
		return;

	if (nullptr != m_stpSprite[iSpriteIndex].bytImage)
	{
		delete[] m_stpSprite[iSpriteIndex].bytImage;
		memset(&this->m_stpSprite[iSpriteIndex], 0, sizeof(st_SPRITE));
	}
}

// 스프라이트 정보를 백버퍼에 그리는 함수입니다.
void CSpriteDib::DrawSprite(int iSpriteIndex, short iDrawX, short iDrawY, BYTE* bytDest, int iDestWidth, int iDestHeight, int iDestPitch, int iDrawLen)
{
	// 인자값이 배열의 최대 길이보다 클 경우 return
	if (iSpriteIndex >= m_iMaxSprite)
		return;

	// 이미지에 값이 없으면은 return
	if (nullptr == this->m_stpSprite[iSpriteIndex].bytImage)
		return;

	// 배열에 저장된 이미지 정보를 가져옵니다.
	st_SPRITE* stpSprite = &m_stpSprite[iSpriteIndex];

	// 이미지의 가로길이 변수에 담기
	int iSpriteWidth = stpSprite->iWidth;

	// 이미지의 세로길이 변수에 담기
	int iSpriteHeight = stpSprite->iHeight;

	// 가로 몇퍼센트만 그릴지
	iSpriteWidth = iSpriteWidth * iDrawLen / 100;


	// 백버퍼의 포인터이다.
	// 픽셀단위로 접근하기 때문에 DWORD 즉, unsigned long으로 받는다.
	DWORD* dwpDest = (DWORD*)bytDest;
	

	// 이미지 포인터
	// 픽셀단위로 접근하기 때문에 DWORD 즉, unsigned long으로 받는다.
	DWORD* dwpSprite = (DWORD*)(stpSprite->bytImage);

	
	// 그리는 X좌표에서 X센터 포지션만큼 뺀다.
	iDrawX = iDrawX - stpSprite->iCenterPointX;
	
	// 그리는 Y좌표에서 Y센터 포지션만큼 뺀다.
	iDrawY = iDrawY - stpSprite->iCenterPointY;
	

	// 위클로킹
	if(0 > iDrawY)
	{
		iSpriteHeight = iSpriteHeight - (-iDrawY);

		// Y위치와 피치의 길이를 곱한 후 이미지 포인터에 더하여 
		// 그려야 될 이미지의 포인터 위치를 셋팅한다.
		dwpSprite = (DWORD*)(stpSprite->bytImage + stpSprite->iPitch * (-iDrawY));

		// 그리고 iDrawY 값을 0으로 셋튕~
		iDrawY = 0;	
	}


	// 왼쪽 클로킹
	if (0 > iDrawX)
	{
		// 가로길이에서 x포지션의 값을 더한다.
		iSpriteWidth = iSpriteWidth - (-iDrawX);

		
		// 이미지 정보 포인터에서 X포지션 만큼 더한다.
		dwpSprite = dwpSprite + (-iDrawX);

		iDrawX = 0;
	}


	// 아래클로킹
	// 이미지의 크기가 메모리 버퍼의 세로 길이를 넘어갔을 때 if문
	if (iDestHeight <= iDrawY + iSpriteHeight)
	{
		// 백 버퍼 세로길이를 넘어간 만큼 이미지를 그릴 세로길이에서 뺀다.
		iSpriteHeight -= ((iDrawY + stpSprite->iHeight)- iDestHeight);
	}


	// 오른쪽 클리핑
	if (iDestWidth <= iDrawX + iSpriteWidth)
	{
		// 백 버퍼 가로 길이를 넘어간 만큼 이미지를 그릴 가로길이에서 뺀다.
		iSpriteWidth -= ((iDrawX + iSpriteWidth) - iDestWidth);
	}

	// 이미지의 가로길이 또는 세로길이가 0 이하면은 그릴게 없기 때문에 그냥 return하면 된다.
	if (iSpriteWidth <= 0 || iSpriteHeight <= 0)
		return;


	// 이미지를 그릴 시작점을 정한다. 
	dwpDest = (DWORD*)((BYTE*)(dwpDest + iDrawX) + (iDrawY * iDestPitch));

	// 백버퍼 오리진, 바이트 단위로 연산을 진행하기 위함이다.
	BYTE* byteDestOrigin = (BYTE*)dwpDest;

	//이미지 정보 오리진, 바이트 단위로 연산을 진행하기 위함이다.
	BYTE* byteSpriteOrigin = (BYTE*)dwpSprite;


	// 이미지의 세로 길이만큼 반복
	for (int iCntY = 0; iSpriteHeight > iCntY; iCntY++)
	{
		// 이미지의 가로 길이만큼 반복
		for (int iCntX = 0; iSpriteWidth > iCntX; iCntX++)
		{
			// 이미지의 컬러키를 제외하고 그린다.
			if(this->m_dwColorKey != (*dwpSprite & 0x00ffffff))
			{		
				
				*dwpDest = *dwpSprite;		
			}
			
			dwpDest++;
			dwpSprite++;
		}
		
		// 백 버퍼의 다음 줄
		byteDestOrigin = byteDestOrigin + iDestPitch;
	
		// 이미지 정보에서 다음 줄
		byteSpriteOrigin = byteSpriteOrigin + stpSprite->iPitch;

		dwpDest = (DWORD*)byteDestOrigin;
		dwpSprite = (DWORD*)byteSpriteOrigin;
	}
}

// 배경용 스프라이트 정보를 백버퍼에 그리는 함수입니다.
void CSpriteDib::DrawImage(int iSpriteIndex,  short iDrawX,  short iDrawY, BYTE* bytDest, int iDestWidth, int iDestHeight, int iDestPitch, int iDrawLen)
{
	// 최대 스프라이트 배열 보다 클 수 없다. 
	if (iSpriteIndex >= m_iMaxSprite)
		return;

	// 해당 스프라이트 인덱스가 nullptr 일 경우 return
	if (nullptr == this->m_stpSprite[iSpriteIndex].bytImage)
		return;

	st_SPRITE* stpSprite = &m_stpSprite[iSpriteIndex];

	int iSpriteWidth = stpSprite->iWidth;
	int iSpriteHeight = stpSprite->iHeight;

	iSpriteWidth = iSpriteWidth * iDrawLen / 100;

	// 백 버퍼 포인터이다.
	DWORD* dwpDest = (DWORD*)bytDest;
	
	// 배경 그림 포인터이다.
	DWORD* dwpSprite = (DWORD*)(stpSprite->bytImage);

	// 인자로 들어온 좌표와 중점 좌표를 계산한다.
	iDrawX = iDrawX - stpSprite->iCenterPointX;
	iDrawY = iDrawY - stpSprite->iCenterPointY;	

	if (iSpriteWidth <= 0 || iSpriteHeight <= 0)
		return;

	dwpDest = (DWORD*)((BYTE*)(dwpDest + iDrawX) + (iDrawY * iDestPitch));

	BYTE* byteDestOrigin = (BYTE*)dwpDest;
	BYTE* byteSpriteOrigin = (BYTE*)dwpSprite;

	for (int iCntY = 0; iSpriteHeight > iCntY; iCntY++)
	{
		for (int iCntX = 0; iSpriteWidth > iCntX; iCntX++)
		{
			if (this->m_dwColorKey != (*dwpSprite & 0x00ffffff)) 
			{	
				// 백 버퍼에 4BYTE 만큼 씩 복사한다.
				*dwpDest = *dwpSprite;
			}

			dwpDest++;
			dwpSprite++;
		}

		// 피치만큼 포인터를 옮겨 다음 줄을 셋팅한다.
		byteDestOrigin = byteDestOrigin + iDestPitch;
		byteSpriteOrigin = byteSpriteOrigin + stpSprite->iPitch;

		// 셋팅한 다음줄을 DWORD 포인터에 캐스팅하여 참조한다.
		dwpDest = (DWORD*)byteDestOrigin;
		dwpSprite = (DWORD*)byteSpriteOrigin;
	}
}
