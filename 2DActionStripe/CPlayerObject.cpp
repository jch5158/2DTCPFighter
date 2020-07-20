#include "framework.h"

#include "CList.h"
#include "CScreenDib.h"
#include "CSpriteDib.h"
#include "CBaseObject.h"
#include "CPlayerObject.h"


void CPlayerObject::Update()
{

	NextFrame();
	ActionProc();	
}

void CPlayerObject::ActionProc()
{	
	
	switch (this->m_dwActionCur)
	{
	case KeyList::eACTION_ATTACK1:
	case KeyList::eACTION_ATTACK2:
	case KeyList::eACTION_ATTACK3:
	
		if (this->m_bEndFrame)
		{
			if (this->m_dwDirCur == e_PlayerDir::eRight) 
			{
				SetSprite(e_SPRITE::ePLAYER_STAND_R01, e_SPRITE::ePLAYER_STAND_R_MAX, 5);
			}
			else if (this->m_dwDirCur == e_PlayerDir::eLeft)
			{
				SetSprite(e_SPRITE::ePLAYER_STAND_L01, e_SPRITE::ePLAYER_STAND_L_MAX, 5);
			}
		
			this->m_dwActionOld = this->m_dwActionCur;
			this->m_dwActionCur = KeyList::eACTION_STAND;
		}	

		
		break;

	default:

		InputActionProc();

		break;
	}
}

