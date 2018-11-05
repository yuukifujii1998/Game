#pragma once
#include "PlayerSate/PlayerStateMachine.h"
#include "PlayerSate/PlayerState.h"
#include "PlayerArrow.h"
#include "Weapon.h"
#include "PlayerSate/PlayerStateCommon.h"
#include "WireAction.h"

class IItem;

struct SplayerStatus
{
	int Strength;			//攻撃力
	int Defense;			//防御力
	int	Health;				//体力
	int MaxHealth;			//レベルごとの最大HP
	int Level;				//レベル
	int ExperiencePoint	;	//現在の経験値
	int NextExp;			//レベルアップまでに必要な経験値
	int OldExp;				//現在のレベルアップに必要だった経験値
	int AccumulationExp;	//累積経験値
	int Gold;
};



class CPlayer : public IGameObject
{

public:

	//プレイヤーの初期化
	void Init(CVector3 position);

	//プレイヤーのインスタンスの取得
	static CPlayer& GetInstance()
	{
		return *m_player;
	}

	//インスタンスの生成
	static void CPlayer::Create()
	{
		if (!m_player)
		{
			m_player = New<CPlayer>(1);
		}
	}

	//インスタンスの消去
	static void CPlayer::Destroy()
	{
		Delete(m_player);
		m_player = nullptr;
	}

	//プレイヤーの更新関数
	void Update()override;

	//プレイヤーの描画関数
	void Draw()override;

	void AfterDraw();

	//プレイヤーの座標を取得する関数
	const CVector3& GetPosition() const
	{
		return m_position;
	}
	//プレイヤーの座標を設定する関数
	void SetPosition(const CVector3 setpos)
	{
		 m_characterController.SetPosition(setpos);
	}

	//プレイヤーのスピードを取得する
	const CVector3& GetMoveSpeed() const
	{
		return m_characterController.GetMoveSpeed();
	}

	//移動速度を設定
	void SetMoveSpeed(const CVector3& moveSpeed)
	{
		m_characterController.SetMoveSpeed(moveSpeed);
	}

	//プレイヤーのステータスを取得
	const SplayerStatus& GetStatus() const
	{
		return m_status;
	}

	//プレイヤーのワールド行列を取得
	const CMatrix& GetWorldMatrix() const
	{
		return m_skinmodel.GetWorldMatrix();
	}

	//ステートマシーンの取得
	CPlayerStateMachine& GetPlayerStateMachine()
	{
		return m_PlayerStateMachine;
	}

	//武器を管理するクラスを取得
	const CWeapon& GetWeapon() const
	{
		return m_weapon;
	}

	//プレイヤーのスキンモデルの情報を取得
	const CSkinModel& GetPlayerSkin() const
	{
		return m_skinmodel;
	}

	//プレイヤーの頭のボーンの位置を取得
	CVector3 GetPlayerHead() const
	{

		CMatrix PlayerHead = m_skinmodel.FindBoneWorldMatrix(L"Head");
		CVector3 PlayerHeadPos = { PlayerHead.m[3][0],PlayerHead.m[3][1],PlayerHead.m[3][2] };
		return PlayerHeadPos;
	}

	//アニメーションの情報を取得
	CAnimation& GetAnimation()
	{
		return m_animation;
	}

	//ステータスの計算処理を行う関数
	void StatusCalculation();

	/*プレイヤーの経験値量を上げる関数（セットする関数ではないです）
	引数で倒した敵の経験値量を入れる*/
	void ExpUP(const int expup)
	{
		m_status.ExperiencePoint += expup;
		m_status.AccumulationExp += expup;
	}

	//プレイヤーのHP回復をする
	//recoveryValue		回復値
	void RecoveryHP(const int recoveryValue)
	{
		m_status.Health += recoveryValue;
		if (m_status.Health >= m_status.MaxHealth) {
			m_status.Health = m_status.MaxHealth;
		}
		m_isStatusConversion = true;
	}

	//プレイヤーがお金を取得する
	//gold		お金
	void GainGold(const int gold)
	{
		m_status.Gold += gold;
	}

	//プレイヤーのダメージ処理
	void GetDamage()
	{
		if (!m_isDamege && !m_intervalOn)
		{
			m_status.Health -= 5;
			m_isDamege = true;
		}
	}

	//プレイヤーが生きているかの判定
	bool GetIsDied() const
	{
		return m_isDied;
	}

	//死亡したかを設定
	void SetIsDeid(bool SetDied)
	{
		m_isDied = SetDied;
	}

	//ダメージを受けたかの判定
	bool GetIsDamage() const
	{
		return m_isDamege;
	}

