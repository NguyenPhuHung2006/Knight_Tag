#pragma once
#include <SDL.h>

enum Index
{
    redPlayer,
    bluePlayer,
    redEntity,
    blueEntity
};

namespace GameConstants 
{
    extern int windowWidth;
    extern int windowHeight;

    namespace TileConstants
    {
        extern int srcWidth;
        extern int srcHeight;

        extern int destWidth;
        extern int destHeight;

        extern int numRowsInPng;
        extern int numColsInTxt;
        extern int numRowsInTxt;
    }

    extern int frameDelayMs;
    extern int maxVelocityPossible;

    namespace RedKnight 
    {
        extern int srcX, srcY, srcW, srcH;
        extern int destX, destY, destW, destH;
        extern int velocity;
        extern bool isAnimated;

        extern int idleIndex, idleNumFrame, idleSpeed;
        extern int walkIndex, walkNumFrame, walkSpeed;

        extern int boundary;

        extern int scoreSizeText;
        extern int hasBedSizeText;

        extern Uint8 r;
        extern Uint8 g;
        extern Uint8 b;

        extern int scoreTextDestX;
        extern int scoreTextDestY;

        extern int hasBedTextDestX;
        extern int hasBedTextDestY;
    }

    namespace RedEntity
    {
        extern int srcX, srcY, srcW, srcH;
        extern int destX, destY, destW, destH;
        extern int velocity;
        extern bool isAnimated;

        extern int idleIndex, idleNumFrame, idleSpeed;
        extern int walkIndex, walkNumFrame, walkSpeed;

    }

    namespace BlueKnight 
    {
        extern int srcX, srcY, srcW, srcH;
        extern int destX, destY, destW, destH;
        extern int velocity;
        extern bool isAnimated;

        extern int idleIndex, idleNumFrame, idleSpeed;
        extern int walkIndex, walkNumFrame, walkSpeed;

        extern int boundary;

        extern int scoreSizeText;
        extern int hasBedSizeText;

        extern Uint8 r;
        extern Uint8 g;
        extern Uint8 b;

        extern int scoreTextDestX;
        extern int scoreTextDestY;

        extern int hasBedTextDestX;
        extern int hasBedTextDestY;

    }

    namespace BlueEntity
    {
        extern int srcX, srcY, srcW, srcH;
        extern int destX, destY, destW, destH;
        extern int velocity;
        extern bool isAnimated;

        extern int idleIndex, idleNumFrame, idleSpeed;
        extern int walkIndex, walkNumFrame, walkSpeed;

    }

    namespace Timer
    {
        extern int size;
        extern Uint8 r;
        extern Uint8 g;
        extern Uint8 b;
        extern int destX;
        extern int destY;

        extern int timeStart;
    }

    namespace Button
    {
        namespace Pause
        {
            extern int srcX, srcY, srcW, srcH;
            extern int destX, destY, destW, destH;
        }
        namespace Resume
        {
            extern int srcX, srcY, srcW, srcH;
            extern int destX, destY, destW, destH;
        }
        namespace Quit
        {
            extern int srcX, srcY, srcW, srcH;
            extern int destX, destY, destW, destH;
        }
    }

    namespace Gameplay 
    {
        extern int skillDuration;
        extern int superVelocity;
        extern int slowedVelocity;
        extern int reviveDuration;
        extern int fadeOutDuration;
        extern int fadeInDuration;
    }

    namespace Paths 
    {
        extern const char* redKnightTexture;
        extern const char* blueKnightTexture;

        extern const char* redEntityTexture;
        extern const char* blueEntityTexture;

        extern const char* mapBackgroundTxt;
        extern const char* mapCollidersTxt;
        extern const char* mapTileTexture;

        extern const char* openChestSound;
        extern const char* powerUpSound;

        extern const char* mainFont;
        extern const char* iconTexture;
        extern const char* cursorTexture;

        extern const char* hasBedSound;
        extern const char* returnBedSound;
        extern const char* backGroundMusic;

        extern const char* pauseTexture;
        extern const char* resumeTex;
        extern const char* quitTex;
    }

    namespace Environment 
    {
        namespace Chest 
        {
            extern int closedSrcX;
            extern int closedSrcY;
            extern int openSrcX;
            extern int openSrcY;
        }

        namespace Bed
        {
            extern int redSrcX, redSrcY, redDestX, redDestY;
            extern int blueSrcX, blueSrcY, blueDestX, blueDestY;
        }
    }
}

