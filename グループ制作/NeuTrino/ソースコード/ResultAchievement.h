//----------------------------------------------------------------------------
//
//	リザルトの評価
//
//														 制作者：劉喆
//                                                       作成日：2018/01/23
//----------------------------------------------------------------------------

#ifndef RESULT_ACHIEVEMENT_H
#define RESULT_ACHIEVEMENT_H

//---------------------------インクルード-------------------------------------

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------
typedef enum {
	RAP_FULL_COMBO,
	RAP_ALL_PERFECT,
	RAP_MAX
}RESULT_ACHIEVEMENT_PARTS;

//---------------------------構造体定義---------------------------------------

//---------------------------グローバル変数-----------------------------------

//---------------------------プロトタイプ宣言---------------------------------
void InitResultAchievement();
void UninitResultAchievement();
void UpdateResultAchievement();
void DrawResultAchievement();

void SetResultAchievement(int nParts, bool bStatus);

void InitResultClearFailed();
void UninitResultClearFailed();
void UpdateResultClearFailed();
void DrawResultClearFailed();

#endif // !RESULT_ACHIEVEMENT_H

