#pragma once
#include "../RenderTarget.h"
#include "../Texture.h"
#include "../Primitive.h"
#include "../Shader.h"
#include "../ConstantBuffer.h"

class CCamera;

enum EnRenderTarget 
{
	enRenderTargetColor,		//カラー
	enRenderTargetNormalMap,	//法線マップ
	enRenderTargetNormal,		//法線
	enRenderTargetTangent,		//接ベクトル
	enRenderTargetDepth,		//深度値(今はワールド座標
	enRenderTargetMaterial,		//マテリアル情報
	enRenderTargetNum
};

class CDeferred : Uncopyable
{
public:
	//頂点レイアウト
	struct SVSLayout
	{
		CVector4 position;
		CVector2	uv;
	};

	//コンストラクタ
	CDeferred();

	//初期化関数
	void Init();

	//ディファード用のレンダリングターゲットに切り替える
	void Start();

	//描画関数
	void Draw();

	/*
	シェーダーリソースビューを取得
	numRenderTarget	レンダリングターゲットの番号
	*/
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetShaderResource(EnRenderTarget numRenderTarget)
	{
		return m_renderTarget[numRenderTarget].GetRenderTargetTexture().GetShaderResource();
	}

	/*
	カメラのセッター
	camera	ゲームカメラ
	*/
	void SetCamera(const CCamera* camera);

private:
	struct SDefferedCB
	{
		CVector4 gameCameraPos;
		int frameBufferWidth;
		int frameBufferHeight;
	};
	CRenderTarget							m_renderTarget[enRenderTargetNum];	//Gバッファー
	CPrimitive								m_primitive;						//プリミティブ
	CShader									m_vertexShader;						//頂点シェーダー
	CShader									m_pixelShader;						//ピクセルシェーダー
	CConstantBuffer							m_lightCB;							//ライトの定数バッファ
	CConstantBuffer							m_gameCameraCB;						//ゲームカメラの定数バッファ
	CConstantBuffer							m_materialCB;						//マテリアル情報
	CConstantBuffer							m_defferedCB;						//フレーム情報
	const CCamera*							m_camera = nullptr;					//定数バッファの更新に使うゲームカメラ
	ID3D11SamplerState*						m_pLinearSampler;
	ID3D11SamplerState*						m_pAnisotropicSampler;
};