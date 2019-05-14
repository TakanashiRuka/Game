//----------------------------------------------------------------------------
//
//	判定文字
//
//														 制作者：高梨　琉佳
//                                                       作成日：2017/12/22
//----------------------------------------------------------------------------

#ifndef _DECISION_H_
#define _DECISION_H_

typedef enum {
	JELT_EARLY,
	JELT_LATE,
	JELT_MAX
}JUDGE_EARLY_LATE_TYPE;

void InitDecision();
void UninitDecision();
void UpdateDecision();
void DrawDecision();

//------------------------------------------------------------------------------
//	関数名:	CreateDecision
//	引数:	判定するレーン,判定レベル
//	戻り値:	なし
//	説明:	判定レベルのエフェクトを生成
//------------------------------------------------------------------------------
void CreateDecision(int lane, int judgeLevel);

void CreateDecisionEarlyLate(int lane, int nType);

#endif // !_DECISION_H_