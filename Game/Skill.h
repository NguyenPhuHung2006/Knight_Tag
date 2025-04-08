#pragma once

#include "TransformComponent.h"
#include "SpriteComponent.h"

class Skill
{
private:
	void reset();

public:
	int superSpeedIndex, invincibleIndex, invisibleIndex, slowedIndex, freezeIndex;
	int super_vel, slowed_vel;
	int skillDuration;

	int skillIndex;
	int index;

	Uint32 skillStartTimer, skillEndTimer;

	bool hasOpenChest, hasPower;

	Skill();

	void init(int super_vel, int slowed_vel, int skill_duration);
	void update();
};
