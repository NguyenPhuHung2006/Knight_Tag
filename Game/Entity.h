#pragma once

#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "ColliderComponent.h"
#include "GameConstant.h"
#include <stack>

struct SDLPointEqual
{
	bool operator()(const SDL_Point& a, const SDL_Point& b) const
	{
		return a.x == b.x && a.y == b.y;
	}
};

struct SDLPointHash
{
	size_t operator()(const SDL_Point& p) const
	{
		return hash<int>()(p.x) ^ (hash<int>()(p.y) << 1);
	}
};

class Entity
{
protected:
	vector<pair<int, int>> dirs8 = {
	{-1, 0}, {1, 0}, {0, -1}, {0, 1},
	{-1, -1}, {-1, 1}, {1, -1}, {1, 1}
	};

private:
	void checkOpponent();
	bool hasReachTarget(SDL_Point start, SDL_Point target);
	void findDirToTarget(SDL_Point target);
	void setPathToTarget(SDL_Point target, stack<SDL_Point>& path);
	void reconstructPath(const unordered_map<SDL_Point, SDL_Point, SDLPointHash, SDLPointEqual>& cameFrom, SDL_Point currentPoint, stack<SDL_Point>& target);

	stack<SDL_Point> pathToOrigin;
	stack<SDL_Point> pathToLastSeenPoint;

	vector<pair<int, int>> takeValidDirs(vector<pair<int, int>> dirs, SDL_Rect entityRect);

	bool isMovingToOrigin = false;
	bool isMovingToLastSeenPoint = false;
	bool isIdle = true;

	SDL_Point lastSeenPoint = {};

public:
	TransformComponent transform;
	SpriteComponent sprite;
	ColliderComponent collider;

	int index;
	bool hasTaggedOpponent = false;

	Entity(int index);
	void updateEntity();
	void drawEntity();
};
