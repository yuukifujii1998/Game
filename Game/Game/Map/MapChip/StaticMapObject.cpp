#include "stdafx.h"
#include "StaticMapObject.h"
#include "../../Player/Player.h"
#include "../../Player/WireAction.h"
#include "../../Enemy/PathFinding/PathFinding.h"

StaticMapObject::StaticMapObject() :
	m_rigidBody(),
	m_boxCollider()
{
	this->SetIsActive(false);
}

StaticMapObject::~StaticMapObject()
{
}

void StaticMapObject::Init(const CVector3& position, const CQuaternion& rotation, const wchar_t* modelName, const bool istree, const bool collider, CAnimation* anim)
{
	MapChip::Init(position, rotation, modelName,istree,collider);

	SRigidBodyInfo rInfo;
	m_isTree = istree;
	rInfo.mass = 0.0f;
	rInfo.pos = m_position;
	rInfo.rot = m_rotation;
	CQuaternion multi = CQuaternion::Identity;
	//multi.SetRotationDeg(CVector3::AxisX, -90.0f);
	rInfo.rot.Multiply(multi);
	m_skinModel.Update(m_position, m_rotation, m_scale);
	//メッシュコライダーからAABBを作成
	isCollider = collider;

	if (m_isTree)
	{
		CMatrix rotMat;
		rotMat.MakeRotationFromQuaternion(m_rotation);
		CMeshCollider mesh;
		mesh.CreateCollider(&m_skinModel);
		CVector3 boxsize = (mesh.GetAabbMax() - mesh.GetAabbMin()) / 2.0f;
		CVector3 pos = (mesh.GetAabbMax() + mesh.GetAabbMin()) / 2.0f;
		//pos.Mul(rotMat);
		rInfo.pos.y = pos.y + m_position.y;
		m_boxCollider.reset(new CBoxCollider);
		m_boxCollider->Create({ 1.0f,boxsize.y,1.0f });
		rInfo.collider = m_boxCollider.get();
	}
	//メッシュコライダー
	else if (!collider)
	{
		m_meshCollider.reset(new CMeshCollider);
		m_meshCollider->CreateCollider(&m_skinModel);
		rInfo.collider = m_meshCollider.get();
		
		/*CMatrix rotMat;
		rotMat.MakeRotationFromQuaternion(multi);
		CMeshCollider mesh;
		mesh.CreateCollider(&m_skinModel);
		CVector3 boxsize = (mesh.GetAabbMax() - mesh.GetAabbMin()) / 2.0f;
		boxsize.y += 1.0f;
		CVector3 pos = (mesh.GetAabbMax() + mesh.GetAabbMin()) / 2.0f;
		pos.y -= 1.0f;
		pos.Mul(rotMat);
		rInfo.pos = pos + m_position;
		m_boxCollider.reset(new CBoxCollider);
		m_boxCollider->Create({ boxsize.x,boxsize.y,boxsize.z });
		rInfo.collider = m_boxCollider.get();
		g_pathFinding.GetNavigationMesh().SetSkinModel(&m_skinModel);*/
	}
	//AABB
	else
	{
		CMatrix rotMat;
		rotMat.MakeRotationFromQuaternion(m_rotation);
		CMeshCollider mesh;
		mesh.CreateCollider(&m_skinModel);
		CVector3 boxsize = (mesh.GetAabbMax() - mesh.GetAabbMin()) / 2.0f;
		CVector3 pos = (mesh.GetAabbMax() + mesh.GetAabbMin()) / 2.0f;
		pos.Mul(rotMat);
		rInfo.pos = pos + m_position;
		m_boxCollider.reset(new CBoxCollider);
		m_boxCollider->Create({ boxsize.x,boxsize.y,boxsize.z });
		rInfo.collider = m_boxCollider.get();
	}


	

	//剛体を作成
	m_rigidBody.reset(new CRigidBody);
	m_rigidBody->Create(rInfo);
	GetPlayer().GetWireAction().Add(m_skinModel);
	//m_skinModel.SetShaderTechnique(enShaderTechniqueDithering);
	m_skinModel.Update(m_position, m_rotation, m_scale);
	this->SetIsActive(true);
}


void StaticMapObject::Update()
{
	MapChip::Update();
}

void StaticMapObject::Draw()
{
	MapChip::Draw();
	if (isCollider || m_isTree)
	{
		m_rigidBody->Draw();
	}
}