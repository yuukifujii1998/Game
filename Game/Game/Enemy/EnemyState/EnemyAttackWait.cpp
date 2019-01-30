#include "EnemyAttackWait.h"
#include "../IEnemy.h"
#include "../../Player/Player.h"

bool CEnemyAttackWait::Start()
{
	m_enemy->GetAnimation().Play(CEnemyState::enState_Idle, 0.3f);
	//タイマーを初期化
	m_timer = 0.0f;
	m_isNoneEnd = false;

	return true;
}

void CEnemyAttackWait::Update()
{
	m_timer += GameTime().GetDeltaFrameTime();

	if (m_enemy->GetIsDamage())
	{
		//ダメージを受けた
		m_esm->ChangeState(CEnemyState::enState_Damage);
	}

	if (m_timer <= 3.0f)
	{
		//しばらく何もしない
		return;
	}

	if (m_timer > 3.0f && !m_isNoneEnd)
	{
		m_isNoneEnd = true;
		m_enemy->GetAnimation().Play(CEnemyState::enAnimation_Walk, 0.3f);
	}

	CVector3 playerPos = GetPlayer().GetPosition();
	CVector3 distance = m_enemy->GetPosition() - playerPos;
	distance.y = 0.0f;
	float length = distance.Length();

	if (m_enemy->GetAttackType() == IEnemy::enAttackType_Far) {
		//遠距離攻撃ができるタイプだった
		if (length < 4.0f)
		{
			//近距離攻撃をする
			m_enemy->SetAttackLength(1.2f);
		}
		else
		{
			//遠距離攻撃をする
			m_enemy->SetAttackLength(10.0f);
		}
	}

	if (length < m_enemy->GetAttackLength()) 
	{
		bool isRange = m_enemy->CalucFanShape(10.0f, playerPos);
		if (!isRange) {
			//攻撃範囲に入っていないので回転させる
			Rotation();
		}
		else {
			//攻撃
			m_esm->ChangeState(CEnemyState::enState_Attack);
		}
	}
	else 
	{
		//追いかける
		m_esm->ChangeState(CEnemyState::enState_Chase);
	}
}

void CEnemyAttackWait::Rotation()
{
	//プレイヤーへのベクトルを求める
	CVector3 playerPos = GetPlayer().GetPosition();
	CVector3 enemyPos = m_enemy->GetPosition();
	CVector3 toPlayer = playerPos - enemyPos;
	toPlayer.Normalize();
	//エネミーの前方向を求める
	CMatrix enemyWorldMatrix = m_enemy->GetWorldMatrix();
	CVector3 enemyForward;
	enemyForward.x = enemyWorldMatrix.m[2][0];
	enemyForward.y = 0.0f;
	enemyForward.z = enemyWorldMatrix.m[2][2];
	enemyForward.Normalize();
	//角度を計算
	float angle = toPlayer.Dot(enemyForward);
	if (angle > 1.0f) {
		angle = 1.0f;
	}
	else if (angle < -1.0f) {
		angle = -1.0f;
	}
	angle = acosf(angle);
	//回転方向を決める
	CVector3 cross = toPlayer;
	cross.Cross(enemyForward);
	if (cross.y > 0.0f) {
		angle *= -1.0f;
	}
	//徐々に回転させるために分割
	angle /= 5.0f;
	//エネミーを回転させる
	CQuaternion enemyRotation = m_enemy->GetRotation();
	CQuaternion rotation;
	rotation.SetRotation(CVector3::AxisY, angle);
	enemyRotation.Multiply(rotation);
	m_enemy->SetRotation(enemyRotation);
}
