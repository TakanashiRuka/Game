//----------------------------------------------------------------------------
//
//	タイトルの背景部分
//
//														 制作者：劉��
//                                                       作成日：2017/01/27
//----------------------------------------------------------------------------

#ifndef TITLE_BG_H
#define TITLE_BG_H

//---------------------------インクルード-------------------------------------

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------

//---------------------------構造体定義---------------------------------------

//---------------------------グローバル変数-----------------------------------

//---------------------------プロトタイプ宣言---------------------------------
void InitTitleBG();
void UninitTitleBG();
void UpdateTitleBG();
void DrawTitleBG();

void DrawTitleAllRightReserved();

void InitVersion();
void UninitVersion();
void UpdateVersion();
void DrawVersion();

void InitTitleMask();
void UninitTitleMask();
void UpdateTitleMask();
void DrawTitleMask();

#endif // !TITLE_BG_H
