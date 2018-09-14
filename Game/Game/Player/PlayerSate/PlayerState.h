#pragma once


class CPlayerState 
{
public:
	enum EnPlayerState 
	{
		enPlayerStand,		//待機アニメーション
		enPlayerWalk,		//歩行アニメーション
		enPlayerRun,		//走りアニメーション
		enPlayerJump,		//ジャンプアニメーション
		enPlayerAttack,		//攻撃アニメーション
		enPlayerDamage,		//ダメージアニメーション
		enPlayerAvoidance,	//回避アニメーション
		enPlayerDete,		//死亡アニメーション
		enPlayerArroAttack,	//弓のアニメーション
		enPlayerNum	,		//アニメーションの数
		enState_Invald,		//何もない
	};
};