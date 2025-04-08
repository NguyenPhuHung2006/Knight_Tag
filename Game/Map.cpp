#include "Map.h"
#include "TextureManager.h"
#include "GameConstant.h"
#include <fstream>
using namespace std;

Tile::Tile(const char* pathPNG, int srcX, int srcY, int srcW, int srcH, int destX, int destY, int destW, int destH)
{
    tileTex = TextureManager::LoadTexture(pathPNG);

    srcRect = { srcX, srcY, srcW, srcH };
    destRect = { destX, destY, destW, destH };

    if ((srcX == GameConstants::Environment::Chest::closedSrcX)
        && (srcY == GameConstants::Environment::Chest::closedSrcY)) // check if the tile is a chest or not
    {
        isChest = true;
    }
    else
        isChest = false;
}

Map::Map() = default;

void Map::addTile(const char* pathTXT, int srcX, int srcY, int srcW, int srcH, int destX, int destY, int destW, int destH)
{
    unique_ptr<Tile> tile = make_unique<Tile>(GameConstants::Paths::mapTileTexture, srcX, srcY, srcW, srcH, destX, destY, destW, destH);

    if (string(pathTXT) == GameConstants::Paths::mapBackgroundTxt)
        backGroundTiles.emplace_back(move(tile));
    else if (string(pathTXT) == GameConstants::Paths::mapCollidersTxt)
        colliderTiles.emplace_back(move(tile));
}

void Map::loadMap(const char* pathTXT, int srcW, int srcH, int numTileInRow, int numTileX, int numTileY, int destW, int destH)
{
    // this function will extract all of the tile index from the txt file and put it to a vector of tile

    if(string(pathTXT) == GameConstants::Paths::mapCollidersTxt)
        colliderBit.assign(numTileX,vector<bool>(numTileY, false));

    ifstream fin(pathTXT);

    int tilePos;

    for (int y = 0; y < numTileY; y++)
    {
        for (int x = 0; x < numTileX; x++)
        {
            fin >> tilePos;
            if (tilePos > 0)
            {
                addTile(pathTXT, ((tilePos - 1) % numTileInRow) * srcW,
                    ((tilePos - 1) / numTileInRow) * srcH,
                    srcW, srcH, x * destW, y * destH, destW, destH);

                if (string(pathTXT) == GameConstants::Paths::mapCollidersTxt)
                {
                    colliderBit[x][y] = true;
                }
            }
            fin.ignore();
        }
    }
}

void Map::drawMap()
{
    for (const unique_ptr<Tile>& backGroundTile : backGroundTiles)
    {
        TextureManager::Draw(backGroundTile->tileTex, backGroundTile->srcRect, backGroundTile->destRect);
    }

    for (const unique_ptr<Tile>& colliderTile : colliderTiles)
    {
        TextureManager::Draw(colliderTile->tileTex, colliderTile->srcRect, colliderTile->destRect);
    }
}
