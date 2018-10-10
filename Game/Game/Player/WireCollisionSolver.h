/*
*	ワイヤーの当たり判定クラス
*/

#pragma once

#include "../../../osumaruEngine/Physics/Collider/CapsuleCollider.h"

class CWireCollisionSolver
{
public:
	//コンストラクタ
	CWireCollisionSolver();

	//デストラクタ
	~CWireCollisionSolver();

	//初期化
	void Init(float radius, float height);

	//コリジョン解決の実行
	//position	プレイヤーの座標
	//target	ワイヤーの移動先の座標
	bool Execute(const CVector3& position, const CVector3 target);

private:
	CCapsuleCollider	m_collider;			//コライダー
	float				m_radius = 0.0f;	//半径
	float				m_height = 0.0f;	//高さ
};