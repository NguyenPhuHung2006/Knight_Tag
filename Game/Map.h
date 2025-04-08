#pragma once

#include <vector>
#include <memory>
#include <SDL.h>
#include "TextureManager.h"
#include "GameConstant.h"
using namespace std;

struct Tile
{
	SDL_Rect srcRect;
	SDL_Rect destRect;
	SDL_Texture* tileTex;

	int collideBy = -1;

	bool isChest;

	Tile(const char* pathPNG, int srcX, int srcY, int srcW, int srcH
		,int destX, int destY, int destW, int destH);
};

struct Map
{
	vector<unique_ptr<Tile>> backGroundTiles;
	vector<unique_ptr<Tile>> colliderTiles;
	vector<vector<bool>> colliderBit;
	Map();

	void loadMap(const char* pathTXT, int srcW, int srcH, int numTileInRow
		, int numTileX, int numTileY, int destW, int destH);

	void drawMap();

	void addTile(const char* pathTXT, int srcX, int srcY, int srcW, int srcH
		, int destX, int destY, int destW, int destH);
};
