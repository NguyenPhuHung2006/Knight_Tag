#include "GameConstant.h"
#include <SDL.h>

// define constants of the game

namespace GameConstants 
{
    int windowWidth = 2250;
    int windowHeight = 1500;

    namespace TileConstants
    {
        int srcWidth = 16, srcHeight = 16;
        int numRowsInPng = 8;
        int numColsInTxt = 30, numRowsInTxt = 20;
        int destWidth = windowWidth / numColsInTxt;
        int destHeight = windowHeight / numRowsInTxt;
    }

    int frameDelayMs = 16;

    int maxVelocityPossible = 35;

    namespace RedKnight 
    {
        int srcX = 0, srcY = 0, srcW = 13, srcH = 19;
        int destX = 2 * TileConstants::destWidth, destY = 10 * TileConstants::destHeight;
        int destW = 39, destH = 57;
        int velocity = 10;
        bool isAnimated = true;

        int idleIndex = 0, idleNumFrame = 4, idleSpeed = 200;
        int walkIndex = 1, walkNumFrame = 7, walkSpeed = 200;

        int boundary = 15 * TileConstants::destWidth;

        int scoreSizeText = 50;
        int hasBedSizeText = 50;

        Uint8 r = 255;
        Uint8 g = 0;
        Uint8 b = 0;

        int scoreTextDestX = 0;
        int scoreTextDestY = 0;

        int hasBedTextDestX = 30 + TileConstants::destWidth;
        int hasBedTextDestY = 0;

    }

    namespace RedEntity
    {
        int srcX = 0, srcY = 0, srcW = 11, srcH = 21;
        int destX = 8 * TileConstants::destWidth, destY = 10 * TileConstants::destHeight;
        int destW = 33, destH = 63;
        int velocity = 5;
        bool isAnimated = true;

        int idleIndex = 0, idleNumFrame = 4, idleSpeed = 200;
        int walkIndex = 1, walkNumFrame = 3, walkSpeed = 200;

    }

    namespace BlueKnight 
    {
        int srcX = 0, srcY = 0, srcW = 15, srcH = 20;
        int destW = 45, destH = 60;
        int destX = 28 * TileConstants::destWidth - destW, destY = 10 * TileConstants::destHeight;
        int velocity = 10;
        bool isAnimated = true;

        int idleIndex = 0, idleNumFrame = 4, idleSpeed = 200;
        int walkIndex = 1, walkNumFrame = 4, walkSpeed = 200;

        int boundary = 16 * TileConstants::destWidth;

        int scoreSizeText = 50;
        int hasBedSizeText = 50;

        Uint8 r = 0;
        Uint8 g = 0;
        Uint8 b = 255;

        int scoreTextDestX = 29 * TileConstants::destWidth;
        int scoreTextDestY = 0;

        int hasBedTextDestX = 2175 - 210 - TileConstants::destWidth;
        int hasBedTextDestY = 0;
    }

    namespace BlueEntity
    {
        int srcX = 0, srcY = 0, srcW = 16, srcH = 16;
        int destX = 21 * TileConstants::destWidth, destY = 10 * TileConstants::destHeight;
        int destW = 48, destH = 48;
        int velocity = 5;
        bool isAnimated = true;

        int idleIndex = 0, idleNumFrame = 4, idleSpeed = 200;
        int walkIndex = 1, walkNumFrame = 4, walkSpeed = 200;

    }

    namespace Timer
    {
        int size = 50;
        Uint8 r = 0;
        Uint8 g = 0;
        Uint8 b = 0;
        int destX = windowWidth / 2 - 15;
        int destY = 0;

        int timeStart = 0;
    }

    namespace Button
    {
        namespace Pause
        {
            int srcX = 0, srcY = 0, srcW = 0, srcH = 0;
            int destX = 15 * TileConstants::destWidth;
            int destY = 19 * TileConstants::destHeight;
            int destW = TileConstants::destWidth; 
            int destH = TileConstants::destHeight;
        }
        namespace Resume
        {
            int srcX = 0, srcY = 0, srcW = 0, srcH = 0;
            int destX = 12 * TileConstants::destWidth;
            int destY = 9 * TileConstants::destHeight;
            int destW = 2 * TileConstants::destWidth;
            int destH = 2 * TileConstants::destHeight;
        }
        namespace Quit
        {
            int srcX = 0, srcY = 0, srcW = 0, srcH = 0;
            int destX = 17 * TileConstants::destWidth;
            int destY = 9 * TileConstants::destHeight;
            int destW = 2 * TileConstants::destWidth;
            int destH = 2 * TileConstants::destHeight;
        }
    }

    namespace Gameplay 
    {
        int skillDuration = 5000;
        int superVelocity = 30;
        int slowedVelocity = 1;
        int reviveDuration = 3000;
        int fadeOutDuration = 500;
        int fadeInDuration = 1500;
    }

    namespace Paths 
    {
        const char* redKnightTexture = "assets/red_knight.png";
        const char* blueKnightTexture = "assets/blue_knight.png";

        const char* redEntityTexture = "assets/red_entity.png";
        const char* blueEntityTexture = "assets/blue_entity.png";

        const char* mapBackgroundTxt = "assets/tile_layer1.txt";
        const char* mapCollidersTxt = "assets/tile_layer2.txt";
        const char* mapTileTexture = "assets/tileset.png";

        const char* openChestSound = "assets/chest_open.wav";
        const char* powerUpSound = "assets/power_up.wav";
        const char* backGroundMusic = "assets/background_theme.mp3";

        const char* mainFont = "assets/JetBrainsMono_Bold.ttf";
        const char* iconTexture = "assets/window_icon.png";
        const char* cursorTexture = "assets/sword_cursor.png";

        const char* hasBedSound = "assets/has_bed.wav";
        const char* returnBedSound = "assets/return_bed.wav";

        const char* pauseTexture = "assets/pause_icon.png";
        const char* resumeTex = "assets/resume_icon.png";
        const char* quitTex = "assets/quit_icon.png";
    }

    namespace Environment 
    {
        namespace Chest 
        {
            int closedSrcX = 112, closedSrcY = 384;
            int openSrcX = 0, openSrcY = 400;
        }

        namespace Bed 
        {
            int blueSrcX = 0, blueSrcY = 368;
            int blueDestX = 28 * TileConstants::destWidth, blueDestY = 10 * TileConstants::destHeight;

            int redSrcX = 16, redSrcY = 368;
            int redDestX = 1 * TileConstants::destWidth, redDestY = 10 * TileConstants::destHeight;
        }
    }
}
