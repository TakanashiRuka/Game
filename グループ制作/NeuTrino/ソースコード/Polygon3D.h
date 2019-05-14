#ifndef POLYGON_3D
#define POLYGON_3D

void InitPolygon3D();
void UninitPolygon3D();
void SetPolygonColor3D(D3DCOLOR d_color);
void SetScale3D(float fX, float fY, float fZ);
void SetRot3D(float fX, float fY, float fZ);
void DrawPolygon3D(float fX, float fY, float fZ, int nSU, int nSV, int nWU, int nHV);

//------------------------------------------------------------------------------
//	�֐���:	SetPolygon3DVertex
//	����:	������W�@�E����W�@�������W�@�E�����W
//	�߂�l:	�Ȃ�
//	����:	�|���S���̒��_���W��ݒ肷��
//------------------------------------------------------------------------------
void SetPolygon3DVertex(D3DXVECTOR3 vPos1, D3DXVECTOR3 vPos2, D3DXVECTOR3 vPos3, D3DXVECTOR3 vPos4);

#endif // !POLYGON_3D
