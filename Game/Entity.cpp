#include "Entity.h"
#include "Player.h"
#include "GameConstant.h"
#include "Game.h"
#include <set>
#include <queue>
#include <unordered_map>

extern unordered_map<int, Entity*> entities;
extern unordered_map<int, Player*> players;
extern Map* map;

Entity::Entity(int index)
{
	transform.index = index;
	sprite.index = index;
	collider.index = index;

	collider.transformLink = &transform;

	entities[index] = this;

	this->index = index;
}

void Entity::updateEntity()
{
	checkOpponent();
	collider.update();
	transform.update();
	sprite.update();
}

void Entity::drawEntity()
{
	sprite.draw();
}

bool hasLineOnSight(SDL_Point entity, SDL_Point opponent) // this is the bresenham algorithm
{
	int dx = abs(opponent.x - entity.x);
	int sx = (entity.x < opponent.x ? 1 : -1);

	int dy = -abs(opponent.y - entity.y);
	int sy = (entity.y < opponent.y ? 1 : -1);

	int err = dx + dy;
	int x = entity.x;
	int y = entity.y;

	while (!(x == opponent.x && y == opponent.y))
	{
		int roundedX = x / GameConstants::TileConstants::destWidth;
		int roundedY = y / GameConstants::TileConstants::destHeight;

		if (x < 0 || y < 0 || x > GameConstants::windowWidth || y > GameConstants::windowHeight || map->colliderBit[roundedX][roundedY])
			return false;

		int e = 2 * err;

		if (e >= dy)
		{
			err += dy;
			x += sx;
		}
		if (e <= dx)
		{
			err += dx;
			y += sy;
		}
	}
	return true;
}

inline bool operator==(const SDL_Point& a, const SDL_Point& b)
{
	return a.x == b.x && a.y == b.y;
}

bool Entity::hasReachTarget(SDL_Point start, SDL_Point target) 
{
	const int tolerance = 10; // this is used to show if the start point is approximately equal to the target
	return abs(start.x - target.x) <= tolerance
		&& abs(start.y - target.y) <= tolerance;
}

struct Node
{
	int x, y, dx, dy, g, h, f;
	bool operator<(const Node& other) const
	{
		return f > other.f || (f == other.f && g > other.g);
	}

	// this constructor is for the A* algorithm
	Node(int x, int y, int g, int h, int f)
	{
		this->x = x;
		this->y = y;
		this->g = g;
		this->h = h;
		this->f = f;
	}

	// this is for the findDirToTarget function, find the best direction to reach the target
	Node(int dx, int dy, int f)
	{
		this->dx = dx;
		this->dy = dy;
		this->f = f;
	}
};

