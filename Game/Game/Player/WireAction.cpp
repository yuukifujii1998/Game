#include "WireAction.h"
#include "Player.h"
#include "../Enemy/IEnemy.h"
#include "../Scene/SceneManager.h"
#include "../Scene/GameScene.h"
#include "../Map/Map.h"
#include "../Camera/GameCamera.h"

void CWireAction::Init(const CPlayer* player)
{
	m_pPlayer = player;
	m_wireCollisionSolver.Init(0.3f, 1.0f);
	for (int i = 0; i < WIRE_POS_LIST_NUM; i++)
	{
		m_wirePositionList[i].wirePos = CVector3::Zero;
		m_wirePositionList[i].value = 0.0f;
	}
}

void CWireAction::Add(const CSkinModel& skinModel)
{
	const CSkelton* skelton = skinModel.GetSkelton();
	if (skelton != nullptr)
	{
		//ボーンから飛ぶ先の地点を登録
		int boneNum = skinModel.GetSkelton()->GetBoneNum();
		for (int i = 0; i < boneNum; i++)
		{
			m_posWireFly.push_back((CVector3*)(skelton->GetBoneWorldMatrix(i).m[3]));
		}
	}
}

void CWireAction::Update()
{
	int wireListCount = 0;
	//ワイヤーの処理
	bool isWireAction = false;
	if (!m_isWireMove && GetPlayer().GetIsAction())
	{
		if (Pad().IsTriggerButton(enButtonY))
		{
			m_state = enStateEnemy;
			isWireAction = true;
		}
		else if (Pad().IsTriggerButton(enButtonRightTrigger))
		{
			m_state = enStateMap;
			isWireAction = true;
		}
	}
	if (isWireAction)
	{
		float minLength = FLT_MAX;
		std::list<IEnemy*> enemyList;
		//ワイヤーを飛ばす先を決める
		switch (m_state)
		{
		case enStateEnemy:
			enemyList = GetSceneManager().GetMap()->GetEnemyList();
			for (auto& enemy : enemyList)
			{
				if (enemy->GetIsDead())
				{
					continue;
				}
				CVector3 enemyPos = enemy->GetPosition();
				CVector3 toEnemyPos = enemyPos - GetPlayer().GetPosition();
				float length = toEnemyPos.Length();
				if (minLength > length)
				{
					minLength = length;
					for (int i = WIRE_POS_LIST_NUM - 2; 0 <= i; i--)
					{
						m_wirePositionList[i + 1].wirePos = m_wirePositionList[i].wirePos;
						m_wirePositionList[i + 1].value = m_wirePositionList[i].value;
					}
					m_wirePositionList[0].wirePos = enemyPos;
					m_wirePositionList[0].value = minLength;
					wireListCount++;
				}
			}
			break;
		case enStateMap:
			const CCamera& gameCamera = GetGameCamera().GetCamera();
			CVector3 frontVec = gameCamera.GetTarget() - gameCamera.GetPosition();
			frontVec.Normalize();
			CVector3 rightVec;
			rightVec.Cross(CVector3::AxisY, frontVec);
			rightVec.Normalize();
			CVector3 playerDir;
			playerDir = frontVec * Pad().GetLeftStickY();
			playerDir += rightVec * Pad().GetLeftStickX();
			playerDir.Normalize();
			float score = 0.0f;
			for (const CVector3* pos : m_posWireFly)
			{
				CVector3 distance =  *pos - m_pPlayer->GetPosition();
				float len = distance.Length();
				distance.Normalize();
				float localScore = (100.0f - len) / 100.0f * 0.3f;
				float dt = max(distance.Dot(playerDir), 0.0f);
				localScore += dt * 0.7f;
				if (score < localScore)
				{
					for (int i = WIRE_POS_LIST_NUM - 2; 0 <= i; i--)
					{
						m_wirePositionList[i + 1].wirePos = m_wirePositionList[i].wirePos;
						m_wirePositionList[i + 1].value = m_wirePositionList[i].value;
					}
					m_wirePositionList[0].wirePos = *pos;
					m_wirePositionList[0].value = dt;
					wireListCount++;
					score = localScore;
				}
			}
			break;
		}
		if (WIRE_POS_LIST_NUM < wireListCount)
		{
			wireListCount = WIRE_POS_LIST_NUM;
		}
		for (int i = 0; i < wireListCount; i++)
		{
			if (!m_wireCollisionSolver.Execute(m_pPlayer->GetPosition(), m_wirePositionList[i].wirePos))
			{
				m_wirePosition = m_wirePositionList[i].wirePos;
				//レイを飛ばしてプレイヤーとの間に障害物がないならワイヤーを使う
				m_isWireMove = true;
				minLength = FLT_MAX;
				switch (m_state)
				{
				case enStateEnemy:
					WIRE_OFFSET_Y = 1.0f;
					m_wirePosition.y += WIRE_OFFSET_Y;
					for (auto& enemy : enemyList)
					{
						CVector3 enemyPos = enemy->GetPosition();
						CVector3 toMovePos = m_wirePosition - enemyPos;
						float length = toMovePos.Length();
						if (length < minLength) 
						{
							minLength = length;
							m_hitEnemy = enemy;
						}
					}
					//ワイヤーが当たったエネミーにフラグを設定する
					m_hitEnemy->SetIsWireHit(true);
					break;

				case enStateMap:
					break;
				}
				break;
			}
		}
	}
}