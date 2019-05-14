//----------------------------------------------------------------------------
//
//	���U���g(�ڍ׌���)�̃X�R�A
//
//														 ����ҁF����
//                                                       �쐬���F2018/01/14
//----------------------------------------------------------------------------

//---------------------------�C���N���[�h-------------------------------------
#include "main.h"
#include "Polygon.h"
#include "Texture.h"
#include "number.h"
#include "ResultScore.h"
#include "score.h"

//---------------------------�}�N����`---------------------------------------

//---------------------------�񋓌^��`---------------------------------------

//---------------------------�\���̒�`---------------------------------------
typedef struct {
	D3DXVECTOR2 vPos;			// ���W
	float fScore;				// �X�R�A
	int nA;						// ���l
	int nFadeInCount;
}RESULT_SCORE;					// ���U���g(�ڍ׌���)�̃X�R�A

//---------------------------�O���[�o���ϐ�-----------------------------------
static RESULT_SCORE g_ResultScore;

static const float gc_fResultScoreRangeX = 43.0f;
static const D3DXVECTOR2 gc_vResultScoreInitPos =
{
	WINDOW_WIDTH / 2.0f + 3.0f * gc_fResultScoreRangeX,
	295.0f
};

static const int gc_nResultScoreInitA = 0;
static const int gc_nResultScoreEndA = 255;

static const int gc_nResultScoreFadeInMaxCount = 8;

//---------------------------�v���g�^�C�v�錾---------------------------------
void InitResultScore()
{
	g_ResultScore.vPos = gc_vResultScoreInitPos;
	g_ResultScore.nFadeInCount = 0;
	g_ResultScore.nA = gc_nResultScoreInitA;
	g_ResultScore.fScore = GetScore();
}

void UninitResultScore()
{

}

void UpdateResultScore()
{
	if (g_ResultScore.nFadeInCount < gc_nResultScoreFadeInMaxCount)
	{
		g_ResultScore.nFadeInCount++;
		g_ResultScore.nA += (gc_nResultScoreEndA - gc_nResultScoreInitA) * 1.0f / (gc_nResultScoreFadeInMaxCount * 1.0f);
		if (g_ResultScore.nFadeInCount == gc_nResultScoreFadeInMaxCount)
		{
			g_ResultScore.nA = gc_nResultScoreEndA;
		}
	}
}

void DrawResultScore()
{
	TEXTURE tScoreNum = GetTextureStatus(TEX_RESULT_SCORE_NUM);
	int n_ScoreBackUp = g_ResultScore.fScore;
	int nDigit = 0;

	do
	{
		n_ScoreBackUp /= 10;
		nDigit++;
	} while (n_ScoreBackUp != 0);

	n_ScoreBackUp = g_ResultScore.fScore;
	int count;
	for (count = 0; count < nDigit; count++)
	{
		int n = n_ScoreBackUp % 10;
		DrawNumber(TEX_RESULT_SCORE_NUM, tScoreNum.u, tScoreNum.v, n, g_ResultScore.vPos.x - (count)* gc_fResultScoreRangeX, g_ResultScore.vPos.y, g_ResultScore.nA);
		n_ScoreBackUp /= 10;
	}
	//for (count; count < gc_nMaxDigitScore; count++)
	//{
	//	DrawNumber(TEX_RESULT_SCORE_NUM, tScoreNum.u, tScoreNum.v, 0, gc_vScoreNumInitPos.x - (count)* gc_fScoreNumRangeX, gc_vScoreNumInitPos.y, gc_nScoreNumAMin);
	//}
}