#include "main.h"
#include "RenderState.h"

void SetUVSTA(int nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// サンプラーステートの設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, nType);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, nType);
}

void SetBlend(int nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	  // 今から描くもの、ポリゴンテクスチャ
	pDevice->SetRenderState(D3DRS_DESTBLEND, nType);  // すでに描画した   D3DBLEND_INVSRCALPHA
}

void SetCullMode(int nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	pDevice->SetRenderState(D3DRS_CULLMODE, nType);
}

void SetAlphaTest(bool bStatus, int nA)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, bStatus);	// ある闘値とalpha値を比べて合格したら描画する
	
	if (bStatus)
	{
		pDevice->SetRenderState(D3DRS_ALPHAREF, nA);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	}
}

void SetZBuffer(bool bStatus)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	pDevice->SetRenderState(D3DRS_ZENABLE, bStatus);
}