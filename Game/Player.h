#pragma once

#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "ColliderComponent.h"
#include "KeyboardComponent.h"
#include "Skill.h"
#include "TextManager.h"
#include <string>
using namespace std;

struct Player
{
	TransformComponent transform;
	SpriteComponent sprite;
	ColliderComponent collider;
	KeyboardComponent keyboard;
	TextManager scoreText;
	TextManager hasBedText;
	Skill skills;

	bool hasBed;
	int score;

	Player(int index);
	void updatePlayer();
	void drawPlayer();
};
