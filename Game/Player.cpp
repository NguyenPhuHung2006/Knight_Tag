#include "Player.h"
#include "Map.h"
#include "GameConstant.h"
#include "Game.h"
#include <iostream>

extern unordered_map<int, Player*> players;

Player::Player(int index)
{
	// each component of the player will have an index equal to the index of the player for the simplicity of the code
	sprite.index = index;
	keyboard.index = index;
	transform.index = index;
	skills.index = index;
	collider.index = index;

	hasBed = false;
	score = 0;

	players[index] = this;

	collider.transformLink = &transform;
}

void Player::updatePlayer()
{
	collider.update();
	if (!collider.isTagged)
	{
		keyboard.update();
		skills.update();
		sprite.update();
		transform.update();
	}
	scoreText.updateText(score);
}

void Player::drawPlayer()
{
	if(!collider.isTagged)
		sprite.draw();

	scoreText.drawText();

	if (hasBed)
		hasBedText.drawText();
}