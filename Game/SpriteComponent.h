#pragma once

#include "TransformComponent.h"
#include "TextureManager.h"
#include <unordered_map>

struct Animation
{
	int index, numFrame, frameSpeed;

	Animation() = default;

	Animation(int i, int n, int s)
	{
		index = i;
		numFrame = n;
		frameSpeed = s;
	}
};

class SpriteComponent
{
private:
	SDL_Texture* playerTex;
	Animation idleAnimation, walkAnimation;

	bool animated;
	int animationIndex, numAnimationFrame, animationSpeed;

public:
	unordered_map<const char*, Animation> animations;
	
	SDL_RendererFlip spriteFlip;

	SDL_Rect srcRect;

	int index;
	bool fadeIn, fadeOut;
	bool isInvisible;
	bool hasSuperSpeed;
	bool isSlowed, isFreezed;
	bool isInvincible;

	Uint32 fadeInStartTimer, fadeInEndTimer;
	Uint32 fadeOutStartTimer, fadeOutEndTimer;

	SpriteComponent();
	~SpriteComponent();

	void init(const char* pathPNG, bool isAnimated, int srcX, int srcY, int srcW, int srcH);
	void update();
	void draw();
	void playAnimation(const char* animationName);
};
