#include "framework.h"

#include "CList.h"

#include "CScreenDib.h"
#include "CSpriteDib.h"

#include "CBaseObject.h"
#include "CPlayerObject.h"


// �� ������ ���� ���� ���� �׸��� 32 �÷���Ʈ
CScreenDib ScreenDib = CScreenDib(640, 480, 32);;

// �迭 �ִ밳���� �÷�Ű �� ARGB  -> ��Ʋ ��������� BGRA
CSpriteDib SpriteDib = CSpriteDib(e_SPRITE::eSPRITE_MAX, 0x00ffffff);




void CBaseObject::SetSprite(DWORD dwSpriteStart, DWORD dwSpriteEnd, DWORD dwFrameDelay)
{
	this->m_dwSpriteStart = dwSpriteStart;
	this->m_dwSpriteEnd = dwSpriteEnd;
	this->m_dwFrameDelay = dwFrameDelay;

	this->m_dwSpriteNow = dwSpriteStart;
	this->m_dwDelayCount = 0;

	// �ش� ��������Ʈ ������ �������� üũ�ϱ� ���� �ܰ�
	this->m_bEndFrame = false;
}

int CBaseObject::NextFrame()
{
	
	this->m_dwDelayCount++;

	// ������ ���ǿ� ������ ���� ��������Ʈ �������� �����Ѵ�.
	if (this->m_dwDelayCount >= m_dwFrameDelay)
	{
		this->m_dwDelayCount = 0;

		this->m_dwSpriteNow++;

		// ������ ��������Ʈ ���۱��� ������ ���
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