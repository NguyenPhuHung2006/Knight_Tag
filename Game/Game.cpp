#include "Game.h"
#include "Map.h"
#include "Player.h"
#include "Entity.h"
#include "GameConstant.h"
#include <unordered_map>
#include <string>
using namespace std;

// declare these as global to use extern
Map* map = nullptr;
unordered_map<int, Player*> players;
unordered_map<int, Entity*> entities;

Game::Game() = default;

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = (fullscreen ? SDL_WINDOW_FULLSCREEN : 0);

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		isRunning = true;

		uiManager.init(); 

		setUpSound();
		setUpMap();
		setUpPlayer();
		setUpEntity();
	}
	else
		isRunning = false;
}

void Game::setUpSound()
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	openChestSoundEffect = Mix_LoadWAV(GameConstants::Paths::openChestSound);
	powerUpSoundEffect = Mix_LoadWAV(GameConstants::Paths::powerUpSound);
	hasBedSoundEffect = Mix_LoadWAV(GameConstants::Paths::hasBedSound);
	returnBedSoundEffect = Mix_LoadWAV(GameConstants::Paths::returnBedSound);

	backGroundTheme = Mix_LoadMUS(GameConstants::Paths::backGroundMusic);

	Mix_PlayMusic(backGroundTheme, -1);
	Mix_VolumeMusic(30); // set the theme volume range to 30%
}

void Game::setUpMap()
{
	map = new Map();

	// there are 2 types of tiles, background (floor) and (collider) which is collidable or not

	map->loadMap(GameConstants::Paths::mapBackgroundTxt,
		GameConstants::TileConstants::srcWidth,
		GameConstants::TileConstants::srcHeight,
		GameConstants::TileConstants::numRowsInPng,
		GameConstants::TileConstants::numColsInTxt,
		GameConstants::TileConstants::numRowsInTxt,
		GameConstants::TileConstants::destWidth,
		GameConstants::TileConstants::destHeight
	);

	map->loadMap(GameConstants::Paths::mapCollidersTxt,
		GameConstants::TileConstants::srcWidth,
		GameConstants::TileConstants::srcHeight,
		GameConstants::TileConstants::numRowsInPng,
		GameConstants::TileConstants::numColsInTxt,
		GameConstants::TileConstants::numRowsInTxt,
		GameConstants::TileConstants::destWidth,
		GameConstants::TileConstants::destHeight
	);
}

void Game::setUpPlayer()
{
	// the index of the red knight is 0 and the blue knight is 1 to make this code less complicated
	Player* redKnight = new Player(Index::redPlayer);
	Player* blueKnight = new Player(Index::bluePlayer);

	// the transform component represents the destRect and the movement of the knight
	redKnight->transform.init(
		GameConstants::RedKnight::destX,
		GameConstants::RedKnight::destY,
		GameConstants::RedKnight::destW,
		GameConstants::RedKnight::destH,
		GameConstants::RedKnight::velocity
	);

	// this represents the texture, srcRect, animation and effects of the player
	redKnight->sprite.init(
		GameConstants::Paths::redKnightTexture,
		GameConstants::RedKnight::isAnimated,
		GameConstants::RedKnight::srcX,
		GameConstants::RedKnight::srcY,
		GameConstants::RedKnight::srcW,
		GameConstants::RedKnight::srcH
	);

	// init the skills, which are superspeed, invisible, invincible, slowed, and freeze
	redKnight->skills.init(
		GameConstants::Gameplay::superVelocity,
		GameConstants::Gameplay::slowedVelocity,
		GameConstants::Gameplay::skillDuration
	);

	// init the score text which show the score of the player at the top of the screen
	redKnight->scoreText.init(
		GameConstants::Paths::mainFont,
		GameConstants::RedKnight::scoreSizeText,
		GameConstants::RedKnight::r, GameConstants::RedKnight::g, GameConstants::RedKnight::b,
		GameConstants::RedKnight::scoreTextDestX,
		GameConstants::RedKnight::scoreTextDestY,
		to_string(redKnight->score).c_str()
	);

	// same to the score text but the text will only appear on the screen if you have taken the opponent's bed
	redKnight->hasBedText.init(
		GameConstants::Paths::mainFont,
		GameConstants::RedKnight::scoreSizeText,
		GameConstants::RedKnight::r, GameConstants::RedKnight::g, GameConstants::RedKnight::b,
		GameConstants::RedKnight::hasBedTextDestX,
		GameConstants::RedKnight::hasBedTextDestY,
		"HAS BED"
	);

	// same to the red knight
	blueKnight->transform.init(
		GameConstants::BlueKnight::destX,
		GameConstants::BlueKnight::destY,
		GameConstants::BlueKnight::destW,
		GameConstants::BlueKnight::destH,
		GameConstants::BlueKnight::velocity
	);

	blueKnight->sprite.init(
		GameConstants::Paths::blueKnightTexture,
		GameConstants::BlueKnight::isAnimated,
		GameConstants::BlueKnight::srcX,
		GameConstants::BlueKnight::srcY,
		GameConstants::BlueKnight::srcW,
		GameConstants::BlueKnight::srcH
	);

	blueKnight->skills.init(
		GameConstants::Gameplay::superVelocity,
		GameConstants::Gameplay::slowedVelocity,
		GameConstants::Gameplay::skillDuration
	);

	blueKnight->scoreText.init(
		GameConstants::Paths::mainFont,
		GameConstants::BlueKnight::scoreSizeText,
		GameConstants::BlueKnight::r, GameConstants::BlueKnight::g, GameConstants::BlueKnight::b,
		GameConstants::BlueKnight::scoreTextDestX,
		GameConstants::BlueKnight::scoreTextDestY,
		to_string(blueKnight->score).c_str()
	);

	blueKnight->hasBedText.init(
		GameConstants::Paths::mainFont,
		GameConstants::BlueKnight::scoreSizeText,
		GameConstants::BlueKnight::r, GameConstants::BlueKnight::g, GameConstants::BlueKnight::b,
		GameConstants::BlueKnight::hasBedTextDestX,
		GameConstants::BlueKnight::hasBedTextDestY,
		"HAS BED"
	);

	redKnight->transform.vel = min(redKnight->transform.vel, GameConstants::maxVelocityPossible);
	blueKnight->transform.vel = min(blueKnight->transform.vel, GameConstants::maxVelocityPossible);

}

