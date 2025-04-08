#pragma once

#include "Map.h"
#include <SDL.h>
#include <memory>

struct TransformComponent
{
	SDL_Rect destRect;
	int dirX, dirY, vel;
	bool collideX, collideY;
	int index;

	TransformComponent();

	void init(int x, int y, int w, int h, int v);

	void update();
};
