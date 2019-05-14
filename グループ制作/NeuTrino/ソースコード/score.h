//----------------------------------------------------------------------------
//
//	スコア
//
//														 制作者：高梨　琉佳
//                                                       作成日：2017/12/23
//----------------------------------------------------------------------------

#ifndef _SCORE_H_
#define _SCORE_H_

//プロトタイプ宣言
void InitScore();
void UninitScore();
void DrawScore();
void UpdateScore();

//------------------------------------------------------------------------------
//	関数名:	AddScore
//	引数:	音符の種類, 判定レベル
//	戻り値:	なし
//	説明:	音符の種類と判定レベルに応じたスコアの更新
//------------------------------------------------------------------------------
void AddScore(int note, int judgeLevel);

//------------------------------------------------------------------------------
//	関数名:	GetScore
//	引数:	なし
//	戻り値:	現在のスコア数
//	説明:	現在のスコア数をfloat型で返す
//------------------------------------------------------------------------------
float GetScore();

float GetDisplayScore();

void ResetScore();

void InitScoreBoard();
void UninitScoreBoard();
void UpdateScoreBoard();
void DrawScoreBoard();

void InitScoreWord();
void UninitScoreWord();
void UpdateScoreWord();
void DrawScoreWord();

#endif // !_SCORE_H_