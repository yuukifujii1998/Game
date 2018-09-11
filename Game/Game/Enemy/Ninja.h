/*
*	忍者クラス
*/

#pragma once

#include "Enemy/IEnemy.h"

class CNinja : public IEnemy
{
public:
	//コンストラクタ
	CNinja();

	//デストラクタ
	~CNinja();

	//初期化
	//position	座標
	void Init(CVector3 position);

	//更新
	void Update();

	//描画
	void Draw();
};