#include "stdafx.h"
#include "Ninja.h"
#include "../Player/Player.h"
#include "../Camera/GameCamera.h"
#include "PathFinding/RootPoint.h"
#include "PathFinding/PathFinding.h"

CNinja::CNinja()
{
}

CNinja::~CNinja()
{
}

void CNinja::Init(CVector3 position)
{
	//モデルを読み込む
	m_skinModel.Load(L"Assets/modelData/Ninja.cmo", &m_animation);
	m_skinModel.LoadNormalmap(L"Assets/modelData/Ninja_normal.png");
	m_position = position;
	wchar_t* animClip[CEnemyState::enState_Num] = {
		L"Assets/modelData/ninjaStand.tka",
		L"Assets/modelData/ninjaWalk.tka",
		L"Assets/modelData/ninjaDash.tka",
		L"Assets/modelData/ninjaAttack.tka",
		L"Assets/modelData/ninjaDamage.tka",
		L"Assets/modelData/ninjaDeath.tka"
	};
	m_animation.Init(animClip, CEnemyState::enState_Num);
	m_animation.SetLoopFlg(CEnemyState::enState_Idle, true);
	m_animation.SetLoopFlg(CEnemyState::enState_Walk, true);
	m_animation.SetLoopFlg(CEnemyState::enState_Chase, true);
	Add(&m_enemyStateMachine, 0);
	Add(&m_enemyMove, 0);
	Add(&m_enemyTurn, 0);
	Add(&m_enemySearch, 0);
	//ダメージ表示の初期化
	m_damageNumber.Init();

	//ステータスを設定
	m_status.Strength = 10;
	m_status.Defense = 0;
	m_status.Hp = 50;
	m_status.MaxHp = m_status.Hp;
	m_status.Gold = 100;
}

bool CNinja::Start()
{
	m_enemyStateMachine.SetEnemyGroup(m_enemyGroup);

	return true;
}

void CNinja::Update()
{
	//if (m_isFind && m_rootPoint != nullptr) {
	//	//見つかっていれば経路探索する
	//	std::vector<CVector2> root;
	//	g_pathFinding.FindRoot(root, m_rootPoint->GetListNumber(), GetPlayer().GetRootPoint()->GetListNumber());
	//	if (!root.empty()) {
	//		CVector3 moveSpeed = { root[0].x, 0.0f, root[0].y };
	//		CVector3 pos = m_position;
	//		pos.y = 0.0f;
	//		moveSpeed -= pos;
	//		moveSpeed.Normalize();
	//		moveSpeed *= 2.0f;
	//		m_characterController.SetMoveSpeed(moveSpeed);
	//	}
	//}

	if (!m_isWireHit) {
		m_animation.Update(GameTime().GetDeltaFrameTime() * 2.0f);
	}
 	m_skinModel.Update(m_position, m_rotation, { 1.0f, 1.0f, 1.0f }, true);
}

void CNinja::Draw()
{
	m_skinModel.Draw(GetGameCamera().GetViewMatrix(), GetGameCamera().GetProjectionMatrix());
}
