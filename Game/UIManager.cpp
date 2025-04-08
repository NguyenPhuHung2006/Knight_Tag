#include "UIManager.h"
#include "GameConstant.h"
#include "Game.h"
#include "TextureManager.h"
#include <string>
using namespace std;

extern Game* game;

UIManager::UIManager() = default;

void UIManager::init()
{
	isPaused = false;
	setUpCursor();
	setUpIcon();
	setUpTimer();
	setUpButton();
}

void UIManager::setUpCursor()
{
	SDL_Surface* swordCursorSurface = IMG_Load(GameConstants::Paths::cursorTexture);
	swordCursor = SDL_CreateColorCursor(swordCursorSurface, 49, 0);
	SDL_FreeSurface(swordCursorSurface);
	SDL_SetCursor(swordCursor);
}

void UIManager::setUpIcon()
{
	SDL_Surface* icon = IMG_Load(GameConstants::Paths::iconTexture);
	if (icon)
	{
		SDL_SetWindowIcon(game->window, icon);
		SDL_FreeSurface(icon);
	}
}

void UIManager::setUpTimer()
{
	timer.init(GameConstants::Paths::mainFont,
		GameConstants::Timer::size,
		GameConstants::Timer::r, GameConstants::Timer::g, GameConstants::Timer::b,
		GameConstants::Timer::destX,
		GameConstants::Timer::destY,
		to_string(GameConstants::Timer::timeStart).c_str()
	);
}

void UIManager::setUpButton()
{
	pauseTex = TextureManager::LoadTexture(GameConstants::Paths::pauseTexture);
	resumeTex = TextureManager::LoadTexture(GameConstants::Paths::resumeTex);
	quitTex = TextureManager::LoadTexture(GameConstants::Paths::quitTex);

	SDL_QueryTexture(pauseTex, nullptr, nullptr, &GameConstants::Button::Pause::srcW, &GameConstants::Button::Pause::srcH);
	SDL_QueryTexture(resumeTex, nullptr, nullptr, &GameConstants::Button::Resume::srcW, &GameConstants::Button::Resume::srcH);
	SDL_QueryTexture(quitTex, nullptr, nullptr, &GameConstants::Button::Quit::srcW, &GameConstants::Button::Quit::srcH);

	// i will use query because i want the srcw and srch will be relative to the actual width and height of the texture

	pauseSrcRect = {
		GameConstants::Button::Pause::srcX, GameConstants::Button::Pause::srcY,
		GameConstants::Button::Pause::srcW, GameConstants::Button::Pause::srcH
	};
	pauseDestRect = {
		GameConstants::Button::Pause::destX, GameConstants::Button::Pause::destY,
		GameConstants::Button::Pause::destW, GameConstants::Button::Pause::destH
	};

	resumeSrcRect = {
		GameConstants::Button::Resume::srcX, GameConstants::Button::Resume::srcY,
		GameConstants::Button::Resume::srcW, GameConstants::Button::Resume::srcH
	};
	resumeDestRect = {
		GameConstants::Button::Resume::destX, GameConstants::Button::Resume::destY,
		GameConstants::Button::Resume::destW, GameConstants::Button::Resume::destH
	};

	quitSrcRect = {
		GameConstants::Button::Quit::srcX, GameConstants::Button::Quit::srcY,
		GameConstants::Button::Quit::srcW, GameConstants::Button::Quit::srcH
	};
	quitDestRect = {
		GameConstants::Button::Quit::destX, GameConstants::Button::Quit::destY,
		GameConstants::Button::Quit::destW, GameConstants::Button::Quit::destH
	};
}

void UIManager::update()
{
	timer.updateText((int)((SDL_GetTicks() / 1000) - (game->pauseTime / 1000)));
}

void UIManager::draw()
{
	timer.drawText();
	if(!isPaused)
		TextureManager::Draw(pauseTex, pauseSrcRect, pauseDestRect);
	else
	{
		TextureManager::Draw(resumeTex, resumeSrcRect, resumeDestRect);
		TextureManager::Draw(quitTex, quitSrcRect, quitDestRect);
	}
}

void UIManager::checkPause()
{
	int mouseX = game->event.button.x;
	int mouseY = game->event.button.y;

	bool detectPause = false, detectResume = false, detectQuit = false;

	if (!isPaused)
	{
		detectPause = (mouseX > GameConstants::Button::Pause::destX
			&& mouseX < GameConstants::Button::Pause::destX + GameConstants::Button::Pause::destW
			&& mouseY > GameConstants::Button::Pause::destY
			&& mouseY < GameConstants::Button::Pause::destY + GameConstants::Button::Pause::destW);
	}

	else
	{
		detectResume = (mouseX > GameConstants::Button::Resume::destX
			&& mouseX < GameConstants::Button::Resume::destX + GameConstants::Button::Resume::destW
			&& mouseY > GameConstants::Button::Resume::destY
			&& mouseY < GameConstants::Button::Resume::destY + GameConstants::Button::Resume::destW);

		detectQuit = (mouseX > GameConstants::Button::Quit::destX
			&& mouseX < GameConstants::Button::Quit::destX + GameConstants::Button::Quit::destW
			&& mouseY > GameConstants::Button::Quit::destY
			&& mouseY < GameConstants::Button::Quit::destY + GameConstants::Button::Quit::destW);
	}

	switch (game->event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		if (detectPause && !isPaused)
		{
			isPaused = true;
			pauseStartTimer = SDL_GetTicks();
		}
		else if (detectResume && isPaused)
		{
			isPaused = false;
			pauseEndTimer = SDL_GetTicks();
			game->pauseTime += (pauseEndTimer - pauseStartTimer);
		}
		else if (detectQuit && isPaused)
			game->isRunning = false;
		break;
	}

}