int heuristic(SDL_Point p1, SDL_Point p2) // calculate the expected distance between p1 and p2
{
	return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

bool isCollideWithCollider(SDL_Rect rect)
{
	vector<vector<bool>>& colliderBit = map->colliderBit;

	int tileDestWidth = GameConstants::TileConstants::destWidth;
	int tileDestHeight = GameConstants::TileConstants::destHeight;

	int roundedX = rect.x / tileDestWidth;
	int roundedY = rect.y / tileDestHeight;

	int roundedXPlusW = (rect.x + rect.w) / tileDestWidth;
	int roundedYPlusH = (rect.y + rect.h) / tileDestHeight;

	return colliderBit[roundedX][roundedY]
		|| colliderBit[roundedX][roundedYPlusH]
		|| colliderBit[roundedXPlusW][roundedY]
		|| colliderBit[roundedXPlusW][roundedYPlusH];
}

vector<pair<int, int>> Entity::takeValidDirs(vector<pair<int, int>> dirs, SDL_Rect entityRect)
{
	set<pair<int, int>> notValidDirs;

	for (pair<int, int> dir : dirs)
	{
		int dx = dir.first, dy = dir.second;
		SDL_Rect nextEntityRect = {
			entityRect.x + dx * transform.vel,
			entityRect.y + dy * transform.vel,
			entityRect.w,
			entityRect.h
		};

		vector<vector<bool>>& colliderBit = map->colliderBit;
		// the colliderBit is a vector which shows if the current tile is a collider or not

		if (isCollideWithCollider(nextEntityRect))
			notValidDirs.insert(dir);
	}

	vector<pair<int, int>> validDirs;

	for (pair<int, int> dir : dirs)
	{
		if (notValidDirs.find(dir) == notValidDirs.end())
			validDirs.push_back(dir);
	}
	return validDirs;
}

void Entity::findDirToTarget(SDL_Point target) // give the best direction X and Y possible to reach the target
{
	SDL_Rect currentEntityRect = transform.destRect;

	priority_queue<Node> pq;

	vector<pair<int, int>> validDirs = takeValidDirs(dirs8, currentEntityRect);

	for (pair<int, int> dir : validDirs)
	{
		int dx = dir.first, dy = dir.second;
		int nextX = currentEntityRect.x + dx * transform.vel;
		int nextY = currentEntityRect.y + dy * transform.vel;

		SDL_Point nextPoint = { nextX, nextY };

		pq.push({ dx,dy,heuristic(nextPoint,target) });

	}
	if (!pq.empty())
	{
		transform.dirX = pq.top().dx;
		transform.dirY = pq.top().dy;

		int nextOfficialX = currentEntityRect.x + transform.dirX * transform.vel;
		int nextOfficialY = currentEntityRect.y + transform.dirY * transform.vel;

		SDL_Point nextOfficialEntityPoint = { nextOfficialX, nextOfficialY };

		if (isMovingToOrigin && hasReachTarget(nextOfficialEntityPoint, target) && target == pathToOrigin.top())
			pathToOrigin.pop();
		if (isMovingToLastSeenPoint && hasReachTarget(nextOfficialEntityPoint, target) && target == pathToLastSeenPoint.top())
			pathToLastSeenPoint.pop();
	}
	else
	{
		transform.dirX = 0;
		transform.dirY = 0;
	}
}

// reconstruct the path from the hashmap
void Entity::reconstructPath(const unordered_map<SDL_Point, SDL_Point, SDLPointHash, SDLPointEqual>& cameFrom, SDL_Point currentPoint, stack<SDL_Point>& path)
{
	while (cameFrom.count(currentPoint))
	{
		path.push(currentPoint);
		currentPoint = cameFrom.at(currentPoint);
	}
	path.push(currentPoint);
}

// the A* algorithm
void Entity::setPathToTarget(SDL_Point target, stack<SDL_Point>& path)
{
	priority_queue<Node> pq;
	unordered_map<SDL_Point, int, SDLPointHash, SDLPointEqual> gScore;
	unordered_map<SDL_Point, SDL_Point, SDLPointHash, SDLPointEqual> cameFrom;

	SDL_Rect startEntityRect = transform.destRect;
	SDL_Point startEntityPoint = { startEntityRect.x, startEntityRect.y };

	int startHeuristic = heuristic(startEntityPoint, target);

	pq.push({ startEntityPoint.x, startEntityPoint.y, 0, startHeuristic, startHeuristic });
	gScore[startEntityPoint] = 0;

	while (!pq.empty())
	{
		Node currentNode = pq.top();
		pq.pop();

		SDL_Point currentPoint = { currentNode.x, currentNode.y };
		SDL_Rect currentRect = { currentNode.x, currentNode.y, transform.destRect.w, transform.destRect.h };

		if (hasReachTarget(currentPoint, target))
		{
			reconstructPath(cameFrom, currentPoint, path);
			return;
		}

		vector<pair<int, int>> validDirs = takeValidDirs(dirs8, currentRect);

		for (pair<int, int> dir : validDirs)
		{
			int dx = dir.first, dy = dir.second;
			int nextX = currentNode.x + dx * transform.vel;
			int nextY = currentNode.y + dy * transform.vel;

			int nextG = gScore[currentPoint] + 1;

			SDL_Point nextPoint = { nextX, nextY };

			if (!gScore.count(nextPoint) || nextG < gScore[nextPoint])
			{
				int nextH = heuristic(nextPoint, target);
				int nextF = nextG + nextH;

				pq.push({ nextX, nextY, nextG, nextH, nextF });

				gScore[nextPoint] = nextG;
				cameFrom[nextPoint] = currentPoint;
			}
		}
	}
}

void clearPath(stack<SDL_Point>& path)
{
	while (!path.empty())
	{
		path.pop();
	}
}

void Entity::checkOpponent()
{
	int opponentIndex = (index - 2) ^ 1;
	SDL_Rect opponentRect = players[opponentIndex]->transform.destRect;

	SDL_Point entityPoint = { transform.destRect.x, transform.destRect.y };
	SDL_Point opponentPoint = { opponentRect.x , opponentRect.y };

	SDL_Point originalEntityPoint = {};

	if (index == Index::redEntity)
		originalEntityPoint = { GameConstants::RedEntity::destX, GameConstants::RedEntity::destY };
	else if (index == Index::blueEntity)
		originalEntityPoint = { GameConstants::BlueEntity::destX, GameConstants::BlueEntity::destY };

	bool inScope = ((opponentIndex == Index::bluePlayer && opponentRect.x < GameConstants::RedKnight::boundary)
		|| (opponentIndex == Index::redPlayer && opponentRect.x + opponentRect.w > GameConstants::BlueKnight::boundary));

	bool canSee = (!players[opponentIndex]->sprite.isInvisible && hasLineOnSight(entityPoint, opponentPoint));

	if (inScope && canSee)
	{
		sprite.playAnimation("Walk");
		findDirToTarget(opponentPoint);

		isMovingToOrigin = false;
		isMovingToLastSeenPoint = false;
		isIdle = false;

		clearPath(pathToOrigin);
		clearPath(pathToLastSeenPoint);

		lastSeenPoint = {}; // the lastSeenPoint mean the last position when the entity can't see the opponent
	}
	else
	{
		bool canFindOpponent = !isCollideWithCollider(opponentRect);

		// this is the lambda function which is used when the entity can't see the opponent at the lastSeenPoint
		auto startMovingToOrigin = [&]() {
			clearPath(pathToLastSeenPoint);
			isMovingToLastSeenPoint = false;
			isMovingToOrigin = true;
			lastSeenPoint = {};

			if(pathToOrigin.empty())
				setPathToTarget(originalEntityPoint, pathToOrigin);
		};

		if (!canSee && !isMovingToLastSeenPoint && !isMovingToOrigin && inScope && !isIdle && canFindOpponent)
		{
			lastSeenPoint = opponentPoint;
			isMovingToLastSeenPoint = true;
			setPathToTarget(lastSeenPoint, pathToLastSeenPoint);
		}

		else if (isMovingToLastSeenPoint)
		{
			if (!hasReachTarget(entityPoint, lastSeenPoint) && !pathToLastSeenPoint.empty())
				findDirToTarget(pathToLastSeenPoint.top());
			else
				startMovingToOrigin();
		}

		else if (hasTaggedOpponent || (!inScope && !isIdle && !isMovingToOrigin))
		{
			startMovingToOrigin();
			hasTaggedOpponent = false;
		}

		else if (isMovingToOrigin)
		{
			if (!hasReachTarget(entityPoint, originalEntityPoint) && !pathToOrigin.empty())
				findDirToTarget(pathToOrigin.top());
			else
			{
				isMovingToOrigin = false;
				isMovingToLastSeenPoint = false;
				isIdle = true;

				clearPath(pathToOrigin);
			}
		}

		else if (isIdle)
		{
			sprite.playAnimation("Idle");
			transform.dirX = 0;
			transform.dirY = 0;
		}
 	}

}
