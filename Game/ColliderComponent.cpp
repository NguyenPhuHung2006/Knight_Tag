#include "Game.h"
#include "Player.h"
#include "Entity.h"
#include "GameConstant.h"
#include "Map.h"
#include <iostream>
using namespace GameConstants;

extern unordered_map<int, Player*> players;
extern unordered_map<int, Entity*> entities;
extern Map* map;
extern Game* game;

void ColliderComponent::update()
{
	checkWall();
	// if the current one is the player
	if (index <= 1)
	{
		checkBed();
		checkTagged();
		checkChest();
	}
}

void ColliderComponent::checkWall()
{
	const vector<unique_ptr<Tile>>& colliders = map->colliderTiles;

	SDL_Rect nextRect = transformLink->destRect;

	int velocity = transformLink->vel;

	nextRect.x += transformLink->dirX * velocity;
	nextRect.y += transformLink->dirY * velocity;

	for (const unique_ptr<Tile>& collider : colliders)
	{
		if (SDL_HasIntersection(&nextRect, &collider->destRect))
		{
			SDL_Rect currentRect = transformLink->destRect;

			SDL_Rect testX = currentRect;
			testX.x += transformLink->dirX * velocity;
			if (SDL_HasIntersection(&testX, &collider->destRect))
			{
				// if the player has intersection with the colliders, the player will be shifted back to the original position 
				if (transformLink->dirX > 0)
					transformLink->destRect.x = collider->destRect.x - transformLink->destRect.w;

				else if (transformLink->dirX < 0)
					transformLink->destRect.x = collider->destRect.x + collider->destRect.w;

				transformLink->collideX = true;

				if (collider->isChest && collider->collideBy < 0 && index <= 1 && !players[index]->skills.hasOpenChest)
					collider->collideBy = index;
			}

			// same to the x coordinate
			SDL_Rect testY = currentRect;
			testY.y += transformLink->dirY * velocity;
			if (SDL_HasIntersection(&testY, &collider->destRect))
			{
				if (transformLink->dirY > 0)
					transformLink->destRect.y = collider->destRect.y - transformLink->destRect.h;

				else if (transformLink->dirY < 0)
					transformLink->destRect.y = collider->destRect.y + collider->destRect.h;

				transformLink->collideY = true;

				if (collider->isChest && collider->collideBy < 0 && index <= 1 && !players[index]->skills.hasOpenChest)
					collider->collideBy = index;
			}
		}
	}

}

void ColliderComponent::checkBed()
{
	// i will expand the bed rect by 1 pixel because the checkWall function has shifted back the player when it has collision
	SDL_Rect redBedRect = {
		Environment::Bed::redDestX,
		Environment::Bed::redDestY - 1,
		TileConstants::destWidth + 1,
		TileConstants::destHeight + 2
	};

	SDL_Rect blueBedRect = {
		Environment::Bed::blueDestX - 1,
		Environment::Bed::blueDestY - 1,
		TileConstants::destWidth,
		TileConstants::destHeight + 2
	};

	Player* redKnight = players[Index::redPlayer];
	Player* blueKnight = players[Index::bluePlayer];

	if (index == Index::bluePlayer)
	{
		if (SDL_HasIntersection(&redBedRect,&players[index]->transform.destRect) && !players[index]->hasBed)
		{
			blueKnight->hasBed = true;
			Mix_PlayChannel(-1, game->hasBedSoundEffect, 0);
		}

		if (SDL_HasIntersection(&blueBedRect, &players[index]->transform.destRect) && players[index]->hasBed)
		{
			blueKnight->hasBed = false;
			blueKnight->score++;
			Mix_PlayChannel(-1, game->returnBedSoundEffect, 0);
		}
	}
	else if (index == Index::redPlayer)
	{
		if (SDL_HasIntersection(&blueBedRect, &players[index]->transform.destRect) && !players[index]->hasBed)
		{
			redKnight->hasBed = true;
			Mix_PlayChannel(-1, game->hasBedSoundEffect, 0);
		}

		if (SDL_HasIntersection(&redBedRect, &players[index]->transform.destRect) && players[index]->hasBed)
		{
			redKnight->hasBed = false;
			redKnight->score++;
			Mix_PlayChannel(-1, game->returnBedSoundEffect, 0);
		}
	}
}

