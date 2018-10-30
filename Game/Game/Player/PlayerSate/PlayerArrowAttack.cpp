#include "stdafx.h"
#include "PlayerArrowAttack.h"
#include "../Player.h"
#include "../../Camera/GameCamera.h"


void CPlayerArrowAttack::Init()
{
	//弓の生成
	m_pPlayer->InitArrow();
	m_pPlayer->SetPlayerAnimation(CPlayerState::enPlayerArrowAttack, 0.5f);
	m_pPlayer->SetAttack(true);
	m_pPlayer->SetMoveSpeed(CVector3::Zero);
	//弓用の視点に変更
}

void CPlayerArrowAttack::Update()
{
	if (Pad().IsPressButton(enButtonRightTrigger))
	{
		m_isCharge = true;
	}
	else
	{
		m_isCharge = false;
	}

	if (!GetPlayer().GetAnimation().IsPlay())
	{

		if (!m_isCharge)
		{

			m_pPlayer->GetPlayerStateMachine().SetState(CPlayerState::enPlayerArrowShoot);
		}
		
	}

}
