//----------------------------------------------------------------------------
//
//	ゲームの背景
//
//														 制作者：劉喆
//                                                       作成日：2017/02/01
//----------------------------------------------------------------------------

#ifndef GAME_BG_H
#define GAME_BG_H

//---------------------------インクルード-------------------------------------

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------

//---------------------------構造体定義---------------------------------------

//---------------------------グローバル変数-----------------------------------

//---------------------------プロトタイプ宣言---------------------------------
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
