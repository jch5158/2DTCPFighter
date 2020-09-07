#include "framework.h"

#include "CScreenDib.h"
#include "CSpriteDib.h"
#include "CBaseObject.h"
#include "CPlayerObject.h"


void CPlayerObject::Update()
{
	// 조건에 맞으면은 다음 스프라이트 프레임으로 전환한다.
	NextFrame();

	// 액션 동작
	ActionProc();	
}

void CPlayerObject::ActionProc()
{	
	this->actionCheck = false;

	switch (this->m_dwActionCur)
	{
	case KeyList::eACTION_ATTACK1:
	case KeyList::eACTION_ATTACK2:
	case KeyList::eACTION_ATTACK3:
	
		// 공격 액션이 끝났다면은 스탠드 상태가 된다.
		if (this->m_bEndFrame)
		{
			// 캐릭터가 보는 방향이 오른쪽일 경우
			if (this->m_dwDirCur == e_PlayerDir::eRight) 
			{
				SetSprite(e_SPRITE::ePLAYER_STAND_R01, e_SPRITE::ePLAYER_STAND_R_MAX, 5);
			}
			// 캐릭터가 보는 방향이 왼쪽일 경우
			else if (this->m_dwDirCur == e_PlayerDir::eLeft)
			{
				SetSprite(e_SPRITE::ePLAYER_STAND_L01, e_SPRITE::ePLAYER_STAND_L_MAX, 5);
			}
					

			// 멈추고 다음 동작
			this->m_ActionInput = KeyList::eACTION_STAND;
			this->m_dwActionCur = KeyList::eACTION_STAND;
			this->actionCheck = true;	
		}	
	
		break;

	// 공격 액션이 끝났다면 움직이는 액션을 취할 수 있다. 	
	default:

		InputActionProc();

		break;
	}
}

