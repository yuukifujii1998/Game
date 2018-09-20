#include "stdafx.h"
#include "EnemyDamage.h"
#include "../IEnemy.h"
#include "../../Player/Player.h"
#include "../EnemyGroup.h"

bool CEnemyDamage::Start()
{
	//ダメージを受けたフラグを戻す
	m_enemy->SetIsDamage(false);

	//ダメージアニメーションを再生
	m_enemy->PlayAnimation(CEnemyState::enState_Damage);

	//ダメージ計算
	int playerStrength = GetPlayer().GetStatus().Strength;
	int enemyDefence = m_enemy->GetStatus().Defense;
	int damage = playerStrength - enemyDefence;
	m_enemy->ReduceHp(damage);
	m_enemy->DamageCalculation(damage);

	return true;
}

void CEnemyDamage::Update()
{
	//ダメージを受けているときは動かない
	CVector3 moveSpeed = m_enemy->GetMoveSpeed();
	moveSpeed.x = 0.0f;
	moveSpeed.z = 0.0f;
	m_enemy->SetMoveSpeed(moveSpeed);

	CVector3 playerPos = GetPlayer().GetPosition();
	CVector3 toPlayerPos = playerPos - m_enemy->GetPosition();
	float length = toPlayerPos.Length();

	if (!m_enemy->IsPlayAnimation()) {
		//アニメーションが終了していればプレイヤーを追いかける
		m_esm->ChangeState(CEnemyState::enState_Chase);
		//ダメージ表示の描画をやめる
		m_enemy->DamageIndicateReset();
	}
	if (m_enemy->GetStatus().Hp <= 0) {
		//HPが無くなれば死亡
		m_esm->ChangeState(CEnemyState::enState_Death);
		//ダメージ表示の描画をやめる
		m_enemy->DamageIndicateReset();
	}
}
