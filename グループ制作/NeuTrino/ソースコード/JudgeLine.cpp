//----------------------------------------------------------------------------
//
//	���胉�C��
//
//														 ����ҁF����
//                                                       �쐬���F2017/11/30
//----------------------------------------------------------------------------

//---------------------------�C���N���[�h-------------------------------------
#include "main.h"
#include "JudgeLine.h"
#include "Texture.h"
#include "Polygon3D.h"
#include "Light.h"
#include "Note.h"
#include "Btn.h"

//---------------------------�}�N����`---------------------------------------
#define JUDGE_LINE_X					(0.0f)
#define JUDGE_LINE_Y					(0.0002f)
#define JUDGE_LINE_SP_A_MIN				(0)
#define JUDGE_LINE_SP_A_MAX				(127)
#define JUDGE_LINE_SP_LOOP_TIME			(3)

//---------------------------�񋓌^��`---------------------------------------

//---------------------------�\���̒�`---------------------------------------
typedef struct {
	D3DXVECTOR3 vPos;		// ���W
}JUDGE_LINE;				// ���胉�C��

//---------------------------�O���[�o���ϐ�-----------------------------------
static JUDGE_LINE g_JudgeLine;
static int g_nJudgeLineSpA;
static int g_nJudgeLineSpCount;

//---------------------------�v���g�^�C�v�錾---------------------------------
void InitJudgeLine()
{
	g_JudgeLine.vPos = { JUDGE_LINE_X,JUDGE_LINE_Y,JUDGE_LINE_Z };
	g_nJudgeLineSpA = JUDGE_LINE_SP_A_MIN;
	g_nJudgeLineSpCount = 0;
}

void UninitJudgeLine()
{

}

void UpdateJudgeLine()
{

}

void DrawJudgeLine()
{
	SetLight(FALSE);
	SetTexture(TEX_JUDGE_LINE);
	SetPolygonColor3D(D3DCOLOR_ARGB(255, 255, 255, 255));
	DrawPolygon3D
	(
		g_JudgeLine.vPos.x,
		g_JudgeLine.vPos.y,
		g_JudgeLine.vPos.z,
		0,
		0,
		GetTextureStatus(TEX_JUDGE_LINE).u,
		GetTextureStatus(TEX_JUDGE_LINE).v
	);
}

void UpdateJudgeLineSpecial()
{
	g_nJudgeLineSpCount++;
	if (g_nJudgeLineSpCount < JUDGE_LINE_SP_LOOP_TIME)
	{
		g_nJudgeLineSpA += (float)(JUDGE_LINE_SP_A_MAX - JUDGE_LINE_SP_A_MIN) / JUDGE_LINE_SP_LOOP_TIME;
	}
	if (g_nJudgeLineSpCount == JUDGE_LINE_SP_LOOP_TIME)
	{
		g_nJudgeLineSpA = JUDGE_LINE_SP_A_MAX;
	}
	if (g_nJudgeLineSpCount > JUDGE_LINE_SP_LOOP_TIME && g_nJudgeLineSpCount <= JUDGE_LINE_SP_LOOP_TIME * 2)
	{																									
		g_nJudgeLineSpA += (float)(-JUDGE_LINE_SP_A_MAX + JUDGE_LINE_SP_A_MIN) / JUDGE_LINE_SP_LOOP_TIME;
	}
	if (g_nJudgeLineSpCount == JUDGE_LINE_SP_LOOP_TIME * 2)
	{
		g_nJudgeLineSpA = JUDGE_LINE_SP_A_MIN;
		g_nJudgeLineSpCount = 0;
	}

}

void DrawJudgeLineSpecial()
{
	SetLight(FALSE);
	SetTexture(TEX_JUDGE_LINE_SP);
	for (int nCountJudgeLine = NSP_MAX; nCountJudgeLine < NUM_BTN; nCountJudgeLine++)
	{
		float fY = GetSlidePos(nCountJudgeLine).y;

		if (fY != -1.0f)
		{
			if (fY > NOTE_SLIDE_Y_MAX)
			{
				fY = NOTE_SLIDE_Y_MAX;
			}
			SetPolygonColor3D(D3DCOLOR_ARGB(255, 255, 255, 255));
			DrawPolygon3D
			(
				g_JudgeLine.vPos.x - NOTE_RANGE_X + (nCountJudgeLine - NSP_MAX) * NOTE_RANGE_X * 2.0f,
				fY / N,
				g_JudgeLine.vPos.z,
				0,
				0,
				GetTextureStatus(TEX_JUDGE_LINE).u,
				GetTextureStatus(TEX_JUDGE_LINE).v
			);
			SetBlend(D3DBLEND_BLENDFACTOR);
			SetPolygonColor3D(D3DCOLOR_ARGB(g_nJudgeLineSpA, 255, 255, 255));
			DrawPolygon3D
			(
				g_JudgeLine.vPos.x - NOTE_RANGE_X + (nCountJudgeLine - NSP_MAX) * NOTE_RANGE_X * 2.0f,
				fY / N,
				g_JudgeLine.vPos.z,
				0,
				0,
				GetTextureStatus(TEX_JUDGE_LINE).u,
				GetTextureStatus(TEX_JUDGE_LINE).v
			);
			SetBlend(D3DBLEND_INVSRCALPHA);
		}
	}
}