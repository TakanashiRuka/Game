//----------------------------------------------------------------------------
//
//	���U���g�̕]��
//
//														 ����ҁF����
//                                                       �쐬���F2018/01/23
//----------------------------------------------------------------------------

#ifndef RESULT_ACHIEVEMENT_H
#define RESULT_ACHIEVEMENT_H

//---------------------------�C���N���[�h-------------------------------------

//---------------------------�}�N����`---------------------------------------

//---------------------------�񋓌^��`---------------------------------------
typedef enum {
	RAP_FULL_COMBO,
	RAP_ALL_PERFECT,
	RAP_MAX
}RESULT_ACHIEVEMENT_PARTS;

//---------------------------�\���̒�`---------------------------------------

//---------------------------�O���[�o���ϐ�-----------------------------------

//---------------------------�v���g�^�C�v�錾---------------------------------
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

