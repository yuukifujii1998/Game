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
	m_pPlayer->SetPlayerAnimation(enPlayerAnimationArrowShoot, 0.5f);
	m_pPlayer->SetAttack(true);
	m_isShoot = false;
}

void CPlayerArrowShoot::Update()
{
	if (Pad().GetRightTrigger())
	{
		m_isShoot = true;
	}


	if (!GetPlayer().GetAnimation().IsPlay())
	{
		//連射時の処理
		if (m_isShoot)
		{
			GetPlayer().GetPlayerStateMachine().SetState(CPlayerState::enPlayerStateArrowAttack);
		}
		else
		{
			m_pPlayer->GetPlayerStateMachine().SetState(CPlayerState::enPlayerStateStand);
			m_pPlayer->SetAttack(false);

		}
	}


}
