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

	// �� ������ ���α���
	int DestWidth = ScreenDib.GetWidth();

	// �� ������ ���α���
	int DestHeight = ScreenDib.GetHeight();

	// �� ������ ��ġ
	int pitch = ScreenDib.GetPitch();


	// ���� �����ؾߵ� ��������Ʈ �ε������̴�.
	SpriteDib.DrawSprite(this->m_dwSpriteNow, this->m_iXpos, this->m_iYpos, pDestDib, DestWidth, DestHeight, pitch);
}