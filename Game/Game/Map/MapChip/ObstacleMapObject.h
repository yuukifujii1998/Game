#pragma once
#include "MapChip.h"

class CObstacleMapObject : public MapChip
{
public:

	CObstacleMapObject();

	~CObstacleMapObject();
	
	/*
	初期化する関数
	position	座標
	rotation	回転
	modelName	読み込むモデルの名前
	collider	コライダーの設定　falseならメッシュ	trueならボックス
	anim		アニメーション付きのモデルの場合アニメーションを入れる入れ物
	*/
	void Init(const SMapChipInfo& mapChipInfo, CAnimation* anim = nullptr)override;

	//更新関数
	void Update()override;

	//描画関数
	void Draw()override;

	//objectのId
	//int GetBreakId()
	//{
	//	return m_breakId;
	//}

private:
	CRigidBody							m_rigidBody;		//剛体
	CBoxCollider						m_boxCollider;		//コライダー
};