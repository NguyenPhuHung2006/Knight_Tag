#pragma once
#include <SDL.h>
#include "TextManager.h"

class UIManager
{
private:
	void setUpIcon();
	void setUpCursor();
	void setUpTimer();
	void setUpButton();

public:
	UIManager();

	bool isPaused;

	Uint32 pauseStartTimer = 0, pauseEndTimer = 0;
	SDL_Texture* pauseTex = nullptr;
	SDL_Rect pauseSrcRect, pauseDestRect;

	SDL_Texture* resumeTex = nullptr;
	SDL_Rect resumeSrcRect, resumeDestRect;

	SDL_Texture* quitTex = nullptr;
	SDL_Rect quitSrcRect, quitDestRect;

	SDL_Cursor* swordCursor;

	TextManager timer;

	void init();
	void update();
	void draw();
	void checkPause();
};
