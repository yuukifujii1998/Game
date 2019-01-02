/*
*	ゲームカメラクラス
*/

#pragma once
#include "NormalGameCamera.h"
#include "ArrowGameCamera.h"

class IEnemy;

class CGameCamera : public IGameObject
{
public:

	enum EnCameraState 
	{
		enNormal,
		enArrow,
		enFree,
		enNum,
	};
	//初期化
	void Init();
	
	//更新
	void Update();
	
	//カメラ必要なプレイヤーの情報の初期化`
	void CameraSetPlayer();

	//ゲームカメラのインスタンスを取得
	static CGameCamera& GetInstance()
	{
		return *m_gameCamera;
	}

	//インスタンスの生成
	static void CGameCamera::Create()
	{
		if (!m_gameCamera)
		{
			m_gameCamera = New<CGameCamera>(PRIORITY_CAMERA);
		}
	}

	//インスタンスの消去
	static void CGameCamera::Destroy()
	{
		Delete(m_gameCamera);
		m_gameCamera = nullptr;
		PhysicsWorld().SetCamera(nullptr);
		Engine().GetDeferred().SetCamera(nullptr);
		Sky().SetCamera(nullptr);
	}

	//カメラを取得
	const CCamera& GetCamera()
	{
		return m_camera;
	}

	//バネカメラの取得
	const CSpringCamera& GetSpringCamera()
	{
		return m_springCamera;
	}

	//揺れカメラの取得
	/*const */CShakeCamera& GetShakeCamera()
	{
		return m_shakeCamera;
	}

	//カメラのポジションを設定
	//第一引数　カメラの座標　第二引数　ターゲットの座標
	void SetCameraPosition(const CVector3 pos,const CVector3 tag)
	{
		m_camera.SetPosition(pos);
		m_camera.SetTarget(tag);
		//m_springCamera.SetTarPosition(pos);
		//m_springCamera.SetTarTarget(tag);
	}

	//ビュー行列を取得
	CMatrix GetViewMatrix()
	{
		return m_camera.GetViewMatrix();
	}

	//プロジェクション行列を取得
	CMatrix GetProjectionMatrix()
	{
		return m_camera.GetProjectionMatrix();
	}

	////バネカメラのプロジェクション行列の取得
	//const CMatrix GetSpringProjectionMatrix()
	//{
	//	return m_springCamera.GetProjectionMatrix();
	//}
	////バネカメラのビュー行列の取得
	//const CMatrix GetSpringViewMatrix()
	//{
	//	return m_springCamera.GetSpringViewMatrix();
	//}
	void SetCmareaState(EnCameraState cameraState)
	{
		m_cameraState = cameraState;
	}

	//エネミーをロックオンする
	void RockOnEnemy();

private:
	static CGameCamera* m_gameCamera;							//ゲームカメラ
	CCamera				m_camera;								//カメラ
	EnCameraState		m_cameraState = EnCameraState::enNormal;//カメラステート
	CNormalGameCamera	m_normalCamera;							//ノーマルカメラ
	CArrowGameCamera	m_arrowCamera;							//アローカメラ
	CVector3			m_cameraVec;							//注視点からカメラへのベクトル
	CSpringCamera		m_springCamera;							//バネカメラ
	CShakeCamera		m_shakeCamera;							//揺れカメラ
	IEnemy*				m_rockOnEnemy;							//ロックオンしているエネミー
	bool				m_isRockOn = false;						//ロックオンしているか
};

static CGameCamera& GetGameCamera()
{
	return CGameCamera::GetInstance();
}