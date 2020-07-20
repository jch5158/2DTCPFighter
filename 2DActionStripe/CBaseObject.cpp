#include "framework.h"

#include "CList.h"

#include "CScreenDib.h"
#include "CSpriteDib.h"

#include "CBaseObject.h"
#include "CPlayerObject.h"


// 백 버퍼의 가로 세로 길이 그리고 32 컬러비트
CScreenDib ScreenDib = CScreenDib(640, 480, 32);;

// 배열 최대개수와 컬러키 값 ARGB  -> 리틀 엔디안으로 BGRA
CSpriteDib SpriteDib = CSpriteDib(e_SPRITE::eSPRITE_MAX, 0x00ffffff);




void CBaseObject::SetSprite(DWORD dwSpriteStart, DWORD dwSpriteEnd, DWORD dwFrameDelay)
{
	this->m_dwSpriteStart = dwSpriteStart;
	this->m_dwSpriteEnd = dwSpriteEnd;
	this->m_dwFrameDelay = dwFrameDelay;

	this->m_dwSpriteNow = dwSpriteStart;
	this->m_dwDelayCount = 0;

	// 해당 스프라이트 동작이 끝났는지 체크하기 위한 단계
	this->m_bEndFrame = false;
}

int CBaseObject::NextFrame()
{
	
	this->m_dwDelayCount++;

	// 딜레이 조건에 맞으면 다음 스프라이트 동작으로 변경한다.
	if (this->m_dwDelayCount >= m_dwFrameDelay)
	{
		this->m_dwDelayCount = 0;

		this->m_dwSpriteNow++;

		// 마지막 스프라이트 동작까지 수행한 경우
		if (this->m_dwSpriteNow >= this->m_dwSpriteEnd)
		{
			this->m_dwSpriteNow = this->m_dwSpriteStart;
			this->m_bEndFrame = true;
		}
	}

	return -1;
}


CBaseObject::CBaseObject() {

}

CBaseObject::~CBaseObject() {}


int CBaseObject::GetX() 
{
	return this->m_iXpos;
}

int CBaseObject::GetY() 
{
	return this->m_iYpos;
}

void CBaseObject::SetPosition(int iXpos, int iYpos)
{
	this->m_iXpos = iXpos;

	this->m_iYpos = iYpos;
}


void CBaseObject::ActionInput(DWORD dwAction)
{
	this->m_ActionInput = dwAction;

	return;
}