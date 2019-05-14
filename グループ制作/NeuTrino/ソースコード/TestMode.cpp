//----------------------------------------------------------------------------
//
//	�e�X�g���[�h
//
//														 ����ҁF����
//                                                       �쐬���F2018/01/19
//----------------------------------------------------------------------------

//---------------------------�C���N���[�h-------------------------------------
#include "main.h"
#include "debug.h"
#include "input.h"
#include "TestMode.h"
#include"network.h"		//�ǉ�

//---------------------------�}�N����`---------------------------------------

//---------------------------�񋓌^��`---------------------------------------

//---------------------------�\���̒�`---------------------------------------

//---------------------------�O���[�o���ϐ�-----------------------------------
static bool g_bTestModeMember[TMML_MAX]=
{
	false,
	false,
	false,
	false,
};

static const char gc_cTestModeMemberName[TMML_MAX][MAX_WORD]=
{
	"GUEST MODE",
	"NETWORK",
	"DEMO MODE",
	"XINPUT",
	"EDIT MODE",
	"AUTO PLAY",
};

static const D3DXVECTOR2 gc_vTestModeTitleInitPos = { 50.0f,50.0f };

static const D3DXVECTOR2 gc_vTestModeMemberInitPos = { 50.0f,100.0f };
static const D3DXVECTOR2 gc_vTestModeMemberPosRange = { 0.0f,30.0f };
static const D3DXVECTOR2 gc_vTestModeMemberStatusPosRange = { 200.0f,0.0f };
static const D3DXVECTOR2 gc_vTestModeArrowPosRange = { -30.0f,0.0f };
static int g_nSelect;

//---------------------------�v���g�^�C�v�錾---------------------------------
void InitTestMode()
{
	InitText(F_TEST_MODE, 26, "MS �R�W�b�N");
	g_nSelect = 0;
}

void UninitTestMode()
{
	UninitText(F_TEST_MODE);
}

void UpdateTestMode()
{
	if (GetKeyboardRepeat(DIK_UP))
	{
		g_nSelect--;
		if (g_nSelect < 0)
		{
			g_nSelect = 0;
		}
	}
	if (GetKeyboardRepeat(DIK_DOWN))
	{
		g_nSelect++;
		if (g_nSelect > TMML_MAX - 1)
		{
			g_nSelect = TMML_MAX - 1;
		}
	}
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		if (g_bTestModeMember[g_nSelect])
		{
			g_bTestModeMember[g_nSelect] = false;
			if (g_nSelect == TEST_MODE_MEMBER_LIST::TMML_NETWORK) UninitUDP();	//�ǉ�
		}
		else
		{
			g_bTestModeMember[g_nSelect] = true;
			if (g_nSelect == TEST_MODE_MEMBER_LIST::TMML_NETWORK) InitUDP();	//�ǉ�
		}
	}
}

void DrawTestMode()
{
	SetTextStatus(D3DCOLOR_ARGB(255, 255, 255, 255), D3DCOLOR_ARGB(255, 0, 0, 0), 1);
	SetTextStyle(DT_LEFT);
	OutputText(F_TEST_MODE, gc_vTestModeTitleInitPos.x, gc_vTestModeTitleInitPos.y, "TEST MODE");

	// �����o�[��
	for (int nCountMember = 0; nCountMember < TMML_MAX; nCountMember++)
	{
		SetTextStatus(D3DCOLOR_ARGB(255, 255, 255, 255), D3DCOLOR_ARGB(255, 0, 0, 0), 1);
		SetTextStyle(DT_LEFT);
		OutputText
		(
			F_TEST_MODE,
			gc_vTestModeMemberInitPos.x + nCountMember * gc_vTestModeMemberPosRange.x,
			gc_vTestModeMemberInitPos.y + nCountMember * gc_vTestModeMemberPosRange.y,
			"%s",
			&gc_cTestModeMemberName[nCountMember]
		);
	}

	// �����o�[�̏��
	for (int nCountMember = 0; nCountMember < TMML_MAX; nCountMember++)
	{
		SetTextStyle(DT_LEFT);
		if (g_bTestModeMember[nCountMember])
		{
			SetTextStatus(D3DCOLOR_ARGB(255, 0, 255, 0), D3DCOLOR_ARGB(255, 0, 0, 0), 1);
			OutputText
			(
				F_TEST_MODE,
				gc_vTestModeMemberInitPos.x + nCountMember * (gc_vTestModeMemberPosRange.x) + gc_vTestModeMemberStatusPosRange.x,
				gc_vTestModeMemberInitPos.y + nCountMember * (gc_vTestModeMemberPosRange.y) + gc_vTestModeMemberStatusPosRange.y,
				"ON"
			);
		}
		else
		{
			SetTextStatus(D3DCOLOR_ARGB(255, 255, 0, 0), D3DCOLOR_ARGB(255, 0, 0, 0), 1);
			OutputText
			(
				F_TEST_MODE,
				gc_vTestModeMemberInitPos.x + nCountMember * (gc_vTestModeMemberPosRange.x) + gc_vTestModeMemberStatusPosRange.x,
				gc_vTestModeMemberInitPos.y + nCountMember * (gc_vTestModeMemberPosRange.y) + gc_vTestModeMemberStatusPosRange.y,
				"OFF"
			);
		}
	}

	// ���
	SetTextStatus(D3DCOLOR_ARGB(255, 255, 255, 255), D3DCOLOR_ARGB(255, 0, 0, 0), 1);
	SetTextStyle(DT_LEFT);
	OutputText
	(
		F_TEST_MODE, 
		gc_vTestModeMemberInitPos.x + gc_vTestModeArrowPosRange.x,
		gc_vTestModeMemberInitPos.y + gc_vTestModeArrowPosRange.y + g_nSelect * gc_vTestModeMemberPosRange.y,
		">"
	);

}

bool GetTestModeMemberStatus(int nNum)
{
	return g_bTestModeMember[nNum];
}

void SetTestModeMemberStatus(int nNum, bool bStatus)
{
	g_bTestModeMember[nNum] = bStatus;
}