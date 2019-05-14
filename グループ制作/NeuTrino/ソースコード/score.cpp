//----------------------------------------------------------------------------
//
//	�X�R�A
//
//														 ����ҁF�����@����
//                                                       �쐬���F2017/12/23
//----------------------------------------------------------------------------

#include "score.h"
#include "main.h"
#include "Note.h"
#include "Texture.h"
#include "Polygon.h"
#include "number.h"

#define SCORE_MAX		(1010000.0f)	//�X�R�A�̍ő�l
#define ADD_CUT			(20)						//���_�̕\�������t���[���ɕ������邩

//�����̎�ނ��Ƃ̃X�R�A�{��
#define TAP				(1.0f)
#define BREAK			(2.0f)
#define HOLD			(1.0f)
#define FLICK			(1.5f)
#define SLIDE			(1.5f)

//���背�x�����Ƃ̃X�R�A�{��
#define PERFECT_JUST	(1.0f)
#define PERFECT			(1.0f)
#define GOOD			(0.5f)
#define BAD				(0.0f)

//---------------------------�\���̒�`---------------------------------------
typedef struct {
	D3DXVECTOR2 vPos;				// ���W
	int nA;							// ���l
	TEXTURE tScoreBoard;			// �e�N�X�`���f�[�^
}SCORE_BOARD;						// �X�R�A�{�[�h

typedef struct {
	D3DXVECTOR2 vPos;				// ���W
	int nA;							// ���l
	TEXTURE tScoreWord;				// �e�N�X�`���f�[�^
}SCORE_WORD;						// �X�R�A����

//---------------------------�O���[�o���ϐ�-----------------------------------
static float score = 0;				//���݂̃X�R�A
static float display_score = 0;		//�\�����邽�߂̉��_���X�R�A
static float additional = 0;		//���_
static float additional_display = 0;		//���_
static int combo = 0;				//�R���{��
static int combo_max = 0;			//�ő�R���{��
static int perfect_just = 0;		//�p�[�t�F�N�g��
static int perfect = 0;				//�p�[�t�F�N�g��
static int perfect_max = 0;			//�ő�p�[�t�F�N�g��

static SCORE_BOARD g_ScoreBoard;
static SCORE_WORD g_ScoreWord;

static const D3DXVECTOR2 gc_vScoreWordInitPos = { 952.0f,15.0f };			// �X�R�A�����̏������W

static const int gc_nMaxDigitScore = 7;
static const D3DXVECTOR2 gc_vScoreNumInitPos = { 1165.0f,45.0f };
static const float gc_fScoreNumRangeX = 23.0f;
static const int gc_nScoreNumAMax = 255;
static const int gc_nScoreNumAMin = 0;
static const float gc_fScorePerfectAdd = -1.0f;

void InitScore()
{
	score = 0;
	display_score = 0;
	additional = 0;
	combo = 0;
	combo_max = 0;
	perfect = 0; 
	perfect_just = 0;
	perfect_max = 0;

	float nt_tap = GetNoteNumPerType(NT_TAP);
	float nt_break = GetNoteNumPerType(NT_BREAK);
	float nt_hold = GetNoteNumPerType(NT_HOLD);
	float nt_flick = GetNoteNumPerType(NT_FLICK);
	float nt_slide = GetNoteNumPerType(NT_SLIDE);
	combo_max = perfect_max = nt_tap + nt_break + nt_hold + nt_flick + nt_slide;
	additional = SCORE_MAX / ((nt_tap * TAP) + (nt_break * BREAK) + (nt_hold * HOLD) + (nt_flick * FLICK) + (nt_slide * SLIDE));
}

void UninitScore()
{

}

void UpdateScore()
{
	if (display_score < score)		//���_���X�R�A�����݂̃X�R�A���Ⴂ�ꍇ
	{
		if (perfect + perfect_just == perfect_max)				//���ׂăp�[�t�F�N�g�̏ꍇ
		{
			score = SCORE_MAX + perfect * gc_fScorePerfectAdd;
		}
		//if (perfect_just == perfect_max)				//���ׂăp�[�t�F�N�g�̏ꍇ
		//{
		//	score = SCORE_MAX;
		//}
		display_score += additional_display / ADD_CUT;
		if (display_score > score)	//���_���X�R�A�����݂̃X�R�A��荂���ꍇ
		{
			display_score = score;
		}
	}
}


