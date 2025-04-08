#pragma once

#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "Skill.h"
#include <random>
using namespace std;

// i will use inline because i want to avoid declaring this in mutiple of file 
inline int RandomSkillIndex(int min, int max)
{
	static mt19937 gen(random_device{}()); // the static means that this will only be declared once
	uniform_int_distribution<> dist(min, max);
	return dist(gen);
}

struct KeyboardComponent
{
	int index;

	KeyboardComponent();

	void update();
};