void CPlayerObject::InputActionProc()
{

	this->m_dwActionOld = this->m_dwActionCur;
	this->m_dwActionCur = this->m_ActionInput;

	switch (this->m_dwActionCur)
	{
	case KeyList::eACTION_MOVE_UU:

		if (this->m_iYpos > 50) {
			this->m_iYpos -= 2;
		}

		if (m_dwActionOld != KeyList::eACTION_STAND && this->m_dwDirCur == e_PlayerDir::eRight)
		{	
			if (this->m_bEndFrame)
			{
				SetSprite(e_SPRITE::ePLAYER_MOVE_R01, e_SPRITE::ePLAYER_MOVE_R_MAX, 4);
			}
		}
		else if (m_dwActionOld != KeyList::eACTION_STAND && this->m_dwDirCur == e_PlayerDir::eLeft)
		{
			if (this->m_bEndFrame) 
			{
				SetSprite(e_SPRITE::ePLAYER_MOVE_L01, e_SPRITE::ePLAYER_MOVE_L_MAX, 4);
			}
		}
		else if(this->m_dwDirCur == e_PlayerDir::eLeft)
		{
			SetSprite(e_SPRITE::ePLAYER_MOVE_L01, e_SPRITE::ePLAYER_MOVE_L_MAX, 4);
		}
		else if (this->m_dwDirCur == e_PlayerDir::eRight)
		{
			SetSprite(e_SPRITE::ePLAYER_MOVE_R01, e_SPRITE::ePLAYER_MOVE_R_MAX, 4);
		}

		break;

	case KeyList::eACTION_MOVE_DD:

		if (this->m_iYpos < 470)
		{
			this->m_iYpos += 2;
		}

		if (m_dwActionOld != KeyList::eACTION_STAND && this->m_dwDirCur == e_PlayerDir::eRight)
		{
			if (this->m_bEndFrame)
			{
				SetSprite(e_SPRITE::ePLAYER_MOVE_R01, e_SPRITE::ePLAYER_MOVE_R_MAX, 4);
			}
		}
		else if (m_dwActionOld != KeyList::eACTION_STAND && this->m_dwDirCur == e_PlayerDir::eLeft)
		{
			if (this->m_bEndFrame)
			{
				SetSprite(e_SPRITE::ePLAYER_MOVE_L01, e_SPRITE::ePLAYER_MOVE_L_MAX, 4);
			}
		}
		else if(this->m_dwDirCur == e_PlayerDir::eLeft)
		{
			SetSprite(e_SPRITE::ePLAYER_MOVE_L01, e_SPRITE::ePLAYER_MOVE_L_MAX, 4);
		}
		else if(this->m_dwDirCur == e_PlayerDir::eRight)
		{
		
			SetSprite(e_SPRITE::ePLAYER_MOVE_R01, e_SPRITE::ePLAYER_MOVE_R_MAX, 4);
		}


		break;

	case KeyList::eACTION_MOVE_RR:

		if (this->m_iXpos < 630)
		{
			this->m_iXpos += 3;
		}
		
		if (m_dwActionOld != KeyList::eACTION_STAND && this->m_dwDirCur == e_PlayerDir::eRight)
		{
			if (this->m_bEndFrame)
			{
				SetSprite(e_SPRITE::ePLAYER_MOVE_R01, e_SPRITE::ePLAYER_MOVE_R_MAX, 4);
			}
		}else 
		{
			SetSprite(e_SPRITE::ePLAYER_MOVE_R01, e_SPRITE::ePLAYER_MOVE_R_MAX, 4);
		}	

		this->m_dwDirOld = this->m_dwDirCur;
		this->m_dwDirCur = e_PlayerDir::eRight;

		break;

	case KeyList::eACTION_MOVE_LL:

		if (this->m_iXpos > 10)
		{
			this->m_iXpos -= 3;
		}

		if (m_dwActionOld != KeyList::eACTION_STAND && this->m_dwDirCur == e_PlayerDir::eLeft)
		{
			if (this->m_bEndFrame)
			{
				SetSprite(e_SPRITE::ePLAYER_MOVE_L01, e_SPRITE::ePLAYER_MOVE_L_MAX, 4);
			}
		}
		else
		{
			SetSprite(e_SPRITE::ePLAYER_MOVE_L01, e_SPRITE::ePLAYER_MOVE_L_MAX, 4);
		}

		this->m_dwDirOld = this->m_dwDirCur;
		this->m_dwDirCur = e_PlayerDir::eLeft;

		break;

	case KeyList::eACTION_MOVE_LU:

		if (this->m_iXpos > 10 && this->m_iYpos > 50)
		{
			this->m_iXpos -= 3;
			this->m_iYpos -= 2;
		}

		if (m_dwActionOld != KeyList::eACTION_STAND && this->m_dwDirCur == e_PlayerDir::eLeft)
		{
			if (this->m_bEndFrame)
			{
				SetSprite(e_SPRITE::ePLAYER_MOVE_L01, e_SPRITE::ePLAYER_MOVE_L_MAX, 4);
			}
		}
		else
		{
			SetSprite(e_SPRITE::ePLAYER_MOVE_L01, e_SPRITE::ePLAYER_MOVE_L_MAX, 4);
		}

		this->m_dwDirOld = this->m_dwDirCur;
		this->m_dwDirCur = e_PlayerDir::eLeft;

		break;

	case KeyList::eACTION_MOVE_LD:

		if (this->m_iXpos > 10 && this->m_iYpos < 470)
		{
			this->m_iXpos -= 3;
			this->m_iYpos += 2;
		}

		if (m_dwActionOld != KeyList::eACTION_STAND && this->m_dwDirCur == e_PlayerDir::eLeft)
		{
			if (this->m_bEndFrame)
			{
				SetSprite(e_SPRITE::ePLAYER_MOVE_L01, e_SPRITE::ePLAYER_MOVE_L_MAX, 4);
			}
		}
		else
		{
			SetSprite(e_SPRITE::ePLAYER_MOVE_L01, e_SPRITE::ePLAYER_MOVE_L_MAX, 4);
		}

		this->m_dwDirOld = this->m_dwDirCur;
		this->m_dwDirCur = e_PlayerDir::eLeft;

		break;

	case KeyList::eACTION_MOVE_RU:

		if (this->m_iXpos < 630 && this->m_iYpos > 50)
		{
			this->m_iYpos -= 2;
			this->m_iXpos += 3;
		}

		if (m_dwActionOld != KeyList::eACTION_STAND && this->m_dwDirCur == e_PlayerDir::eRight)
		{
			if (this->m_bEndFrame)
			{
				SetSprite(e_SPRITE::ePLAYER_MOVE_R01, e_SPRITE::ePLAYER_MOVE_R_MAX, 4);
			}
		}
		else
		{
			SetSprite(e_SPRITE::ePLAYER_MOVE_R01, e_SPRITE::ePLAYER_MOVE_R_MAX, 4);
		}

		this->m_dwDirOld = this->m_dwDirCur;
		this->m_dwDirCur = e_PlayerDir::eRight;

		break;

	case KeyList::eACTION_MOVE_RD:
		if (this->m_iXpos < 630 && this->m_iYpos < 470)
		{
			this->m_iYpos += 2;
			this->m_iXpos += 3;
		}


		if (m_dwActionOld != KeyList::eACTION_STAND && this->m_dwDirCur == e_PlayerDir::eRight)
		{
			if (this->m_bEndFrame)
			{
				SetSprite(e_SPRITE::ePLAYER_MOVE_R01, e_SPRITE::ePLAYER_MOVE_R_MAX, 4);
			}
		}
		else
		{
			SetSprite(e_SPRITE::ePLAYER_MOVE_R01, e_SPRITE::ePLAYER_MOVE_R_MAX, 4);
		}

		this->m_dwDirOld = this->m_dwDirCur;
		this->m_dwDirCur = e_PlayerDir::eRight;

		break;

	case KeyList::eACTION_ATTACK1:

		if(this->m_dwDirCur == e_PlayerDir::eRight)
		{
			SetSprite(e_SPRITE::ePLAYER_ATTACK1_R01, e_SPRITE::ePLAYER_ATTACK1_R_MAX, 3);
		}
		else if (this->m_dwDirCur == e_PlayerDir::eLeft)
		{
			SetSprite(e_SPRITE::ePLAYER_ATTACK1_L01, e_SPRITE::ePLAYER_ATTACK1_L_MAX, 3);
		}

			break;

	case KeyList::eACTION_ATTACK2:

		if (this->m_dwDirCur == e_PlayerDir::eRight)
		{
			SetSprite(e_SPRITE::ePLAYER_ATTACK2_R01, e_SPRITE::ePLAYER_ATTACK2_R_MAX, 4);
		}
		else if (this->m_dwDirCur == e_PlayerDir::eLeft)
		{
			SetSprite(e_SPRITE::ePLAYER_ATTACK2_L01, e_SPRITE::ePLAYER_ATTACK2_L_MAX, 4);
		}

			break;

	case KeyList::eACTION_ATTACK3:

		if (this->m_dwDirCur == e_PlayerDir::eRight)
		{
			SetSprite(e_SPRITE::ePLAYER_ATTACK3_R01, e_SPRITE::ePLAYER_ATTACK3_R_MAX, 4);
		}
		else if (this->m_dwDirCur == e_PlayerDir::eLeft)
		{
			SetSprite(e_SPRITE::ePLAYER_ATTACK3_L01, e_SPRITE::ePLAYER_ATTACK3_L_MAX, 4);
		}

			break;
	case KeyList::eACTION_STAND:

		
		if (this->m_dwDirCur == e_PlayerDir::eRight)
		{	
			if(this->m_dwActionOld != KeyList::eACTION_STAND )
			{
				SetSprite(e_SPRITE::ePLAYER_STAND_R01, e_SPRITE::ePLAYER_STAND_R_MAX, 5);
			}
			else if (this->m_bEndFrame)
			{
				SetSprite(e_SPRITE::ePLAYER_STAND_R01, e_SPRITE::ePLAYER_STAND_R_MAX, 5);
			}
		}
		else if (this->m_dwDirCur == e_PlayerDir::eLeft)
		{
		
			if (this->m_dwActionOld != KeyList::eACTION_STAND)
			{
				SetSprite(e_SPRITE::ePLAYER_STAND_L01, e_SPRITE::ePLAYER_STAND_L_MAX, 5);
			}
			else if (this->m_bEndFrame)
			{
				SetSprite(e_SPRITE::ePLAYER_STAND_L01, e_SPRITE::ePLAYER_STAND_L_MAX, 5);
			}
		}

		break;
	}

	
}

