#include "stdafx.h"
#include "EnemyTurn.h"
#include "IEnemy.h"
#include "../Player/Player.h"

void CEnemyTurn::Update()
{
	//ワールド行列からモデルの前方向を取得
	CMatrix worldMatrix = m_enemy->GetWorldMatrix();
	CVector3 forwardXZ;
	forwardXZ.x = worldMatrix.m[2][0];
	forwardXZ.y = 0.0f;
	forwardXZ.z = worldMatrix.m[2][2];
	forwardXZ.Normalize();

	//移動速度を取得
	CVector3 moveSpeed = m_enemy->GetMoveSpeed();
	moveSpeed.y = 0.0f;
	const CEnemyDamage* enemyDamage = dynamic_cast<const CEnemyDamage*>(m_enemy->GetStateMachine().GetState(CEnemyState::enState_Damage));
	if (moveSpeed.LengthSq() < 0.01f || enemyDamage->IsNockBack()) {
		//移動していないと角度の計算ができないので返す
		return;
	}

	//角度の計算
	float targetAngle = atan2f(moveSpeed.x, moveSpeed.z);
	float currentAngle = atan2f(forwardXZ.x, forwardXZ.z);
	
	float diff = targetAngle - currentAngle;
	float a_diff = fabsf(diff);
	if (a_diff > CMath::PI) {
		//180°を超えているので、-180.0f〜180.0fに変換する
		diff = (CMath::PI2 - a_diff) * (a_diff / -diff);
		a_diff = fabsf(diff);
	}
	float rotSpeed = 10.0f * GameTime().GetDeltaFrameTime();
	float addAngle = 0.0f;
	if (a_diff > rotSpeed) {
		addAngle = (diff / a_diff) * rotSpeed;
	}
	else {
		addAngle = diff;
	}

	CQuaternion addRot;
	addRot.SetRotation(CVector3::AxisY, addAngle);

	CQuaternion rot = m_enemy->GetRotation();
	rot.Multiply(rot, addRot);
	m_enemy->SetRotation(rot);
}
