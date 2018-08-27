#include "stdafx.h"
#include "Player.h"
#include "../GameCamera.h"

void Player::Init(Vector3 position)
{
	m_skinmodel.Load(L"Assets/modelData/UnityChan.cmo");
	m_position = position;
	m_characterController.Init(2.0f, 2.0f, m_position);
	m_characterController.SetGravity(0.0f);
	Add(this, 0);
}

void Player::Update()
{

	Move();

	Vector3 playerVec = m_moveSpeed;


	if (playerVec.LengthSq() > 0.001f)
	{
		
		m_rotation.SetRotation(Vector3::AxisY, atan2f(playerVec.x, playerVec.z));

	}

	m_skinmodel.Update(m_position, m_rotation, { 1.0f, 1.0f, 1.0f });
}

void Player::Draw()
{
	m_skinmodel.Draw(GetGameCamera().GetViewMatrix(), GetGameCamera().GetProjectionMatrix());
}

void Player::Move()
{
	

		Vector3 moveSpeed;
		moveSpeed.z = GetPad().GetLeftStickY();
		moveSpeed.x = GetPad().GetLeftStickX();

		Matrix cameraVm = GetGameCamera().GetViewMatrix();
		cameraVm.Inverse();	//カメラのビュー行列の逆行列

		//カメラの前方向
		Vector3 cameraZ;
		cameraZ.x = cameraVm.m[2][0];
		cameraZ.y = 0.0f;
		cameraZ.z = cameraVm.m[2][2];
		cameraZ.Normalize();

		//カメラの横方向
		Vector3 cameraX;
		cameraX.x = cameraVm.m[0][0];
		cameraX.y = 0.0f;
		cameraX.z = cameraVm.m[0][2];
		cameraX.Normalize();


		m_moveSpeed.x = cameraX.x * moveSpeed.x + cameraZ.x * moveSpeed.z;
		m_moveSpeed.z = cameraX.z * moveSpeed.x + cameraZ.z * moveSpeed.z;



		m_characterController.SetMoveSpeed(m_moveSpeed);
		m_characterController.SetPosition(m_position);
		m_characterController.Execute(GetGameTime().GetDeltaFrameTime());

		m_position = m_characterController.GetPosition();


	
}

void Player::Rotation()
{



}