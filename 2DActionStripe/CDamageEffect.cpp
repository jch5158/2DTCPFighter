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

		this->m_EffectTiming = 100;

		break;

	case KeyList::eACTION_ATTACK2:
		
		this->m_EffectTiming = 150;

		break;

	case KeyList::eACTION_ATTACK3:
	
		this->m_EffectTiming = 300;

		break;

	default:

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

	// �� ������ ���α���
	int DestWidth = ScreenDib.GetWidth();

	// �� ������ ���α���
	int DestHeight = ScreenDib.GetHeight();

	// �� ������ ��ġ
	int pitch = ScreenDib.GetPitch();


	// ���� �����ؾߵ� ��������Ʈ �ε������̴�.
	SpriteDib.DrawSprite(this->m_dwSpriteNow, this->m_iXpos, this->m_iYpos, pDestDib, DestWidth, DestHeight, pitch);
}