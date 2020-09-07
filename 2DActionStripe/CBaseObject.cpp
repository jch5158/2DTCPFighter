#include "stdafx.h"
#include "CSpriteDib.h"
#include "CScreenDib.h"
#include "CBaseObject.h"
#include "CPlayerObject.h"




void CBaseObject::SetSprite(DWORD dwSpriteStart, DWORD dwSpriteEnd, DWORD dwFrameDelay)
{
	this->m_dwSpriteStart = dwSpriteStart;
	this->m_dwSpriteEnd = dwSpriteEnd;
	this->m_dwFrameDelay = dwFrameDelay;

	this->m_dwSpriteNow = dwSpriteStart;
	this->m_dwDelayCount = 0;

	// 해당 스프라이트 동작이 끝났는지 체크하기 위한 단계
	// 동작이 끝났으면 NextFrame() 함수에서 true로 변경된다.
	this->m_bEndFrame = false;
}


int CBaseObject::NextFrame()
{	

	// 딜레이 조건에 맞으면 다음 스프라이트 동작으로 변경한다.
	if (this->m_dwDelayCount >= m_dwFrameDelay)
	{
		this->m_dwDelayCount = 0;

		this->m_dwSpriteNow += 1;

		// 마지막 스프라이트 동작까지 수행한 경우
		if (this->m_dwSpriteNow >= this->m_dwSpriteEnd)
		{
			this->m_dwSpriteNow = this->m_dwSpriteStart;
			this->m_bEndFrame = true;
		}
	}


	this->m_dwDelayCount += 1;

	return -1;
}


CBaseObject::CBaseObject() {

}

CBaseObject::~CBaseObject() {}


unsigned int CBaseObject::GetX() 
{
	return this->m_iXpos;
}

unsigned int CBaseObject::GetY() 
{
	return this->m_iYpos;
}


// 캐릭터의 위치를 셋팅해주는 함수입니다.
void CBaseObject::SetPosition(int iXpos, int iYpos)
{
	this->m_iXpos = iXpos;

	this->m_iYpos = iYpos;
}

// KeyProcess() 에서 입력값을 받습니다.
// 어떤 액션을 할지 캐릭터 객체 멤버변수에 셋팅해준다. 
void CBaseObject::ActionInput(DWORD dwAction)
{
	this->m_ActionInput = dwAction;

	return;
}