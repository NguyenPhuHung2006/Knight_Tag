#pragma once

#include "TransformComponent.h"
#include "Skill.h"
#include "Map.h"
#include <vector>

class ColliderComponent
{
private:
	void checkWall();
	void checkChest();
	void checkBed();
	void checkTagged();

	void resetPlayerAfterTagged(int index);

public:
	Uint32 reviveStartTimer = 0, reviveEndTimer = 0;

	TransformComponent* transformLink = nullptr;

	int index;

	bool isTagged = false;

	void update();
};