void Game::setUpEntity()
{
	Entity* redEntity = new Entity(Index::redEntity);
	Entity* blueEntity = new Entity(Index::blueEntity);
	
	// same to the player part
	redEntity->transform.init(
		GameConstants::RedEntity::destX,
		GameConstants::RedEntity::destY,
		GameConstants::RedEntity::destW,
		GameConstants::RedEntity::destH,
		GameConstants::RedEntity::velocity
	);

	redEntity->sprite.init(
		GameConstants::Paths::redEntityTexture,
		GameConstants::RedEntity::isAnimated,
		GameConstants::RedEntity::srcX,
		GameConstants::RedEntity::srcY,
		GameConstants::RedEntity::srcW,
		GameConstants::RedEntity::srcH
	);

	blueEntity->transform.init(
		GameConstants::BlueEntity::destX,
		GameConstants::BlueEntity::destY,
		GameConstants::BlueEntity::destW,
		GameConstants::BlueEntity::destH,
		GameConstants::BlueEntity::velocity
	);

	blueEntity->sprite.init(
		GameConstants::Paths::blueEntityTexture,
		GameConstants::BlueEntity::isAnimated,
		GameConstants::BlueEntity::srcX,
		GameConstants::BlueEntity::srcY,
		GameConstants::BlueEntity::srcW,
		GameConstants::BlueEntity::srcH
	);

	redEntity->transform.vel = min(redEntity->transform.vel, GameConstants::maxVelocityPossible);
	blueEntity->transform.vel = min(blueEntity->transform.vel, GameConstants::maxVelocityPossible);
}

void Game::update()
{
	SDL_PollEvent(&event);

	// if the user press the x button at the top right corner of the window
	if (event.type == SDL_QUIT)
		isRunning = false;

	if (!uiManager.isPaused) // if the game pause, neither the map, the player nor the entities is updated 
	{
		players[Index::redPlayer]->updatePlayer();
		players[Index::bluePlayer]->updatePlayer();

		entities[Index::redEntity]->updateEntity();
		entities[Index::blueEntity]->updateEntity();

		uiManager.update();
	}
}

void Game::render()
{
	SDL_RenderClear(renderer);

	map->drawMap();

	entities[Index::redEntity]->drawEntity();
	entities[Index::blueEntity]->drawEntity();

	players[Index::redPlayer]->drawPlayer();
	players[Index::bluePlayer]->drawPlayer();

	uiManager.draw();

	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Mix_FreeChunk(openChestSoundEffect);
	Mix_FreeChunk(powerUpSoundEffect);
	Mix_FreeChunk(hasBedSoundEffect);
	Mix_FreeChunk(returnBedSoundEffect);
	Mix_FreeMusic(backGroundTheme);
	SDL_FreeCursor(uiManager.swordCursor);

	delete map;
	for (pair<int,Player*> player : players) 
	{
		delete player.second;
	}
	for (pair<int, Entity*> entity : entities)
	{
		delete entity.second;
	}
	players.clear();

	Mix_CloseAudio();
	IMG_Quit();
	SDL_Quit();
}
