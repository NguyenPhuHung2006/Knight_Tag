#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

struct TextManager
{
	TTF_Font* font;
	SDL_Color textColor;
	SDL_Surface* textSurface;
	SDL_Texture* textTexture;
	SDL_Rect textSrcRect;
	SDL_Rect textDestRect;

	void init(const char* fontPath, int size, Uint8 r, Uint8 g, Uint8 b, int destRectX, int destRectY, const char* text);
	void updateText(int text);
	void drawText();
};
