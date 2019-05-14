//----------------------------------------------------------------------------
//
//	�X�R�A
//
//														 ����ҁF�����@����
//                                                       �쐬���F2017/12/23
//----------------------------------------------------------------------------

#ifndef _SCORE_H_
#define _SCORE_H_

//�v���g�^�C�v�錾
void InitScore();
void UninitScore();
void DrawScore();
void UpdateScore();

//------------------------------------------------------------------------------
//	�֐���:	AddScore
//	����:	�����̎��, ���背�x��
//	�߂�l:	�Ȃ�
//	����:	�����̎�ނƔ��背�x���ɉ������X�R�A�̍X�V
//------------------------------------------------------------------------------
void AddScore(int note, int judgeLevel);

//------------------------------------------------------------------------------
//	�֐���:	GetScore
//	����:	�Ȃ�
//	�߂�l:	���݂̃X�R�A��
//	����:	���݂̃X�R�A����float�^�ŕԂ�
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