void CPlayerObject::Render()
{
	// 백 버퍼를 받는다.
	BYTE* pDestDib = ScreenDib.GetDibBuffer();

	// 백 버퍼의 가로길이
	int DestWidth = ScreenDib.GetWidth();

	// 백 버퍼의 세로길이
	int DestHeight = ScreenDib.GetHeight();

	// 백 버퍼의 피치
	int pitch = ScreenDib.GetPitch();


	SpriteDib.DrawSprite(eSHADOW, this->m_iXpos, this->m_iYpos, pDestDib, DestWidth, DestHeight, pitch);

	SpriteDib.DrawSprite(this->m_dwSpriteNow, this->m_iXpos, this->m_iYpos, pDestDib, DestWidth, DestHeight, pitch);	

	SpriteDib.DrawSprite(eGUAGE_HP, this->m_iXpos-35, this->m_iYpos+9, pDestDib, DestWidth, DestHeight, pitch,this->m_chHP);

}

void CPlayerObject::SetHp(DWORD Hp)
{
	this->m_chHP = Hp;
}

CPlayerObject::CPlayerObject()
{
	static DWORD playerID = 0;

	this->m_ObjectType = e_ObjType::eType_Player;

	this->m_chHP = 100;

	this->m_dwObjectID = playerID;

	this->m_ActionInput = KeyList::eACTION_STAND;


	// 지금 입력받은 메시지
	this->m_dwActionCur = KeyList::eACTION_STAND;

	// 그 전에 입력받은 메시지
	this->m_dwActionOld = KeyList::eACTION_STAND;

	// 방향 오른쪽 디폴트
	this->m_dwDirCur = e_PlayerDir::eRight;

}

CPlayerObject::~CPlayerObject()
{
}


