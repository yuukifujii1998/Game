/*
*	エネミーの待機クラス
*/

#pragma once

#include "IEnemyState.h"

class CEnemyIdle : public IEnemyState 
{
public:
	//コンストラクタ
	//enemy		エネミーのインターフェースクラスのポインタ
	//esm		エネミーのステートマシンクラスのポインタ
	CEnemyIdle(IEnemy* enemy, CEnemyStateMachine* esm):
		IEnemyState(enemy, esm)
	{
	}

	//デストラクタ
	~CEnemyIdle() {}

	//更新する前に一度だけ呼ばれる
	bool Start() override;

	//更新
	void Update() override;

private:
	float m_timer = 0.0f;	//タイマー
};