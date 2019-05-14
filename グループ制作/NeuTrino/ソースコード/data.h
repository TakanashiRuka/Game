#ifndef _DATA_H_
#define _DATA_H_

#include<string>

#define PLAYER_NAME ("PLAYER1")

struct Player
{
	std::string name;		//���O
	int character_icon;		//�L�����N�^�[�̃A�C�R��
	int music;				//�Ȃ̔ԍ�
	int music_level;		//�Ȃ̃��x��
	int music_level_num;	//�Ȃ̃��x������
	int game_mode;			//�Q�[���̃��[�h
	float score;			//�X�R�A
	float display_score;
	bool online;			//�l�b�g���[�N���I�����I�t��
	bool room_create;		//���[���̍쐬
	bool room_entry;		//���[���ɎQ��
	bool ready;				//�Q�[���̏������ł��Ă��邩
	bool init_game;			//���[���ɎQ�����ł��Ă��邩
	bool game_preface;
	bool multiplay_cancel;
};

class Data
{
public:
	static Player g_player;
};

//------------------------------------------------------------------------------
//	�֐���:	UpdateSendData
//	����:	�Ȃ�
//	�߂�l:	�Ȃ�
//	����:	���g�̃A�b�v�f�[�g���ꂽ�f�[�^�𑗐M����
//------------------------------------------------------------------------------
void UpdateSendData();

//------------------------------------------------------------------------------
//	�֐���:	DrawRecvDataDebug
//	����:	�Ȃ�
//	�߂�l:	�Ȃ�
//	����:	��M��������̃f�[�^���f�o�b�O�����ŕ`�悷��
//------------------------------------------------------------------------------
void DrawRecvDataDebug();

void ResetData();

#endif // !_DATA_H_