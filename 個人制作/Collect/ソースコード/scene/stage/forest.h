#pragma once

#include "..\..\framework\scene.h"

#include "..\..\entity\color.h"
#include "..\..\entity\light.h"
#include "..\..\entity\sky_box.h"
#include "..\..\entity\mesh_field.h"
#include "..\..\entity\ground.h"
#include "..\..\entity\rock.h"
#include "..\..\entity\tree.h"
#include "..\..\entity\grass.h"
#include "..\..\entity\player.h"
#include "..\..\entity\target.h"
#include "..\..\entity\time_bg.h"
#include "..\..\entity\time.h"
#include "..\..\entity\score_bg.h"
#include "..\..\entity\high_score.h"
#include "..\..\entity\score.h"
#include "..\..\entity\theme.h"
#include "..\..\entity\combo.h"
#include "..\..\entity\combo_num.h"
#include "..\..\entity\operation.h"
#include "..\..\entity\game_wait.h"
#include "..\..\entity\fruit_index.h"
#include "..\..\entity\fruit.h"

class Forest : public Scene
{
public:
	Forest()
	{
		this->AddEntity<Color>();
		this->AddEntity<Light>();
		this->AddEntity<SkyBox>();
		this->AddEntity<MeshField>();
		this->AddEntity<Ground>();
		this->AddEntity<Rock>();
		this->AddEntity<Tree>();
		this->AddEntity<Grass>();
		this->AddEntity<Player>();
		this->AddEntity<Target>();
		this->AddEntity<TimeBG>();
		this->AddEntity<Time>(60.999);
		this->AddEntity<ScoreBG>();
		this->AddEntity<HighScore>();
		this->AddEntity<Score>();
		this->AddEntity<Theme>();
		this->AddEntity<Combo>();
		this->AddEntity<ComboNum>();
		this->AddEntity<Operation>();
		this->AddEntity<GameWait>();
		this->AddEntity<FruitIndex>();
		unsigned int max = 100;
		for (unsigned int fruit = 0; fruit < max; ++fruit)
			this->AddEntity<Fruit>();
	}
};