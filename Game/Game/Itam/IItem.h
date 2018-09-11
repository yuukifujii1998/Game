/*
*	アイテムクラスのインターフェース
*/

#pragma once

class IItem : public IGameObject
{
public:
	//コンストラクタ
	IItem();

	//デストラクタ
	virtual ~IItem();

	//初期化
	//position	座標
	virtual void Init(CVector3 position) = 0;

	//更新
	virtual void Update() = 0;

	//描画
	virtual void Draw() = 0;

protected:
	CSkinModel				m_skinModel;			//スキンモデル
	CVector3				m_position;				//座標
	CQuaternion				m_rotation;				//回転
	CCharacterController	m_characterController;	//キャラクターコントローラー
	CVector3				m_popPosition;			//アイテムがポップする座標
	const float				m_speed = 4.0f;			//移動速度
};