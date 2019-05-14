#ifndef XMODEL_H
#define XMODEL_H

typedef enum {
	XM_CART,
	XM_MAX
}LABEL_XMODEL;

void InitXModel();
void UninitXModel();
void SetXModel(int nXMNum);
void SetXMRot(float x, float y, float z);
void SetXMScale(float x, float y, float z);
void DrawXModel(float x, float y, float z);

#endif // !XMODEL_H
