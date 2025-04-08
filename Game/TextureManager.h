#pragma once

#include "Game.h"

struct TextureManager
{
	// i will use static for this class because i don't want to make an object and use extern everywhere
	static SDL_Texture* LoadTexture(const char* path);
	static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);
	static void DrawEx(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip);
};
