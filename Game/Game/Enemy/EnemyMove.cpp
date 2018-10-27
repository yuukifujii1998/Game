#include "stdafx.h"
#include "EnemyMove.h"
#include "IEnemy.h"
#include "../Player/Player.h"
#include "EnemyState/EnemyState.h"
#include "EnemyGroup.h"

bool CEnemyMove::Start()
{
	m_characterController.Init(0.5f, 0.9f, m_enemy->GetPosition());
	m_characterController.SetGravity(-90.0f);

	return true;
}

void CEnemyMove::Update()
{
	CVector3 moveSpeed = m_enemy->GetMoveSpeed();
	float speed = 2.0f;

	CEnemyState::EnState state = m_enemy->GetState();
	if (state == CEnemyState::enState_Walk) 
	{
		if (!m_isMove) {
			//ランダムに移動先を決定
			float randomPositionX = (float)Random().GetRandDouble();
			float randomPositionZ = (float)Random().GetRandDouble();
			//0.0〜20.0の値に変換
			randomPositionX *= 20.0f;
			randomPositionZ *= 20.0f;
			//-10.0〜10.0の値に変換
			randomPositionX -= 10.0f;
			randomPositionZ -= 10.0f;
			//移動先のベクトルを計算
			CVector3 toRandomPosition;
			toRandomPosition.x = randomPositionX;
			toRandomPosition.y = 0.0f;
			toRandomPosition.z = randomPositionZ;

			//移動先の座標を保存
			m_destination = m_enemy->GetEnemyGroup()->GetPosition() + toRandomPosition;

			m_isMove = true;
		}
		CVector3 position = m_enemy->GetPosition();
		CVector3 distance = m_destination - position;
		distance.y = 0.0f;
		if (distance.Length() > 0.3f) 
		{
			distance.Normalize();
			distance *= speed;
			moveSpeed.x = distance.x;
			moveSpeed.z = distance.z;
		}
		else
		{
			m_isMove = false;
		}
	}
	else if (state == CEnemyState::enState_Chase) 
	{
		//プレイヤーを追いかける
		CVector3 playerPos = GetPlayer().GetPosition();
		CVector3 toPlayerDir = playerPos - m_enemy->GetPosition();
		toPlayerDir.Normalize();
		toPlayerDir *= speed;
		moveSpeed.x = toPlayerDir.x;
		moveSpeed.z = toPlayerDir.z;
	}
	else
	{
		moveSpeed.x = 0.0f;
		moveSpeed.z = 0.0f;
	}
	m_characterController.SetMoveSpeed(moveSpeed);

	m_characterController.SetPosition(m_enemy->GetPosition());
	m_characterController.Execute(GameTime().GetDeltaFrameTime());
	m_enemy->SetPosition(m_characterController.GetPosition());
}