void DrawScore()
{
	DebugText(0, 220, "Score:%f, %f", display_score, score);
	TEXTURE tScoreNum = GetTextureStatus(TEX_GAME_SCORE_NUM);
	int n_ScoreBackUp = display_score;
	int nDigit = 0;

	do
	{
		n_ScoreBackUp /= 10;
		nDigit++;
	} while (n_ScoreBackUp != 0);

	n_ScoreBackUp = display_score;
	int count;
	for (count = 0; count < nDigit; count++)
	{
		int n = n_ScoreBackUp % 10;
		DrawNumber(TEX_GAME_SCORE_NUM, tScoreNum.u, tScoreNum.v, n, gc_vScoreNumInitPos.x - (count)* gc_fScoreNumRangeX, gc_vScoreNumInitPos.y, gc_nScoreNumAMax);
		n_ScoreBackUp /= 10;
	}
	for (count; count < gc_nMaxDigitScore; count++)
	{
		DrawNumber(TEX_GAME_SCORE_NUM, tScoreNum.u, tScoreNum.v, 0, gc_vScoreNumInitPos.x - (count)* gc_fScoreNumRangeX, gc_vScoreNumInitPos.y, gc_nScoreNumAMin);
	}
}

float GetScore()
{
	return score;
}

float GetDisplayScore()
{
	return display_score;
}

void AddScore(int note, int judgeLevel)
{
	float fNote = 0.0f;
	switch (note)
	{
	case NT_TAP:
		fNote = TAP;
		break;
	case NT_BREAK:
		fNote = BREAK;
		break;
	case NT_HOLD:
		fNote = HOLD;
		break;
	case NT_FLICK:
		fNote = FLICK;
		break;
	case NT_SLIDE:
		fNote = SLIDE;
		break;
	}

	display_score = score;

	switch (judgeLevel)
	{
	case 0:
		score += (additional * fNote * PERFECT_JUST);
		perfect_just++;
		combo++;
		break;
	case 1:
		score += (additional * fNote * PERFECT) + gc_fScorePerfectAdd;
		perfect++;
		combo++;
		break;
	case 2:
		score += (additional * fNote * GOOD);
		combo++;
		break;
	case 3:
		score += (additional * fNote * BAD);
		combo = 0;
		break;
	}
	additional_display = score - display_score;
}

//---------------------------------------------------------------
// �X�R�A�{�[�h
//---------------------------------------------------------------
void InitScoreBoard()
{
	g_ScoreBoard.nA = 255;
	g_ScoreBoard.tScoreBoard = GetTextureStatus(TEX_GAME_SCORE_BOARD);
	g_ScoreBoard.vPos =
	{
		WINDOW_WIDTH - (g_ScoreBoard.tScoreBoard.u / 2.0f) + 1.0f,
		g_ScoreBoard.tScoreBoard.v / 2.0f - 2.0f,
	};
}

void UninitScoreBoard()
{

}

void UpdateScoreBoard()
{

}

void DrawScoreBoard()
{
	SetTexture(TEX_GAME_SCORE_BOARD);
	SetPolygonColor(D3DCOLOR_ARGB(g_ScoreBoard.nA, 255, 255, 255));
	DrawPolygon
	(
		g_ScoreBoard.vPos.x,
		g_ScoreBoard.vPos.y,
		0,
		0,
		g_ScoreBoard.tScoreBoard.u,
		g_ScoreBoard.tScoreBoard.v
	);
}

//---------------------------------------------------------------
// �X�R�A����
//---------------------------------------------------------------
void InitScoreWord()
{
	g_ScoreWord.nA = 255;
	g_ScoreWord.tScoreWord = GetTextureStatus(TEX_GAME_SCORE_WORD);
	g_ScoreWord.vPos = gc_vScoreWordInitPos;
}

void UninitScoreWord()
{

}

void UpdateScoreWord()
{

}

void DrawScoreWord()
{
	SetTexture(TEX_GAME_SCORE_WORD);
	SetPolygonColor(D3DCOLOR_ARGB(g_ScoreWord.nA, 255, 255, 255));
	DrawPolygon
	(
		g_ScoreWord.vPos.x,
		g_ScoreWord.vPos.y,
		0,
		0,
		g_ScoreWord.tScoreWord.u,
		g_ScoreWord.tScoreWord.v
	);
}

void ResetScore()
{
	score = 0;
	display_score = 0;
}