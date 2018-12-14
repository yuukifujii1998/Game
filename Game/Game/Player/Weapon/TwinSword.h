#pragma once
#include "IWeapon.h"

class CTwinSword : public IWeapon
{
public:
	void Init()override;

	void Update()override;

	void Draw() override;

	//エネミーとの当たり判定を取る関数　二つ目の剣用
	//void EnemyAttack() override;

	//void EnemyAttackTwin();

	SWeaponEnemyAttackInfo EnemyAttackPositionDecide()override;

	//SWeaponEnemyAttackInfo EnemyAttackPositionTwinDecide();

	SWeaponTraceDrawInfo WeaponTraceDraw()override;

	//二つ目の剣用
	//SWeaponTraceDrawInfo WeaponTraceTwinDraw();

	//void WeaponTraceTwinDrawer();
private:
	
	const CMatrix*				m_normalTwinBoneMat = nullptr;	//プレイヤーのボーン行列
	const CMatrix*				m_attackTwinBoneMat = nullptr;	//プレイヤーのボーン行列
	CVector3					m_positionTwin;					//通常時の座標
	CVector3					m_attackTwinPosition;			//攻撃時の座標
	CQuaternion					m_rotationTwin;					//通常時の回転
	CQuaternion					m_attackTwinRotation;			//攻撃時の回転
	CSkinModel					m_skinModelTwin;				//双剣武器のスキンモデル
};