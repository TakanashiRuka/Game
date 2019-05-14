#ifndef BILLBOARD_H
#define BILLBOARD_H

void InitBillboard();
void UninitBillboard();
void UpdateBillboard();
void SetBillBoardColor(D3DCOLOR dColor);
void SetBillboardRot(float x, float y, float z);
void SetBillboardCenterPos(float x, float y);
void SetBillboardScale(float x, float y, float z);
void DrawBillboard(float x, float y, float z, int nSU, int nSV, int nWU, int nHV);

#endif // !BILLBOARD_H