void CPlayerObject::InputActionProc()
{
	// 현재 액션을 과거 액션에 넣어준다.
	DWORD dwActionOld = this->m_dwActionCur;
	
	// BaseObject의 ActionInput() 함수로 입력받은 값을 넣어준다.
	this->m_dwActionCur = this->m_ActionInput;

	if (dwActionOld != this->m_dwActionCur)
	{
		this->actionCheck = true;
	}

	switch (this->m_dwActionCur)
	{
	case KeyList::eACTION_MOVE_UU:

		if (this->m_iYpos > 57) {
			this->m_iYpos -= 2;
		}

		switch (this->m_dwDirCur)
		{
		case e_PlayerDir::eRight:			
			if (dwActionOld != KeyList::eACTION_STAND)
			{
				if (this->m_bEndFrame == true)
				{
					SetSprite(e_SPRITE::ePLAYER_MOVE_R01, e_SPRITE::ePLAYER_MOVE_R_MAX, 4);
				}
			}
			else 
			{
				SetSprite(e_SPRITE::ePLAYER_MOVE_R01, e_SPRITE::ePLAYER_MOVE_R_MAX, 4);
			}
			break;
		case e_PlayerDir::eLeft:
			if (dwActionOld != KeyList::eACTION_STAND)
			{
				if (this->m_bEndFrame == true)
				{
					SetSprite(e_SPRITE::ePLAYER_MOVE_L01, e_SPRITE::ePLAYER_MOVE_L_MAX, 4);
				}
			}
			else
			{
				SetSprite(e_SPRITE::ePLAYER_MOVE_L01, e_SPRITE::ePLAYER_MOVE_L_MAX, 4);
			}				
			break;
		}

		break;

	case KeyList::eACTION_MOVE_DD:

		if (this->m_iYpos < 467)
		{
			this->m_iYpos += 2;
		}

		switch (this->m_dwDirCur)
		{
		case e_PlayerDir::eRight:	
			if (dwActionOld != KeyList::eACTION_STAND)
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
			break;
		case e_PlayerDir::eLeft:
			if (dwActionOld != KeyList::eACTION_STAND)
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

			break;
		}

		break;

	case KeyList::eACTION_MOVE_RR:

		if (this->m_iXpos < 627)
		{
			this->m_iXpos += 3;
		}
		
		if (dwActionOld != KeyList::eACTION_STAND && this->m_dwDirCur == e_PlayerDir::eRight)
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

	case KeyList::eACTION_MOVE_LL:

		if (this->m_iXpos > 27)
		{
			this->m_iXpos -= 3;
		}

		if (dwActionOld != KeyList::eACTION_STAND && this->m_dwDirCur == e_PlayerDir::eLeft)
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

		if (this->m_iXpos > 27 && this->m_iYpos > 67)
		{
			this->m_iXpos -= 3;
			this->m_iYpos -= 2;
		}

		if (dwActionOld != KeyList::eACTION_STAND && this->m_dwDirCur == e_PlayerDir::eLeft)
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

		if (this->m_iXpos > 27 && this->m_iYpos < 467)
		{
			this->m_iXpos -= 3;
			this->m_iYpos += 2;
		}

		if (dwActionOld != KeyList::eACTION_STAND && this->m_dwDirCur == e_PlayerDir::eLeft)
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

		if (this->m_iXpos < 627 && this->m_iYpos > 67)
		{
			this->m_iYpos -= 2;
			this->m_iXpos += 3;
		}

		if (dwActionOld != KeyList::eACTION_STAND && this->m_dwDirCur == e_PlayerDir::eRight)
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
		if (this->m_iXpos < 627 && this->m_iYpos < 467)
		{
			this->m_iYpos += 2;
			this->m_iXpos += 3;
		}


		if (dwActionOld != KeyList::eACTION_STAND && this->m_dwDirCur == e_PlayerDir::eRight)
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
	
		switch (this->m_dwDirCur)
		{
		case e_PlayerDir::eRight:

			if (dwActionOld != KeyList::eACTION_STAND)
			{
				SetSprite(e_SPRITE::ePLAYER_STAND_R01, e_SPRITE::ePLAYER_STAND_R_MAX, 5);
			}
			else if (this->m_bEndFrame)
			{
				SetSprite(e_SPRITE::ePLAYER_STAND_R01, e_SPRITE::ePLAYER_STAND_R_MAX, 5);
			}

			break;
		case e_PlayerDir::eLeft:
			
			if (dwActionOld != KeyList::eACTION_STAND)
			{
				SetSprite(e_SPRITE::ePLAYER_STAND_L01, e_SPRITE::ePLAYER_STAND_L_MAX, 5);
			}
			else if (this->m_bEndFrame)
			{
				SetSprite(e_SPRITE::ePLAYER_STAND_L01, e_SPRITE::ePLAYER_STAND_L_MAX, 5);
			}

			break;
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

	// 그림자
	SpriteDib.DrawSprite(eSHADOW, this->m_iXpos, this->m_iYpos, pDestDib, DestWidth, DestHeight, pitch);

	// 현재 실행해야될 스프라이트 인덱스값이다.
	SpriteDib.DrawSprite(this->m_dwSpriteNow, this->m_iXpos, this->m_iYpos, pDestDib, DestWidth, DestHeight, pitch);	

	// 채력 게이지
	SpriteDib.DrawSprite(eGUAGE_HP, this->m_iXpos-35, this->m_iYpos+9, pDestDib, DestWidth, DestHeight, pitch,this->m_chHP);

}

// 체력을 셋팅해주는 함수입니다. 
void CPlayerObject::SetHp(BYTE Hp)
{
	this->m_chHP = Hp;
}

CPlayerObject::CPlayerObject()
{
	this->deleteCheck = false;

	this->m_ObjectType = e_ObjType::eType_Player;

	this->m_chHP = 100;

	// 키보드 입력 메시지
	this->m_ActionInput = KeyList::eACTION_STAND;

	// 지금 입력받은 메시지
	this->m_dwActionCur = KeyList::eACTION_STAND;
	
	// 방향 오른쪽 디폴트
	this->m_dwDirCur = e_PlayerDir::eRight;
}

CPlayerObject::~CPlayerObject()
{
}


