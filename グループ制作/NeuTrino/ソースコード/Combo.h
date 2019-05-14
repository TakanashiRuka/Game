//----------------------------------------------------------------------------
//
//	コンボ
//
//														 制作者：劉��
//                                                       作成日：2017/12/25
//----------------------------------------------------------------------------

#ifndef COMBO_H
#define COMBO_H

//---------------------------インクルード-------------------------------------

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------
typedef enum {
	CC_ADD,
	CC_RESET,
}COMBO_COMMEND;

//---------------------------構造体定義---------------------------------------

//---------------------------グローバル変数-----------------------------------

//---------------------------プロトタイプ宣言---------------------------------
void InitCombo();
void UninitCombo();
void UpdateCombo();
void DrawCombo();

//------------------------------------------------------------------------------
//	関数名:	SetCombo
//	引数:	コンボコマンド
//	戻り値:	なし
//	説明:	コンボの値を変更させる
//------------------------------------------------------------------------------
void SetCombo(int nComboCommend);

void InitComboWord();
void UninitComboWord();
void UpdateComboWord();
void DrawComboWord();

void InitMaxComboWord();
void UninitMaxComboWord();
void UpdateMaxComboWord();
void DrawMaxComboWord();

void InitMaxComboNum();
void UninitMaxComboNum();
void UpdateMaxComboNum();
void DrawMaxComboNum();

//------------------------------------------------------------------------------
//	関数名: GetMaxCombo
//	引数:	なし
//	戻り値:	最大コンボ数
//	説明:	最大コンボ数を取得する
//------------------------------------------------------------------------------
int GetMaxCombo();

#endif // !COMBO_H

