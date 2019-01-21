#pragma once

enum EnAlphaBlendState
{
	enAlphaBlendStateAdd,				//加算合成	
	enAlphaBlendStateTranslucent,		//半透明合成
	enAlphaBlendStateNone,				//アルファブレンド無
	enAlphaBlendStateNum,
};

//アルファブレンドの設定を管理するクラス
class CAlphaBlendState : Uncopyable
{
public:

	//初期化
	void Init(Microsoft::WRL::ComPtr<ID3D11Device> device);


	/*
	アルファブレンド設定を変更
	deviceContext		グラフィックデバイス
	enBlendState		アルファブレンドの設定
	*/
	void SetBlendState(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, EnAlphaBlendState enBlendState)
	{
		m_currentState = enBlendState;
		deviceContext->OMSetBlendState(m_blendState[m_currentState].Get(), NULL, 0xffffffff);
	}

	//現在の設定を取得
	EnAlphaBlendState GetCurrentBlendState()
	{
		return m_currentState;
	}

private:
	Microsoft::WRL::ComPtr<ID3D11BlendState>		m_blendState[enAlphaBlendStateNum] = {nullptr};		//アルファブレンドステート
	EnAlphaBlendState		m_currentState = enAlphaBlendStateAdd;				//現在の設定
};