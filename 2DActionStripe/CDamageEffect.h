#pragma once

class CDamageEffect : public CBaseObject
{
public:

	CDamageEffect(unsigned short usX, unsigned short usY,DWORD actionCur);

	virtual ~CDamageEffect();

	DWORD m_EffectTiming;

	DWORD m_TimeSet;

	bool m_RenderCheck;

	void Update();

	void Render();

};

