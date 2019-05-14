//----------------------------------------------------------------------------
//
//	�{�^���̑������
//
//														 ����ҁF����
//                                                       �쐬���F2017/02/06
//----------------------------------------------------------------------------

//---------------------------�C���N���[�h-------------------------------------
#include "main.h"
#include "Polygon.h"
#include "Texture.h"
#include "BtnManual.h"
#include "JoyStick.h"

//---------------------------�}�N����`---------------------------------------

//---------------------------�񋓌^��`---------------------------------------

//---------------------------�\���̒�`---------------------------------------
typedef struct {
	D3DXVECTOR2 vPos;			// ���W
	D3DXVECTOR2 vPosOld;		// ���W(�`��p)
	D3DXVECTOR2 vPosDir;		//
	int nCount;					// 
	int nU;						//
	int nV;						//
	bool bStatus;				//
}BTN_MANUAL;					//

//---------------------------�O���[�o���ϐ�-----------------------------------
static BTN_MANUAL g_BtnManual;

static const int gc_nBtnManualMaxCount = 3;
static const D3DXVECTOR2 gc_vBtnManualPos[RDP_MAX] =
{ { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT + 21.0f },{ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 21.0f } };

static int g_nBtnManualTarget;

//---------------------------�v���g�^�C�v�錾---------------------------------
void InitBtnManual(int nBtnManual)
{
	g_BtnManual.bStatus = false;
	g_BtnManual.nCount = 0;
	g_BtnManual.nV = nBtnManual;
	g_BtnManual.vPos = gc_vBtnManualPos[RDP_END];
	g_BtnManual.vPosOld = gc_vBtnManualPos[RDP_INIT];
	g_BtnManual.vPosDir = g_BtnManual.vPos - g_BtnManual.vPosOld;
	if (GetJSDev() == NULL)
	{
		g_BtnManual.nU = 0;
	}
	else
	{
		g_BtnManual.nU = 1;
	}
}

void UninitBtnManual()
{

}

void UpdateBtnManual()
{
	if (g_BtnManual.vPosOld != g_BtnManual.vPos)
	{
		g_BtnManual.nCount++;
		g_BtnManual.vPosOld += g_BtnManual.vPosDir / (gc_nBtnManualMaxCount * 1.0f);
		if (g_BtnManual.nCount == gc_nBtnManualMaxCount)
		{
			g_BtnManual.vPosOld = g_BtnManual.vPos;
			if (g_BtnManual.vPosOld == gc_vBtnManualPos[RDP_INIT])
			{
				SetBtnManualPos(RDP_END);
				g_BtnManual.nV = g_nBtnManualTarget;
			}
			else
				if (g_BtnManual.vPosOld == gc_vBtnManualPos[RDP_END])
				{
					g_BtnManual.bStatus = false;
				}
		}
	}
}

void DrawBtnManual()
{
	SetZBuffer(FALSE);
	SetTexture(TEX_BTN_MANUAL);
	TEXTURE tTexture = GetTextureStatus(TEX_BTN_MANUAL);
	SetPolygonColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	DrawPolygon
	(
		g_BtnManual.vPosOld.x,
		g_BtnManual.vPosOld.y,
		g_BtnManual.nU * tTexture.u,
		g_BtnManual.nV * tTexture.v,
		tTexture.u,
		tTexture.v
	);
	SetZBuffer(TRUE);
}

void SetBtnManualPos(int nPart)
{
	g_BtnManual.nCount = 0;
	g_BtnManual.vPos = gc_vBtnManualPos[nPart];
	g_BtnManual.vPosDir = g_BtnManual.vPos - g_BtnManual.vPosOld;
}

void SetBtnManualTarget(int nNum)
{
	g_BtnManual.bStatus = true;
	g_nBtnManualTarget = nNum;
}

bool GetBtnManualStatus()
{
	return g_BtnManual.bStatus;
}