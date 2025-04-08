#include "TextureManager.h"
#include "Game.h"

extern Game* game;

SDL_Texture* TextureManager::LoadTexture(const char* path)
{
	SDL_Surface* surface = IMG_Load(path);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest)
{
	SDL_RenderCopy(game->renderer, tex, &src, &dest);
}

void TextureManager::DrawEx(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip)
{
	SDL_RenderCopyEx(game->renderer, tex, &src, &dest, 0.0, NULL, flip);
}
