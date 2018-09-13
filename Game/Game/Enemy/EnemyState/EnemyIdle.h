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
	bool Start();

	//更新
	void Update();

private:
	float m_timer = 0.0f;
};