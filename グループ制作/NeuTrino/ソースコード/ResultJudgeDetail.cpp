//----------------------------------------------------------------------------
//
//	���U���g(�ڍ׌���)
//
//														 ����ҁF����
//                                                       �쐬���F2018/01/14
//----------------------------------------------------------------------------

//---------------------------�C���N���[�h-------------------------------------
#include "main.h"
#include "Polygon.h"
#include "Texture.h"
#include "number.h"
#include "ResultJudgeDetail.h"
#include "Judge.h"
#include "Combo.h"
#include "sound.h"

//---------------------------�}�N����`---------------------------------------

//---------------------------�񋓌^��`---------------------------------------

//---------------------------�\���̒�`---------------------------------------
typedef struct {
	D3DXVECTOR2 vPos;				// ���W
	int nA;							// ���l
	int nV;							// V�̒l
	int nCountFadeIn;				// �t�F�[�h�C���̃J�E���^�[
	int nJudgeNum;					// ���背�x���ɂ������̒l
}RESULT_JUDGE_DETAIL;				// �ڍ׌���

//---------------------------�O���[�o���ϐ�-----------------------------------
static RESULT_JUDGE_DETAIL g_ResultJudgeDetail[JL_MAX + 1];

static const int gc_nResultJudgeDetailFadeInCountMax = 4;

static const int gc_nResultJudgeDetailA[RDP_MAX] = 
{ 
	0 ,255
};

static const int gc_nResultJudgeDetailStartTime[JL_MAX + 1]=
{
	0,
	4,
	8,
	12,
	16,
};

static int g_nResultJudgeDetailCountStartTime;

static const D3DXVECTOR2 gc_vResultJudgeDetailInitPos = { 400.0f,408.0f };
static const D3DXVECTOR2 gc_vResultJudgeDetailAddPos = { 0.0f,38.0f };

static const D3DXVECTOR2 gc_vResultJudgeDetailNumAddPos = { 192.0f,0.0f };
static const float gc_fResultJudgeDetailNumRangeX = 12.0f;

//---------------------------�v���g�^�C�v�錾---------------------------------
void InitResultJudgeDetail()
{
	g_nResultJudgeDetailCountStartTime = 0;

	for (int nCountJudgeDetail = 0; nCountJudgeDetail < JL_MAX + 1; nCountJudgeDetail++)
	{
		g_ResultJudgeDetail[nCountJudgeDetail].nA = gc_nResultJudgeDetailA[RDP_INIT];
		g_ResultJudgeDetail[nCountJudgeDetail].nCountFadeIn = 0;
		g_ResultJudgeDetail[nCountJudgeDetail].nV = nCountJudgeDetail;
		g_ResultJudgeDetail[nCountJudgeDetail].vPos = gc_vResultJudgeDetailInitPos + nCountJudgeDetail * gc_vResultJudgeDetailAddPos;
		if (nCountJudgeDetail < JL_MAX)
		{
			g_ResultJudgeDetail[nCountJudgeDetail].nJudgeNum = GetJudgeLevelDetail(nCountJudgeDetail);
		}
		else
		{
			g_ResultJudgeDetail[nCountJudgeDetail].nJudgeNum = GetMaxCombo();
		}
	}
}

void UninitResultJudgeDetail()
{

}

void UpdateResultJudgeDetail()
{
	if (g_nResultJudgeDetailCountStartTime < gc_nResultJudgeDetailStartTime[JL_MAX])
	{
		g_nResultJudgeDetailCountStartTime++;
	}

	for (int nCountJudgeDetail = 0; nCountJudgeDetail < JL_MAX + 1; nCountJudgeDetail++)
	{
		if (g_nResultJudgeDetailCountStartTime >= gc_nResultJudgeDetailStartTime[nCountJudgeDetail])
		{
			if (g_ResultJudgeDetail[nCountJudgeDetail].nCountFadeIn < gc_nResultJudgeDetailFadeInCountMax)
			{
				g_ResultJudgeDetail[nCountJudgeDetail].nCountFadeIn++;
				g_ResultJudgeDetail[nCountJudgeDetail].nA += (gc_nResultJudgeDetailA[RDP_END] - gc_nResultJudgeDetailA[RDP_INIT]) * 1.0f / (gc_nResultJudgeDetailFadeInCountMax * 1.0f);
				if (g_ResultJudgeDetail[nCountJudgeDetail].nCountFadeIn == gc_nResultJudgeDetailFadeInCountMax)
				{
					g_ResultJudgeDetail[nCountJudgeDetail].nA = gc_nResultJudgeDetailA[RDP_END];
					if(GetPlayStatus(SE_RESULT_DETAIL))
					{
						StopSound(SE_RESULT_DETAIL);
					}
					PlaySound(SE_RESULT_DETAIL);
				}
			}
		}
	}
}

void DrawResultJudgeDetail()
{
	TEXTURE tJudgeDetailWord = GetTextureStatus(TEX_RESULT_JUDGE_LEVEL);
	TEXTURE tJudgeDetailNum = GetTextureStatus(TEX_RESULT_DETAIL_NUM);

	for (int nCountJudgeDetail = 0; nCountJudgeDetail < JL_MAX + 1; nCountJudgeDetail++)
	{
		SetTexture(TEX_RESULT_JUDGE_LEVEL);
		SetPolygonColor(D3DCOLOR_ARGB(g_ResultJudgeDetail[nCountJudgeDetail].nA, 255, 255, 255));
		DrawPolygon
		(
			g_ResultJudgeDetail[nCountJudgeDetail].vPos.x,
			g_ResultJudgeDetail[nCountJudgeDetail].vPos.y,
			0,
			tJudgeDetailWord.v * g_ResultJudgeDetail[nCountJudgeDetail].nV,
			tJudgeDetailWord.u,
			tJudgeDetailWord.v
		);

		// ����
		int n_ScoreBackUp = g_ResultJudgeDetail[nCountJudgeDetail].nJudgeNum;
		int nDigit = 0;

		do
		{
			n_ScoreBackUp /= 10;
			nDigit++;
		} while (n_ScoreBackUp != 0);

		n_ScoreBackUp = g_ResultJudgeDetail[nCountJudgeDetail].nJudgeNum;
		int count;
		for (count = 0; count < nDigit; count++)
		{
			int n = n_ScoreBackUp % 10;
			DrawNumber(TEX_RESULT_DETAIL_NUM, tJudgeDetailNum.u, tJudgeDetailNum.v, n, g_ResultJudgeDetail[nCountJudgeDetail].vPos.x - (count)* gc_fResultJudgeDetailNumRangeX + gc_vResultJudgeDetailNumAddPos.x, g_ResultJudgeDetail[nCountJudgeDetail].vPos.y, g_ResultJudgeDetail[nCountJudgeDetail].nA);
			n_ScoreBackUp /= 10;
		}
	}
}