#pragma once
#include "ICollider.h"
//球形状のコライダークラス

class CSphereCollider : public ICollider
{
public:

	//コンストラクタ
	CSphereCollider();
	
	//デストラクタ
	~CSphereCollider();

	/*
	コライダーを作成
	radius		球の半径
	*/
	void Create(float radius);

	//コライダーを取得
	const btSphereShape* GetBody() const override
	{
		return m_sphereShape.get();
	}

private:
	std::unique_ptr<btSphereShape> m_sphereShape;	//球のコライダー
};