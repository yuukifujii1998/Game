#include "stdafx.h"
#include "BreakMapObject.h"
#include "../../Enemy/Maw.h"

CBreakMapObject::CBreakMapObject():
	m_rigidBody(),
	m_boxCollider()
{
}

CBreakMapObject::~CBreakMapObject()
{
}

void CBreakMapObject::Init(const CVector3& position, const CQuaternion& rotation, const wchar_t* modelName, const bool collider, CAnimation* anim)
{
	MapChip::Init(position, rotation, modelName, collider);

	SRigidBodyInfo rInfo;

	//メッシュコライダーからAABBを作成
	/*if (!collider)
	{
		m_meshCollider.reset(new CMeshCollider);
		m_meshCollider->CreateCollider(&m_skinModel);
		rInfo.collider = m_meshCollider.get();
	}
	else
	{*/
		CMeshCollider mesh;
		mesh.CreateCollider(&m_skinModel);
		CVector3 boxsize = (mesh.GetAabbMax() - mesh.GetAabbMin());
		boxsize.x /= 2.0f;
		boxsize.z /= 2.0f;
		m_boxCollider.reset(new CBoxCollider);
		m_boxCollider->Create({ boxsize.x,boxsize.y,boxsize.z });
		rInfo.collider = m_boxCollider.get();
	//}



	rInfo.mass = 1.0f;
	rInfo.pos = m_position;
	rInfo.rot = m_rotation;

	//剛体を作成
	m_rigidBody.reset(new CRigidBody);
	m_rigidBody->Create(rInfo);
	m_skinModel.Update(m_position, m_rotation, m_scale);
	//m_boxCollider.get()->GetBody()->setUserIndex(m_boxCollider.get()->GetBody()->getUserIndex());
	//m_rigidBody.get()->SetCollisionFlags(m_rigidBody.get()->GetBody()->CF_KINEMATIC_OBJECT);
	m_rigidBody.get()->SetUserIndex(m_rigidBody.get()->GetBody()->CF_KINEMATIC_OBJECT);

}


void CBreakMapObject::Update()
{
	float killZ = 30.0f;
	float fallinSpeed = 0.02f;
	float breakMaxLength = 3.0f;

	//攻撃していたら
	if (GetMaw().GetIsAttack())
	{
		CVector3 BossLeftHandPos = GetMaw().GetLeftHandBone();
		CVector3 distance = BossLeftHandPos - m_position;
		float BreakLength = distance.Length();
		//腕に当たっていたらかつ壊れていなかったら
		if (BreakLength < breakMaxLength && !isBreak)
		{
			isBreak = true;
		}
	}
	/*if (GetMaw().GetBreakObjectHit()&&!isBreak)
	{
		isBreak = true;
	}*/
	//
	//下に落としていく処理
	if (isBreak) 
	{
		m_position.y -= fallinSpeed;
		//killZより下だったら消去
		if (m_position.y < killZ)
		{
			this->MapChipDelete();
		}
	}
	
		
	MapChip::Update();
	//剛体の座標と回転を更新
	m_rigidBody.get()->SetPosition(m_position);
	//m_rigidBody.get()->SetRotation(m_rotation);
	//スキンモデルを更新
	m_skinModel.Update(m_position, m_rotation, m_scale);
}

void CBreakMapObject::Draw()
{
	MapChip::Draw();
}