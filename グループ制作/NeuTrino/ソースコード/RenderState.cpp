#include "main.h"
#include "RenderState.h"

void SetUVSTA(int nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// �T���v���[�X�e�[�g�̐ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, nType);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, nType);
}

void SetBlend(int nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	  // ������`�����́A�|���S���e�N�X�`��
	pDevice->SetRenderState(D3DRS_DESTBLEND, nType);  // ���łɕ`�悵��   D3DBLEND_INVSRCALPHA
}

void SetCullMode(int nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	pDevice->SetRenderState(D3DRS_CULLMODE, nType);
}

void SetAlphaTest(bool bStatus, int nA)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, bStatus);	// ���铬�l��alpha�l���ׂč��i������`�悷��
	
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