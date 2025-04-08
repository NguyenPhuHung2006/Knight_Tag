#include "Game.h"
#include "GameConstant.h"
#include "Player.h"
#include <unordered_map>
#include <vector>
#include <iostream>

TransformComponent::TransformComponent() = default;

void TransformComponent::init(int x, int y, int w, int h, int v)
{
	destRect.x = x;
	destRect.y = y;
	destRect.w = w;
	destRect.h = h;
	dirX = dirY = 0;
	collideX = collideY = false;
	vel = v;
}

void TransformComponent::update()
{
	if (!collideX && !collideY && dirX != 0 && dirY != 0)
	{
		// i will make the player slower when it move diagonally
		// just remember that 0,7 approximately equals to sqrt(2)/2,
		destRect.x += (int)(dirX * vel * 0.7f);
		destRect.y += (int)(dirY * vel * 0.7f);
	}
	else
	{
		if (!collideX)
			destRect.x += dirX * vel;
		if (!collideY)
			destRect.y += dirY * vel;
	}
	collideX = false;
	collideY = false;

}
