//----------------------------------------------------------------------------
//
//	�Q�[���̔w�i
//
//														 ����ҁF����
//                                                       �쐬���F2017/02/01
//----------------------------------------------------------------------------

#ifndef GAME_BG_H
#define GAME_BG_H

//---------------------------�C���N���[�h-------------------------------------

//---------------------------�}�N����`---------------------------------------

//---------------------------�񋓌^��`---------------------------------------

//---------------------------�\���̒�`---------------------------------------

//---------------------------�O���[�o���ϐ�-----------------------------------

//---------------------------�v���g�^�C�v�錾---------------------------------
void InitGameBG();
void UninitGameBG();
void UpdateGameBG();
void DrawGameBG();

void DrawGameMainBG();

void InitGameBGWall();
void UninitGameBGWall();
void UpdateGameBGWall();
void DrawGameBGWall();

void InitGameBGLine();
void UninitGameBGLine();
void UpdateGameBGLine();
void DrawGameBGLine();

void InitGameBGFloor();
void UninitGameBGFloor();
void UpdateGameBGFloor();
void DrawGameBGFloor();

#endif // !GAME_BG_H
