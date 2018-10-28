#include "stdafx.h"
#include "PlayerWireMove.h"
#include "../Player.h"
#include "../../Scene/SceneManager.h"
#include "../../Map/Map.h"
#include "../../Enemy/IEnemy.h"

bool CPlayerWireMove::Start()
{
	GetPlayer().SetPlayerAnimation(CPlayerState::enPlayerWireMove, 0.1f);
	m_movePosition = GetPlayer().GetWirePosition();

	return true;
}

void CPlayerWireMove::Update()
{
	bool isMoveEnd = false;
	CVector3 playerPos = GetPlayer().GetPosition();
	CVector3 toMovePos = m_movePosition - playerPos;

	float length = toMovePos.Length();
	if (length > 2.0f) {
		//�G�Ƃ̋���������Ă���Έړ���ɐi��
		toMovePos.Normalize();
		toMovePos *= m_speed;
		GetPlayer().SetMoveSpeed(toMovePos);
	}
	else {
		isMoveEnd = true;
	}

	if (isMoveEnd) {
		//�ړ����I�������G�̃t���O��߂��Ă��
		std::list<IEnemy*> enemyList = GetSceneManager().GetGameScene().GetMap()->GetEnemyList();
		for (auto& enemy : enemyList)
		{
			enemy->SetIsWireHit(false);
		}
	}

	if (isMoveEnd) {
		GetPlayer().SetIsWireMove(false);
		//�ړ����I�����
		GetPlayer().GetPlayerStateMachine().ChangeState(CPlayerState::enPlayerStand);
	}
}