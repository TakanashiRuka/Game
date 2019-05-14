//----------------------------------------------------------------------------
//
//	���蕶��
//
//														 ����ҁF�����@����
//                                                       �쐬���F2017/12/22
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
//	�֐���:	CreateDecision
//	����:	���肷�郌�[��,���背�x��
//	�߂�l:	�Ȃ�
//	����:	���背�x���̃G�t�F�N�g�𐶐�
//------------------------------------------------------------------------------
void CreateDecision(int lane, int judgeLevel);

void CreateDecisionEarlyLate(int lane, int nType);

#endif // !_DECISION_H_