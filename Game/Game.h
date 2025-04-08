#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "UIManager.h"

class Game
{
private:
	void setUpSound();
	void setUpMap();
	void setUpPlayer();
	void setUpEntity();

public:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

	Mix_Chunk* openChestSoundEffect;
	Mix_Chunk* powerUpSoundEffect;
	Mix_Chunk* hasBedSoundEffect;
	Mix_Chunk* returnBedSoundEffect;

	Mix_Music* backGroundTheme;

	UIManager uiManager;
	Uint32 pauseTime = 0;

	bool isRunning;

	Game();
	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void update();
	void render();
	void clean();
};
