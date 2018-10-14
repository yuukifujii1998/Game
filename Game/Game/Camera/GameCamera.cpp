#include "stdafx.h"
#include "GameCamera.h"
#include "Player\Player.h"
#include "../Player/PlayerSate/PlayerStateMachine.h"

void CGameCamera::Init()
{
	camera.Init();
	camera.SetFar(300.0f);
	camera.SetNear(1.0f);
	camera.SetAspect((float)FrameBufferWidth() / (float)FrameBufferHeight());
	camera.SetAngle(CMath::DegToRad(60.0f));
	camera.SetPosition({ 0.0f, 1.8f, 3.2f });
	camera.SetTarget({ 0.0f, 0.0f, 0.0f });
	camera.SetUp({ 0.0f, 1.0f, 0.0f });
	camera.Update();
	Add(this, 0);
	m_cameraVec = camera.GetPosition();
	PhysicsWorld().SetCamera(&camera);
	m_arrowCamera = { 0.0f,1.0f,1.3f };
	m_springCamera.Init(camera.GetTarget(), camera.GetPosition(), 5000.0f);
	Engine().GetDeferred().SetCamera(&camera);
}

void CGameCamera::Update()
{

	float rStick_x = Pad().GetRightStickX() * 2 * GameTime().GetDeltaFrameTime();
	float rStick_y = Pad().GetRightStickY() * GameTime().GetDeltaFrameTime();

	m_cameraVec = camera.GetPosition() - camera.GetTarget();

	if (fabsf(rStick_x) > 0.0f) {
		//Y軸周りの回転
		CMatrix matrix;
		matrix.MakeRotationY(rStick_x);
		matrix.Mul(m_cameraVec);
	}
	if (fabsf(rStick_y) > 0.0f) {
		//X軸周りの回転
		CVector3 rotAxis;
		rotAxis.Cross(CVector3::Up, m_cameraVec);
		rotAxis.Normalize();
		CMatrix matrix;
		matrix.MakeRotationAxis(rotAxis, rStick_y);
		//1フレーム前のカメラベクトル
		CVector3 cameraVecOld = m_cameraVec;

		matrix.Mul(m_cameraVec);
		CVector3 cameraDir = m_cameraVec;
		cameraDir.Normalize();

		if (cameraDir.y < -0.5f)
 {
			m_cameraVec = cameraVecOld;
		}
		else if (cameraDir.y > 0.7f) 
		{
			m_cameraVec = cameraVecOld;
		}
	}

	//プレイヤーの後ろに追従するカメラ
	{
		CVector3 position, toCameraXZ;
		position = GetPlayer().GetPosition();
		toCameraXZ = m_cameraVec;
		float height = toCameraXZ.y;
		toCameraXZ.y = 0.0f;
		float toCameraLen = toCameraXZ.Length();
		toCameraXZ.Normalize();
		//注視点の設定
		CVector3 target = GetPlayer().GetPosition();
		if (GetPlayer().GetPlayerStateMachine().GetState() == CPlayerState::EnPlayerState::enPlayerArrowAttack)
		{
			CMatrix	mat = GetPlayer().GetPlayerSkin().GetWorldMatrix();
			CVector3 playerFlont = { -mat.m[0][0],-mat.m[0][1],-mat.m[0][2] };
			playerFlont.Normalize();
			target += playerFlont * 0.5f;
			//target.x += 0.3f; //GetPlayer().GetWeaponPosition();
		}
		
		target.y += 1.5f;
		CVector3	toNewCameraPos = camera.GetPosition() - target;

		toNewCameraPos.y = 0.0f;
		toNewCameraPos.Normalize();
		float weight = 0.3f;  //このウェイトの値は0.0〜1.0の値をとる。1.0に近づくほど追尾が強くなる。
		toNewCameraPos = toNewCameraPos * weight + toCameraXZ * (1.0f - weight);
		toNewCameraPos.Normalize();
		toNewCameraPos *= toCameraLen;
		toNewCameraPos.y = height;              //高さを戻す。
		CVector3 pos = target + toNewCameraPos;  //これで新しい視点が決定。
		

		camera.SetPosition(pos);
		camera.SetTarget(target);
		/*m_springCamera.SetTarPosition(pos);
		m_springCamera.SetTarTarget(target);*/
		
	}
	//float interporation = 2.0f;
	//position.y += interporation;	
	//camera.SetTarget(position);
	//m_springCamera.SetTarTarget(position);

	////position.y -= interporation;
	//position += m_cameraVec;
	//camera.SetPosition(position);
	////m_springCamera.SetTarPosition(position);

	//m_springCamera.Update();
	camera.Update();
}