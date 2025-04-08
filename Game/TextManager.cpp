#include "GameConstant.h"
#include "Player.h"
using namespace std;

extern Game* game;

void TextManager::init(const char* fontPath, int size, Uint8 r, Uint8 g, Uint8 b, int destRectX, int destRectY, const char* text)
{
	TTF_Init();

	font = TTF_OpenFont(GameConstants::Paths::mainFont, size);
	textColor = { r,g,b };

	textSurface = TTF_RenderText_Blended(font, text, textColor);
	textTexture = SDL_CreateTextureFromSurface(game->renderer,textSurface);
	SDL_FreeSurface(textSurface);

	textDestRect.x = destRectX;
	textDestRect.y = destRectY;

	SDL_QueryTexture(textTexture, NULL, NULL, &textDestRect.w, &textDestRect.h);
	textSrcRect = { 0,0,textDestRect.w,textDestRect.h };
}

void TextManager::updateText(int text)
{
	SDL_DestroyTexture(textTexture);
	textSurface = TTF_RenderText_Blended(font, to_string(text).c_str(), textColor);
	textTexture = SDL_CreateTextureFromSurface(game->renderer, textSurface);
	SDL_FreeSurface(textSurface);

	SDL_QueryTexture(textTexture, NULL, NULL, &textDestRect.w, &textDestRect.h);
	textSrcRect = { 0,0,textDestRect.w,textDestRect.h };
}

void TextManager::drawText()
{
	TextureManager::Draw(textTexture, textSrcRect, textDestRect);
}