	//ダメージを受けたかの設定
	void SetIsDamage(bool SetDamage)
	{
		m_isDamege = SetDamage;
	}

	//攻撃中かを取得
	bool GetIsAttack() const
	{
		return m_isAttack;
	}

	//攻撃をしたかの設定
	void SetIsAttack(bool SetA)
	{
		m_isAttack = SetA;
	}

	//無敵時間を設定する
	void SetInterval(const bool set)
	{
		m_intervalOn = set;
	}

	//キャラクターコントローラーを取得
	CCharacterController& GetCharacterController()
	{
		return m_characterController;
	}

	//矢を生成しているかの設定
	void SetInitArrow(const bool set)
	{
		m_initArrow = set;
	}

	//所持アイテムリストに追加
	//item		アイテムリストに追加するアイテム
	void AddItemList(IItem* item);

	//所持装備リストに追加
	//item		装備リストに追加するアイテム
	void AddEquipList(IItem* item);

	//所持アイテムリストを取得
	std::list<IItem*> GetItemList()
	{
		return m_itemList;
	}

	//所持装備リストを取得
	std::list<IItem*> GetEquipList()
	{
		return m_equipList;
	}

	//所持アイテムを使う
	//number		アイテムの番号
	void UseItem(int number);

	//装備の変更
	//number		変更したい装備の番号
	void ChangeEquip(int number);

	/*
	アニメーションを補間つきで再生する関数
	state				アニメーションの番号
	interporationTime	補間時間
	*/
	void PlayAnimation(EnPlayerAnimation state, float interporationTime)
	{
		m_state = state;
		m_animation.Play(state, interporationTime);
	}

	/*
	アニメーション再生関数
	state		アニメーションの番号
	*/
	void PlayAnimation(EnPlayerAnimation state)
	{
		m_state = state;
		m_animation.Play(m_state);
	}
	//買い物をした時の計算を行う
	bool BuyMoney(const int buy)
	{
		if (m_status.Gold < buy) { return false; }
		m_status.Gold -= buy;
		return true;
	}

	//弓を生成する関数
	void InitArrow();
	//ステータスが変化したかを取得
	const bool GetIsStatusConversion()
	{
		return m_isStatusConversion;
	}
	//ステータスが変化したかを設定
	void  SetIsStatusConversion(const bool iscon)
	{
		m_isStatusConversion = iscon;
	}

	CWireAction& GetWireAction()
	{
		return m_wireAction;
	}

private:

	//プレイヤーがエネミーに攻撃する処理
	void PlayerAttack();

	//プレイヤーの回転をする関数
	void Rotation();

	//アニメーションイベントが起きた時に呼ばれる処理。
	void OnInvokeAnimationEvent(const wchar_t* animClipName, const wchar_t* eventName);
private:
	static CPlayer*				m_player;								//プレイヤー
	CWeapon					m_weapon;
	CVector3				m_position;									//座標
	CQuaternion				m_rotation = CQuaternion::Identity;			//回転
	CSkinModel				m_skinmodel;								//スキンモデル
	CCharacterController	m_characterController;						//キャラクターコントローラー
	CAnimation				m_animation;								//アニメーション
	SplayerStatus			m_status;									//プレイヤーのステータス
	bool					m_isSlip = false;							//スリップ判定
	float					m_slipSpeed = 2.0f;							//回避移動時のスピード
	EnPlayerAnimation		m_state = enPlayerAnimationStand;			//アニメーションを遷移させるための変数
	const float				INTERVAL = 1.5;								//ダメージを受けた後の無敵時間
	bool					m_isDamege = false;
	float					m_animetionFrame = 0.0f;
	bool					m_isAttack = false;							//攻撃中かの判定
	bool					m_isDied = false;							//死んでいるかの判定
	bool					m_intervalOn = false;						//無敵中かの判定
	bool					m_initArrow = false;						//弓を生成しているかの判定
	bool					m_isStatusConversion = false;				//ステータスが変化したかを判定する
	float					m_intervalTime = 0.0f;

	CPlayerStateMachine			m_PlayerStateMachine;							//プレイヤーのアニメーションの遷移を行うステートマシーン
	std::list<CPlayerArrow*>	m_arrowList;									//弓矢のリスト
	CSprite						m_arrowtag;										//サークルのスプライト
	CTexture					m_arrowtexture;
	bool						m_isZoom;										//弓用の視点に切り替えるかの判定をする変数

	std::list<IItem*>		m_itemList;								//所持アイテムのリスト
	std::list<IItem*>		m_equipList;							//所持装備のリスト
	CWireAction				m_wireAction;
};

static CPlayer& GetPlayer()
{
	return CPlayer::GetInstance();
}