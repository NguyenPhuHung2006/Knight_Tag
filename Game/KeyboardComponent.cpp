#include "Game.h"
#include "Player.h"
#include "GameConstant.h"

extern Game* game;
extern unordered_map<int, Player*> players;

KeyboardComponent::KeyboardComponent(){}

void KeyboardComponent::update()
{
	Player* redKnight = players[Index::redPlayer];
	Player* blueKnight = players[Index::bluePlayer];

	if (index == Index::redPlayer)
	{
		if (game->event.type == SDL_KEYDOWN)
		{
			switch (game->event.key.keysym.sym)
			{
			case SDLK_w:
				redKnight->transform.dirY = -1;
				break;
			case SDLK_a:
				redKnight->transform.dirX = -1;
				redKnight->sprite.spriteFlip = SDL_FLIP_HORIZONTAL;
				break;
			case SDLK_s:
				redKnight->transform.dirY = 1;
				break;
			case SDLK_d:
				redKnight->transform.dirX = 1;
				redKnight->sprite.spriteFlip = SDL_FLIP_NONE;
				break;
			case SDLK_e:
				if (redKnight->skills.hasOpenChest && !redKnight->skills.hasPower)
				{
					redKnight->skills.hasPower = true;
					redKnight->skills.skillIndex = RandomSkillIndex(1, 5);
					redKnight->skills.skillStartTimer = SDL_GetTicks() - game->pauseTime;
					Mix_PlayChannel(-1, game->powerUpSoundEffect, 0);
				}
				break;
			}
		}

		if (game->event.type == SDL_KEYUP)
		{
			switch (game->event.key.keysym.sym)
			{
			case SDLK_w:
				redKnight->transform.dirY = 0;
				break;
			case SDLK_a:
				redKnight->transform.dirX = 0;
				redKnight->sprite.spriteFlip = SDL_FLIP_NONE;
				break;
			case SDLK_s:
				redKnight->transform.dirY = 0;
				break;
			case SDLK_d:
				redKnight->transform.dirX = 0;
				redKnight->sprite.spriteFlip = SDL_FLIP_NONE;
				break;
			}
		}
	}
	else if (index == Index::bluePlayer)
	{
		if (game->event.type == SDL_KEYDOWN)
		{
			switch (game->event.key.keysym.sym)
			{
			case SDLK_UP:
				blueKnight->transform.dirY = -1;
				break;
			case SDLK_LEFT:
				blueKnight->transform.dirX = -1;
				blueKnight->sprite.spriteFlip = SDL_FLIP_HORIZONTAL;
				break;
			case SDLK_DOWN:
				blueKnight->transform.dirY = 1;
				break;
			case SDLK_RIGHT:
				blueKnight->transform.dirX = 1;
				blueKnight->sprite.spriteFlip = SDL_FLIP_NONE;
				break;
			case SDLK_RSHIFT:
				if (blueKnight->skills.hasOpenChest && !blueKnight->skills.hasPower)
				{
					blueKnight->skills.hasPower = true;
					blueKnight->skills.skillIndex = RandomSkillIndex(1, 5);
					blueKnight->skills.skillStartTimer = SDL_GetTicks() - game->pauseTime;
					Mix_PlayChannel(-1, game->powerUpSoundEffect, 0);
				}
				break;
			}
		}

		if (game->event.type == SDL_KEYUP)
		{
			switch (game->event.key.keysym.sym)
			{
			case SDLK_UP:
				blueKnight->transform.dirY = 0;
				break;
			case SDLK_LEFT:
				blueKnight->transform.dirX = 0;
				blueKnight->sprite.spriteFlip = SDL_FLIP_HORIZONTAL;
				break;
			case SDLK_DOWN:
				blueKnight->transform.dirY = 0;
				break;
			case SDLK_RIGHT:
				blueKnight->transform.dirX = 0;
				blueKnight->sprite.spriteFlip = SDL_FLIP_HORIZONTAL;
				break;
			}
		}
	}
	if (players[index]->transform.dirX != 0 || players[index]->transform.dirY != 0)
		players[index]->sprite.playAnimation("Walk");
	else
		players[index]->sprite.playAnimation("Idle");
}
