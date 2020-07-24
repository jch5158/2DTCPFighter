#include "framework.h"

#include "CList.h"
#include "CScreenDib.h"
#include "CSpriteDib.h"
#include "CBaseObject.h"
#include "CDamageEffect.h"


CDamageEffect::CDamageEffect(unsigned short usX, unsigned short usY,DWORD actionCur)
{
	this->m_iXpos = usX;
	this->m_iYpos = usY;
	this->m_dwActionCur = actionCur;
	this->deleteCheck = false;
	this->m_RenderCheck = false;

	SetSprite(e_SPRITE::eEFFECT_SPARK_01, e_SPRITE::eEFFECT_SPARK_MAX, 3);

	switch (this->m_dwActionCur)
	{
	case KeyList::eACTION_ATTACK1:


	{
		printf_s("attack1\n");
	}

		this->m_EffectTiming = 50;

		break;

	case KeyList::eACTION_ATTACK2:
	
	{
		printf_s("attack2\n");
	}

		this->m_EffectTiming = 100;

		break;

	case KeyList::eACTION_ATTACK3:
	
	{
		printf_s("attack3\n");
	}

		this->m_EffectTiming = 300;

		break;

	default:
	{
		printf_s("default\n");
	}

		this->m_EffectTiming = 300;

		break;
	}

	this->m_TimeSet = timeGetTime();

}


void CDamageEffect::Update()
{
	
	if ( this->m_EffectTiming <= timeGetTime() - this->m_TimeSet) {
	
		this->m_RenderCheck = true;

		NextFrame();

		if (this->m_dwSpriteNow == eEFFECT_SPARK_04)
		{
			this->deleteCheck = true;
		}
	}

}

void CDamageEffect::Render()
{
	if (this->m_RenderCheck == false) 
	{
		return;
	}


	BYTE* pDestDib = ScreenDib.GetDibBuffer();

	// 백 버퍼의 가로길이
	int DestWidth = ScreenDib.GetWidth();

	// 백 버퍼의 세로길이
	int DestHeight = ScreenDib.GetHeight();

	// 백 버퍼의 피치
	int pitch = ScreenDib.GetPitch();


	// 현재 실행해야될 스프라이트 인덱스값이다.
	SpriteDib.DrawSprite(this->m_dwSpriteNow, this->m_iXpos, this->m_iYpos, pDestDib, DestWidth, DestHeight, pitch);
}