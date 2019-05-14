#pragma once

#include "..\model\model_data.h"

#include <string>

class ModelImport
{
public:
	static Model UIData(float x, float y, float width, float height, float uv_x, float uv_y, float uv_w, float uv_h);
	static Model Number(float x, float y, float width, float height, unsigned int number, unsigned int digit, bool zero_fill);
	static Model BillBoardData(float width, float height, float uv_x, float uv_y, float uv_w, float uv_h);
	static Model FieldData(unsigned int width, unsigned int height, float size_w, float size_h, float max_y);
	static Model ModelData(std::string name);
};