void ColliderComponent::checkTagged()
{
	SDL_Rect currentPlayerRect = players[index]->transform.destRect;
	SDL_Rect otherPlayerRect = players[index ^ 1]->transform.destRect;
	// the XOR will work because i assign the index of the players as binary

	SDL_Rect otherEntityRect = entities[index ^ 1 + 2]->transform.destRect;

	if (SDL_HasIntersection(&currentPlayerRect, &otherPlayerRect) && !isTagged)
	{
		if (((index == Index::redPlayer
			&& currentPlayerRect.x < RedKnight::boundary
			&& otherPlayerRect.x < RedKnight::boundary)
			||
			(index == Index::bluePlayer
			&& currentPlayerRect.x + currentPlayerRect.w > BlueKnight::boundary
			&& otherPlayerRect.x > BlueKnight::boundary))
			&& 
			!players[index ^ 1]->sprite.isInvincible)
		{
			resetPlayerAfterTagged(index ^ 1);
		}
	}
	if (SDL_HasIntersection(&currentPlayerRect, &otherEntityRect) && !isTagged)
	{
		if (((index == Index::bluePlayer
			&& currentPlayerRect.x < RedKnight::boundary
			&& otherEntityRect.x < RedKnight::boundary)
			||
			(index == Index::redPlayer
			&& currentPlayerRect.x + currentPlayerRect.w > BlueKnight::boundary
			&& otherEntityRect.x > BlueKnight::boundary))
			&&
			!players[index]->sprite.isInvincible)
		{
			resetPlayerAfterTagged(index);
			entities[index ^ 1 + 2]->hasTaggedOpponent = true;
		}
	}
	if (isTagged)
	{
		reviveEndTimer = SDL_GetTicks();
		// i decrease the time by the game->pause because the game has the pause effect, so this will make the timer works properly
		if (reviveEndTimer - reviveStartTimer - game->pauseTime > Gameplay::reviveDuration)
			isTagged = false;
	}
}

void ColliderComponent::resetPlayerAfterTagged(int index)
{
	Player* player = players[index];
	player->hasBed = false;
	player->collider.isTagged = true;
	if (index == Index::redPlayer)
	{
		player->transform.destRect.x = RedKnight::destX;
		player->transform.destRect.y = RedKnight::destY;
	}
	else if(index == Index::bluePlayer)
	{
		player->transform.destRect.x = BlueKnight::destX;
		player->transform.destRect.y = BlueKnight::destY;
	}
	player->transform.dirX = 0;
	player->transform.dirY = 0;
	player->collider.reviveStartTimer = SDL_GetTicks() - game->pauseTime;
}

void ColliderComponent::checkChest()
{
	const vector<unique_ptr<Tile>>& colliders = map->colliderTiles;

	for (const unique_ptr<Tile>& collider : colliders)
	{
		if (collider->isChest && collider->collideBy >= 0)
		{
			if (!players[index]->skills.hasOpenChest
				&& collider->srcRect.x == Environment::Chest::closedSrcX
				&& collider->srcRect.y == Environment::Chest::closedSrcY
				&& !players[index]->skills.hasPower)
			{
				collider->srcRect.x = Environment::Chest::openSrcX;
				collider->srcRect.y = Environment::Chest::openSrcY;

				players[index]->skills.hasOpenChest = true;

				collider->collideBy = index;

				Mix_PlayChannel(-1, game->openChestSoundEffect, 0);
			}
			else if (collider->srcRect.x == Environment::Chest::openSrcX
				&& collider->srcRect.y == Environment::Chest::openSrcY
				&& !players[collider->collideBy]->skills.hasPower 
				&& !players[collider->collideBy]->skills.hasOpenChest)
			{
				collider->srcRect.x = Environment::Chest::closedSrcX;
				collider->srcRect.y = Environment::Chest::closedSrcY;

				collider->collideBy = -1;
			}
		}
	}
}
