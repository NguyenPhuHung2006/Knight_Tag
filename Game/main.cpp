#include "Game.h"
#include "GameConstant.h"

// the part that have algorithm
// 1. checkWall() function in ColliderComponent.cpp line 44 -> if the player collides with the collider, shifts it back
// 2. hasLineOnSight() function in Entity.cpp in line 40 -> using bresenham algorithm
// 3. setPathToTarget() function in Entity.cpp in line 222 -> using A* algorithm
// 4. loadMap() function in Map.cpp in line 53 (the addTile part) -> using some module operator to extract the exact tile
// 5. draw() function in SpriteComponent in line 81 (the progress part) -> the progress will change from 0 to 1
// 6. update() function in TransformComponent in line 27 -> using physic to make the player velocity equals to the actual velocity when it moves diagonally

Game* game = new Game();

int main(int argc, char* argv[])
{
    game->init("Knight Tag", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        GameConstants::windowWidth, GameConstants::windowHeight, false);

    while (game->isRunning) 
    {
        Uint32 frameStart = SDL_GetTicks();

        game->uiManager.checkPause(); 

        game->update(); 

        game->render(); 

        Uint32 frameTime = SDL_GetTicks() - frameStart;

        if (frameTime < GameConstants::frameDelayMs)
            SDL_Delay(GameConstants::frameDelayMs - frameTime);
        // this code makes sure that every frames took at least "frameDelayMs" to complete
    }
    game->clean();
    return 0;
}
