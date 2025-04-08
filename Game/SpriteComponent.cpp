#include "SpriteComponent.h"
#include "GameConstant.h"
#include "Player.h"
#include "Entity.h"

extern Game* game;
extern unordered_map<int, Player*> players;
extern unordered_map<int, Entity*> entities;

using namespace GameConstants;

SpriteComponent::SpriteComponent() {}

SpriteComponent::~SpriteComponent()
{
	SDL_DestroyTexture(playerTex);
}

void SpriteComponent::init(const char* pathPNG, bool isAnimated, int srcX, int srcY, int srcW, int srcH)
{
	playerTex = TextureManager::LoadTexture(pathPNG);

	animated = isAnimated;

	if (index == Index::redPlayer)
	{
		idleAnimation = Animation(RedKnight::idleIndex, RedKnight::idleNumFrame, RedKnight::idleSpeed);
		walkAnimation = Animation(RedKnight::walkIndex, RedKnight::walkNumFrame, RedKnight::walkSpeed);
	}
	else if (index == Index::bluePlayer)
	{
		idleAnimation = Animation(BlueKnight::idleIndex, BlueKnight::idleNumFrame, BlueKnight::idleSpeed);
		walkAnimation = Animation(BlueKnight::walkIndex, BlueKnight::walkNumFrame, BlueKnight::walkSpeed);
	}
	else if (index == Index::redEntity)
	{
		idleAnimation = Animation(RedEntity::idleIndex, RedEntity::idleNumFrame, RedEntity::idleSpeed);
		walkAnimation = Animation(RedEntity::walkIndex, RedEntity::walkNumFrame, RedEntity::walkSpeed);
	}
	else if (index == Index::blueEntity)
	{
		idleAnimation = Animation(BlueEntity::idleIndex, BlueEntity::idleNumFrame, BlueEntity::idleSpeed);
		walkAnimation = Animation(BlueEntity::walkIndex, BlueEntity::walkNumFrame, BlueEntity::walkSpeed);
	}

	animations["Idle"] = idleAnimation;
	animations["Walk"] = walkAnimation;

	playAnimation("Idle");

	srcRect.x = srcX;
	srcRect.y = srcY;
	srcRect.w = srcW;
	srcRect.h = srcH;

	if (index == Index::redPlayer || index == Index::redEntity)
		spriteFlip = SDL_FLIP_NONE;
	else if (index == Index::bluePlayer || index == Index::blueEntity)
		spriteFlip = SDL_FLIP_HORIZONTAL;

	fadeIn = false;
	fadeOut = false;
	isInvisible = false;
	hasSuperSpeed = false;
	isSlowed = isFreezed = isInvincible = false;
}

void SpriteComponent::update()
{
	if (animated)
	{
		srcRect.x = srcRect.w * static_cast<int>(((SDL_GetTicks() - game->pauseTime) / animationSpeed) % numAnimationFrame);
		srcRect.y = srcRect.h * animationIndex;
	}
}

void SpriteComponent::draw()
{
	Uint8 alpha = 255;
	// this will be used only for the players with index of 0 and 1
	if (index <= 1)
	{
		if (fadeOut)
		{
			SDL_SetTextureBlendMode(playerTex, SDL_BLENDMODE_BLEND);

			fadeOutEndTimer = SDL_GetTicks();
			Uint32 fadeOutTimer = fadeOutEndTimer - fadeOutStartTimer;

			float progress = static_cast<float>(fadeOutTimer) / Gameplay::fadeOutDuration;
			// this variable will make sure that the player will fade in slowly

			if (progress > 1.0f)
				progress = 1.0f;

			alpha = static_cast<Uint8>(255 * (1 - progress));

			if (progress >= 1.0f)
			{
				fadeOut = false;
				isInvisible = true;
			}

		}
		if (fadeIn)
		{
			SDL_SetTextureBlendMode(playerTex, SDL_BLENDMODE_BLEND);

			fadeInEndTimer = SDL_GetTicks();
			Uint32 fadeInTimer = fadeInEndTimer - fadeInStartTimer;

			float progress = static_cast<float>(fadeInTimer) / Gameplay::fadeInDuration;

			if (progress > 1.0f)
				progress = 1.0f;

			alpha = static_cast<Uint8>(255 * progress);

			if (progress >= 1.0f)
			{
				fadeIn = false;
				isInvisible = false;
			}
		}

		// blue tinted = 100,100,255;
		// red tinted = 255,100,100;
		// yellow tinted = 255,255,100

		if (hasSuperSpeed)
		{
			if (index == Index::redPlayer)
				SDL_SetTextureColorMod(playerTex, 255, 100, 100);
			else if (index == Index::bluePlayer)
				SDL_SetTextureColorMod(playerTex, 100, 100, 255);
		}

		else if (isSlowed || isFreezed)
		{
			if (index == Index::redPlayer)
				SDL_SetTextureColorMod(playerTex, 100, 100, 255);
			else if (index == Index::bluePlayer)
				SDL_SetTextureColorMod(playerTex, 255, 100, 100);
		}

		else if (isInvincible)
			SDL_SetTextureColorMod(playerTex, 255, 255, 100);

		else
			SDL_SetTextureColorMod(playerTex, 255, 255, 255);
	}
	
	SDL_SetTextureAlphaMod(playerTex, alpha);

	if (!isInvisible)
	{
		SDL_Rect destRect = {};
		if (index <= 1)
			destRect = players[index]->transform.destRect;
		else if (index >= 2 && index <= 3)
			destRect = entities[index]->transform.destRect;
		TextureManager::DrawEx(playerTex, srcRect, destRect, spriteFlip);
	}
}

void SpriteComponent::playAnimation(const char* animationName)
{
	animationIndex = animations[animationName].index;
	numAnimationFrame = animations[animationName].numFrame;
	animationSpeed = animations[animationName].frameSpeed;
}
