#include "stdafx.h"
#include "PlayerArrowShoot.h"
#include "../Player.h"


CPlayerArrowShoot::CPlayerArrowShoot()
{
}


CPlayerArrowShoot::~CPlayerArrowShoot()
{
}

void CPlayerArrowShoot::Init()
{
	//弓を打つモーション
	m_pPlayerGetter->GetAnimation().Play(enPlayerAnimationArrowShoot, 0.5f);
	m_pPlayerGetter->SetIsAttack(true);
	m_isShoot = false;
}

void CPlayerArrowShoot::Update()
{
	if (Pad().GetRightTrigger())
	{
		m_isShoot = true;
	}


	if (!m_pPlayerGetter->GetAnimation().IsPlay())
	{
		//連射時の処理
		if (m_isShoot)
		{
			GetPlayer().GetStateMachine().SetState(CPlayerState::enPlayerStateArrowAttack);
		}
		else
		{
			m_pPlayer->GetStateMachine().SetState(CPlayerState::enPlayerStateStand);
			m_pPlayerGetter->SetIsAttack(false);

		}
	}


}
