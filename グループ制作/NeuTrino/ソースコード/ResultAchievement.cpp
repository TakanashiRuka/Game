//----------------------------------------------------------------------------
//
//	���U���g�̕]��
//
//														 ����ҁF����
//                                                       �쐬���F2018/01/23
//----------------------------------------------------------------------------

//---------------------------�C���N���[�h-------------------------------------
#include "main.h"
#include "Polygon.h"
#include "Texture.h"
#include "ResultAchievement.h"
#include "Gauge.h"
#include "sound.h"

//---------------------------�}�N����`---------------------------------------

//---------------------------�񋓌^��`---------------------------------------

//---------------------------�\���̒�`---------------------------------------
typedef struct {
	D3DXVECTOR2 vPos;					// ���W
	int nA;								// ���l
	int nU;								// U�̒l
	float fScale;						// �X�P�[�����O
	int nCount;							// �J�E���^�[
	bool bStatus;						// �`����
}RESULT_ACHIEVEMENT_ICON;				// ���U���g�̒B������

typedef struct {
	D3DXVECTOR2 vPos;					// ���W
	int nA;								// ���l
	int nV;								// V�̒l
	float fScale;						// �X�P�[�����O
	int nCount;							// �J�E���^�[
}RESULT_CLEAR_FAILED;					// ���U���g�̒B������(�N���A/���s)

//---------------------------�O���[�o���ϐ�-----------------------------------
//----------------------------------------------------------------------------
// �B������(FC/AP)
//----------------------------------------------------------------------------
static RESULT_ACHIEVEMENT_ICON g_ResultAchievementIcon[RAP_MAX];

static const D3DXVECTOR2 gc_vResultAchievementInitPos = { 895.0f,570.0f };		// �A�C�R���̏������W
static const D3DXVECTOR2 gc_vResultAchievementRangePos = { 55.0f,0.0f };		// �A�C�R�����W�̍�
static const float gc_fResultAchievementScale[RDP_MAX] =
{
	4.0f,1.0f
};

static const int gc_nResultAchievementA[RDP_MAX] =
{
	0,255
};

static const int gc_nResultAchievementMaxCount = 10;							// �J�E���^�[�̍ő�l

static int g_nResultAchievementStartFrameCount;
static const int gc_nResultAchievementStartFrame[RAP_MAX]=
{
  0,
  36,
};

//---------------------------------------------------------------
// �B������(�N���A/���s)
//---------------------------------------------------------------
static RESULT_CLEAR_FAILED g_ResultClearFailed;

static const D3DXVECTOR2 gc_vResultClearFailedInitPos = { WINDOW_WIDTH / 2.0f,28.0f };		// �A�C�R���̏������W
static const float gc_fResultClearFailedScale[RDP_MAX] =
{
	4.0f,1.0f
};

static const int gc_nResultClearFailedA[RDP_MAX] =
{
	0,255
};

static const int gc_nResultClearFailedMaxCount = 10;							// �J�E���^�[�̍ő�l


//---------------------------�v���g�^�C�v�錾---------------------------------
//----------------------------------------------------------------------------
// �B������(FC/AP)
//----------------------------------------------------------------------------
void InitResultAchievement()
{
	g_nResultAchievementStartFrameCount = 0;

	for (int nCountResultAchievementParts = 0; nCountResultAchievementParts < RAP_MAX; nCountResultAchievementParts++)
	{
		g_ResultAchievementIcon[nCountResultAchievementParts].fScale = gc_fResultAchievementScale[RDP_INIT];
		g_ResultAchievementIcon[nCountResultAchievementParts].nCount = 0;
		g_ResultAchievementIcon[nCountResultAchievementParts].nA = gc_nResultAchievementA[RDP_INIT];
		g_ResultAchievementIcon[nCountResultAchievementParts].nU = nCountResultAchievementParts;
		g_ResultAchievementIcon[nCountResultAchievementParts].vPos = gc_vResultAchievementInitPos + nCountResultAchievementParts * gc_vResultAchievementRangePos;
	 }
}

void UninitResultAchievement()
{

}

