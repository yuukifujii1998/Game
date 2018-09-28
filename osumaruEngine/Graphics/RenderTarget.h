#pragma once
#include "Texture.h"
//レンダリングターゲットクラス

class CRenderTarget : Uncopyable
{
public:
	//コンストラクタ
	CRenderTarget();

	//デストラクタ
	~CRenderTarget();

	/*
	レンダリングターゲットの作成
	width			レンダリングターゲットの幅
	height			レンダリングターゲットの高さ
	pRenderTarget	レンダリングターゲットに使うテクスチャ
	*/
	void Create(int width, int height, ID3D11Texture2D* pRenderTarget = nullptr);

	//レンダリングターゲットの取得
	ID3D11RenderTargetView* GetRenderTarget() const
	{
		return m_pRenderTarget;
	}

	//デプスステンシルバッファの取得
	ID3D11DepthStencilView* GetDepthStencil() const
	{
		return m_pDepthStencil;
	}

	//レンダリングターゲット用のテクスチャの取得
	ID3D11Texture2D* GetRenderTargetTexture() const
	{
		return m_pD3DRenderTargetTexture;
	}


	//デプスステンシルテクスチャ
	ID3D11Texture2D* GetDepthStencilTexture() const
	{
		return m_pD3DDepthStencilTexture;
	}

	CTexture& GetRenderTargetTexture()
	{
		return *m_pRenderTargetTexture;
	}

	CTexture& GetDepthStencilTexture()
	{
		return *m_pDepthStencilTexture;
	}
private:
	ID3D11DepthStencilView*					m_pDepthStencil;		//デプスステンシルビュー
	ID3D11RenderTargetView*					m_pRenderTarget;		//レンダリングターゲット
	ID3D11Texture2D*						m_pD3DRenderTargetTexture;	//レンダリングターゲット用のテクスチャ	
	ID3D11Texture2D*						m_pD3DDepthStencilTexture;	//デプスステンシル用のテクスチャ
	int										m_width;				//レンダリングターゲットの幅
	int										m_height;				//レンダリングターゲットの高さ
	std::unique_ptr<CTexture>				m_pRenderTargetTexture;	//レンダリングターゲット用のテクスチャ
	std::unique_ptr<CTexture>				m_pDepthStencilTexture;
};
