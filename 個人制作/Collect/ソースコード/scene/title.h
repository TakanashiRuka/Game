#pragma once

#include "..\framework\scene.h"

#include "..\entity\color.h"
#include "..\entity\light.h"
#include "..\entity\title_camera.h"
#include "..\entity\mesh_field.h"
#include "..\entity\ground.h"
#include "..\entity\grass.h"
#include "..\entity\title_name.h"
#include "..\entity\press.h"
#include "..\entity\key_button.h"

class Title : public Scene
{
public:
	Title()
	{
		this->AddEntity<Color>();
		this->AddEntity<Light>();
		this->AddEntity<TitleCamera>();
		this->AddEntity<MeshField>();
		this->AddEntity<Ground>();
		this->AddEntity<Grass>();
		this->AddEntity<TitleName>();
		this->AddEntity<Press>();
		this->AddEntity<KeyButton>();
	}
};