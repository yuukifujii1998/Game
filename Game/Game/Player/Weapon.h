#pragma once

class CPlayer;

class CWeapon
{
public:
	enum EnPlayerWeapon
	{
		enWeaponSword,			//Πθ
		enWeaponLongSword,		//Όθ
		enWeaponArrow,				//|ξ
		enWeaponTwinSword,		//ρ
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

private:
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
	EnPlayerWeapon			m_weaponState = EnPlayerWeapon::enWeaponSword;
};