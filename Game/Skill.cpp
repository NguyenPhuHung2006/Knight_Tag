#include "Skill.h"
#include "GameConstant.h"
#include "Player.h"

extern unordered_map<int, Player*> players;
extern Game* game;

Skill::Skill() = default;

void Skill::init(int super_vel, int slowed_vel, int skill_duration)
{
	this->super_vel = super_vel;
	this->slowed_vel = slowed_vel;
	skillDuration = skill_duration;

	skillIndex = 0;
	superSpeedIndex = 1;
	invisibleIndex = 2;
	slowedIndex = 3;
	freezeIndex = 4;
	invincibleIndex = 5;

	skillStartTimer = skillEndTimer = 0;

	hasOpenChest = hasPower = false;
}

void Skill::update()
{
	if (hasPower)
	{
		skillEndTimer = SDL_GetTicks();
		int skillTimer = skillEndTimer - skillStartTimer - game->pauseTime;

		// super speed
		if (skillIndex == superSpeedIndex)
		{
			if (skillTimer <= skillDuration)
			{
				if(players[index]->sprite.isSlowed)
					players[index]->transform.vel = super_vel - 15 * slowed_vel;
				else if (players[index]->sprite.isFreezed)
					players[index]->transform.vel = super_vel - 25 * slowed_vel;
				else 
					players[index]->transform.vel = super_vel;

				players[index]->sprite.hasSuperSpeed = true;
			}
			else
			{
				if (index == Index::redPlayer)
					players[index]->transform.vel = GameConstants::RedKnight::velocity;
				if (index == Index::bluePlayer)
					players[index]->transform.vel = GameConstants::BlueKnight::velocity;

				players[index]->sprite.hasSuperSpeed = false;

				reset();
			}
		}

		// invisible
		else if (skillIndex == invisibleIndex)
		{
			if (skillTimer <= skillDuration)
			{
				if (!players[index]->sprite.fadeOut)
				{
					players[index]->sprite.fadeOut = true;
					players[index]->sprite.fadeOutStartTimer = SDL_GetTicks();
				}
			}
			else
			{
				if (!players[index]->sprite.fadeIn)
				{
					players[index]->sprite.fadeOut = false;
					players[index]->sprite.fadeIn = true;
					players[index]->sprite.isInvisible = false;
					players[index]->sprite.fadeInStartTimer = SDL_GetTicks();
				}

				reset();
			}
		}

		// slowed
		else if (skillIndex == slowedIndex)
		{
			if (skillTimer <= skillDuration)
			{
				if (players[index ^ 1]->sprite.hasSuperSpeed)
					players[index ^ 1]->transform.vel = super_vel - 15 * slowed_vel; 
				else
					players[index ^ 1]->transform.vel = slowed_vel;

				players[index ^ 1]->sprite.isSlowed = true;
			}

			else
			{
				players[index ^ 1]->transform.vel = (index ^ 1 ? GameConstants::BlueKnight::velocity
															   : GameConstants::RedKnight::velocity);
				players[index ^ 1]->sprite.isSlowed = false;

				reset();
			}
		}

		// freeze
		else if (skillIndex == freezeIndex)
		{
			if (skillTimer <= skillDuration)
			{
				if (players[index ^ 1]->sprite.hasSuperSpeed)
					players[index ^ 1]->transform.vel = super_vel - 25 * slowed_vel;
				else
					players[index ^ 1]->transform.vel = 0;

				players[index ^ 1]->sprite.isFreezed = true;
			}

			else
			{
				players[index ^ 1]->transform.vel = (index ^ 1 ? GameConstants::BlueKnight::velocity 
															   : GameConstants::RedKnight::velocity);
				players[index ^ 1]->sprite.isFreezed = false;

				reset();
			}
		}

		// invincible
		else if (skillIndex == invincibleIndex)
		{
			if (skillTimer <= skillDuration)
				players[index]->sprite.isInvincible = true;
			else
			{
				players[index]->sprite.isInvincible = false;
				reset();
			}
		}
		else
			reset();
	}
}

void Skill::reset()
{
	hasPower = false;
	hasOpenChest = false;
	skillIndex = 0;
}
