#pragma once

#include "..\framework\scene.h"

#include "..\entity\color.h"
#include "..\entity\result_bg.h"
#include "..\entity\result_high_score.h"
#include "..\entity\result_score.h"

class Result : public Scene
{
public:
	Result()
	{
		this->AddEntity<Color>();
		this->AddEntity<ResultBG>();
		this->AddEntity<ResultHighScore>();
		this->AddEntity<ResultScore>();
	}
};