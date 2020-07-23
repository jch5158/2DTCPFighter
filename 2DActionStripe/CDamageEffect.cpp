#include "framework.h"

#include "CList.h"
#include "CScreenDib.h"
#include "CSpriteDib.h"
#include "CBaseObject.h"
#include "CDamageEffect.h"


CDamageEffect::CDamageEffect(unsigned short usX, unsigned short usY)
{

	this->m_iXpos = usX;
	this->m_iYpos = usY;

	SetSprite(e_SPRITE::eEFFECT_SPARK_01, e_SPRITE::eEFFECT_SPARK_MAX, 3);

	this->deleteCheck = false;
}


void CDamageEffect::Update()
{
	NextFrame();

	if (this->m_dwSpriteNow == eEFFECT_SPARK_04)
	{
		this->deleteCheck = true;
	}
}

void CDamageEffect::Render()
{
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