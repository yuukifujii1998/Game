#pragma once

class CPlayer;
//νπΗ·ιNX

class CWeapon
{
public:
	enum EnPlayerWeapon
	{
		enSword,			//Πθ
		enLongSword,		//Όθ
		enArrow,				//|ξ
		enTwinSword,		//ρ
		enWeaponNum
	};

	void Init(CPlayer* player);

	void Update();

	void Draw();

	EnPlayerWeapon GetCurrentState() const
	{
		return m_weaponState;
	}

	const CVector3& GetPosition() const
	{
		return m_worldPos[m_weaponState];
	}

	struct SWeaponStatus
	{
		int attack = 0;
		int diffence = 0;
		EnPlayerWeapon weaponNum;
	};

	/*
	νΜXe[^Xπέθ
	status		νΜXe[^X
	*/
	void SetStatus(SWeaponStatus& status)
	{
		m_weaponStatus[status.weaponNum] = status;
	}
	/*
	νΜXe[^XπζΎ
	weaponNum	νΜνή
	*/
	const SWeaponStatus& GetStatus(EnPlayerWeapon weaponNum) const
	{
		return m_weaponStatus[weaponNum];
	}

private:
	SWeaponStatus			m_weaponStatus[enWeaponNum];
	const CMatrix*			m_boneMat = nullptr;
	CPlayer*				m_pPlayer = nullptr;
	CVector3				m_position[enWeaponNum];
	CVector3				m_attackPosition[enWeaponNum];
	CQuaternion				m_rotation[enWeaponNum];
	CQuaternion				m_attackRotation[enWeaponNum];
	CVector3				m_worldPos[enWeaponNum];
	CSkinModel				m_weaponskin[enWeaponNum];								//νΜXLf
	CBoxCollider			m_weaponBoxCollider;									//νpΜ{bNXRC_[
	CRigidBody				m_weaponRigitBody;										//{bNXpΜrigidBody
	EnPlayerWeapon			m_weaponState = EnPlayerWeapon::enSword;
};