void UpdateResultAchievement()
{
	if (g_nResultAchievementStartFrameCount < gc_nResultAchievementStartFrame[RAP_MAX - 1])
	{
		g_nResultAchievementStartFrameCount++;
	}
	for (int nCountResultAchievementParts = 0; nCountResultAchievementParts < RAP_MAX; nCountResultAchievementParts++)
	{
		if (g_ResultAchievementIcon[nCountResultAchievementParts].bStatus && 
			g_nResultAchievementStartFrameCount>=gc_nResultAchievementStartFrame[nCountResultAchievementParts])
		{
			if (g_ResultAchievementIcon[nCountResultAchievementParts].nCount < gc_nResultAchievementMaxCount)
			{
				g_ResultAchievementIcon[nCountResultAchievementParts].nCount++;
				
				// �X�P�[�����O
				g_ResultAchievementIcon[nCountResultAchievementParts].fScale += (gc_fResultAchievementScale[RDP_END] - gc_fResultAchievementScale[RDP_INIT]) /
					(gc_nResultAchievementMaxCount * 1.0f);

				// ���l
				g_ResultAchievementIcon[nCountResultAchievementParts].nA += (gc_nResultAchievementA[RDP_END] - gc_nResultAchievementA[RDP_INIT]) * 1.0f /
					(gc_nResultAchievementMaxCount * 1.0f);
				if (g_ResultAchievementIcon[nCountResultAchievementParts].nCount == gc_nResultAchievementMaxCount)
				{
					g_ResultAchievementIcon[nCountResultAchievementParts].fScale = gc_fResultAchievementScale[RDP_END];
					g_ResultAchievementIcon[nCountResultAchievementParts].nA = gc_nResultAchievementA[RDP_END];
					if (GetPlayStatus(SE_FC_AP))
					{
						StopSound(SE_FC_AP);
					}
					PlaySound(SE_FC_AP);
				}
			}
		}
	}
}

void DrawResultAchievement()
{
	SetZBuffer(FALSE);
	SetTexture(TEX_RESULT_ACHIEVEMENT_ICON);
	TEXTURE tTexture = GetTextureStatus(TEX_RESULT_ACHIEVEMENT_ICON);
	for (int nCountResultAchievementParts = 0; nCountResultAchievementParts < RAP_MAX; nCountResultAchievementParts++)
	{
		if (g_ResultAchievementIcon[nCountResultAchievementParts].bStatus)
		{
			SetPolygonColor(D3DCOLOR_ARGB(g_ResultAchievementIcon[nCountResultAchievementParts].nA, 255, 255, 255));
			SetScale
			(
				g_ResultAchievementIcon[nCountResultAchievementParts].fScale,
				g_ResultAchievementIcon[nCountResultAchievementParts].fScale,
				tTexture.u / 2.0f,
				tTexture.v / 2.0f
			);
			DrawPolygon
			(
				g_ResultAchievementIcon[nCountResultAchievementParts].vPos.x,
				g_ResultAchievementIcon[nCountResultAchievementParts].vPos.y,
				tTexture.u * g_ResultAchievementIcon[nCountResultAchievementParts].nU,
				0,
				tTexture.u,
				tTexture.v
			);
		}
	}
	SetZBuffer(TRUE);
}

void SetResultAchievement(int nParts, bool bStatus)
{
	g_ResultAchievementIcon[nParts].bStatus = bStatus;
}

//---------------------------------------------------------------
// �B������(�N���A/���s)
//---------------------------------------------------------------
void InitResultClearFailed()
{
	g_ResultClearFailed.fScale = gc_fResultClearFailedScale[RDP_INIT];
	g_ResultClearFailed.nA = gc_nResultClearFailedA[RDP_INIT];
	g_ResultClearFailed.nCount = 0;
	g_ResultClearFailed.vPos = gc_vResultClearFailedInitPos;
	if (!GetClearStatus())
	{
		g_ResultClearFailed.nV = 0;
	}
	else
	{
		g_ResultClearFailed.nV = 1;
	}
}

void UninitResultClearFailed()
{

}

void UpdateResultClearFailed()
{
	if (g_ResultClearFailed.nCount < gc_nResultClearFailedMaxCount)
	{
		g_ResultClearFailed.nCount++;

		// �X�P�[�����O
		g_ResultClearFailed.fScale += (gc_fResultClearFailedScale[RDP_END] - gc_fResultClearFailedScale[RDP_INIT]) /
			(gc_nResultClearFailedMaxCount * 1.0f);

		// ���l
		g_ResultClearFailed.nA += (gc_nResultClearFailedA[RDP_END] - gc_nResultClearFailedA[RDP_INIT]) * 1.0f /
			(gc_nResultClearFailedMaxCount * 1.0f);
		if (g_ResultClearFailed.nCount == gc_nResultClearFailedMaxCount)
		{
			g_ResultClearFailed.fScale = gc_fResultClearFailedScale[RDP_END];
			g_ResultClearFailed.nA = gc_nResultClearFailedA[RDP_END];
		}
	}
}

void DrawResultClearFailed()
{
	SetZBuffer(FALSE);
	SetTexture(TEX_RESULT_CLEAR_FAILED);
	TEXTURE tTexture = GetTextureStatus(TEX_RESULT_CLEAR_FAILED);
	SetPolygonColor(D3DCOLOR_ARGB(g_ResultClearFailed.nA, 255, 255, 255));
	SetScale
	(
		g_ResultClearFailed.fScale,
		g_ResultClearFailed.fScale,
		tTexture.u / 2.0f,
		tTexture.v / 2.0f
	);
	DrawPolygon
	(
		g_ResultClearFailed.vPos.x,
		g_ResultClearFailed.vPos.y,
		0,
		tTexture.v * g_ResultClearFailed.nV,
		tTexture.u,
		tTexture.v
	);
	SetZBuffer(TRUE);
}