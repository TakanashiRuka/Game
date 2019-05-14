#pragma once

#include "..\framework\entity.h"

class ResultScore : public Entity
{
private:
	unsigned int save_data_score_ = 0;
	unsigned int score_ = 0;

	bool read_ = false;

public:
	ResultScore(Scene* parent);
	~ResultScore();

private:
	void Update() override;
	void Draw(RenderingPass pass) override;

public:
	unsigned int& GetScore();
	unsigned int GetSaveDataScore();
};