#include "engineStdafx.h"
#include "ShadowMap.h"
#include "../../Graphics/SkinModel.h"
#include "../PostRendering/Deferred.h"
#include "../../Engine.h"

void CShadowMap::Init()
{
	m_renderTarget.CreateRenderTarget(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_renderTarget.CreateDepthStencilBuffer(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
	CMatrix mat = CMatrix::Identity;
	m_CB.Create(sizeof(CMatrix), &mat);

}

void CShadowMap::SetConstantBuffer()
{
	CMatrix lightViewProj;
	lightViewProj.Mul(m_viewMatrix, m_projectionMatrix);
	m_CB.Update(&lightViewProj);
	ID3D11Buffer* buffer = m_CB.GetBody();
	GetDeviceContext()->PSSetConstantBuffers(2, 1, &buffer);
	GetDeviceContext()->VSSetConstantBuffers(2, 1, &buffer);
}

void CShadowMap::Draw()
{

	ID3D11RenderTargetView* rtv[] = { m_renderTarget.GetRenderTarget() };
	Engine().GetDeviceContext()->OMSetRenderTargets(1, rtv, m_renderTarget.GetDepthStencil());
	float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	Engine().GetDeviceContext()->ClearRenderTargetView(m_renderTarget.GetRenderTarget(), color);
	Engine().GetDeviceContext()->ClearDepthStencilView(m_renderTarget.GetDepthStencil(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	Engine().SetAlphaBlendState(enAlphaBlendStateNone);
	Engine().SetDepthStencilState(enDepthStencilState3D);
	CMatrix viewMat = m_viewMatrix;
	CMatrix projMat = m_projectionMatrix;
	Engine().SetViewPortState(enViewPortShadow);
	for (auto& model : m_modelList)
	{
		model->Draw(viewMat, projMat, true);
	}
	m_modelList.clear();
	Engine().SetViewPortState(enViewPortGame);
}
