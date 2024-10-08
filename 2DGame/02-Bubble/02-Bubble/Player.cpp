#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerStates
{
	STAND, WALK, JUMP, FALL, DODGE, BUTT_FALL, BUTT_JUMP, FALL_TO_STAND
};


void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	bJumping = false;
	velocity = 0.f;
	spritesheet.loadFromFile("images/Mickey_Mouse.png", TEXTURE_PIXEL_FORMAT_RGBA);	
	sprite = Sprite::createSprite(sizePlayer, glm::vec2(0.066, 0.098), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(STAND, 8);
	sprite->addKeyframe(STAND, glm::vec2(0.f, 0.f)); //ToDo:Hay que girar el Sprite ya que no tenemos animacion left
	sprite->addKeyframe(STAND, glm::vec2(0.066f, 0.f));

	sprite->setAnimationSpeed(WALK, 8);
	sprite->addKeyframe(WALK, glm::vec2(0.066f * 2, 0.f)); //ToDo:Hay que girar el Sprite ya que no tenemos animacion left
	sprite->addKeyframe(WALK, glm::vec2(0.066f * 3, 0.f));
	sprite->addKeyframe(WALK, glm::vec2(0.066f * 4, 0.f));
	sprite->addKeyframe(WALK, glm::vec2(0.066f * 5, 0.f));
	sprite->addKeyframe(WALK, glm::vec2(0.066f * 6, 0.f));
	sprite->addKeyframe(WALK, glm::vec2(0.066f * 7, 0.f));
	sprite->addKeyframe(WALK, glm::vec2(0.066f * 8, 0.f));


	sprite->setAnimationSpeed(DODGE, 8);
	sprite->addKeyframe(DODGE, glm::vec2(0.066f * 0, 0.098 * 1)); //ToDo:Hay que girar el Sprite ya que no tenemos animacion left
	sprite->addKeyframe(DODGE, glm::vec2(0.066f * 1, 0.098 * 1));

	sprite->setAnimationSpeed(JUMP, 8);
	sprite->addKeyframe(JUMP, glm::vec2(0.066f * 2, 0.098 * 1)); //ToDo:Hay que girar el Sprite ya que no tenemos animacion left

	sprite->setAnimationSpeed(FALL, 8);
	sprite->addKeyframe(FALL, glm::vec2(0.066f * 3, 0.098 * 1)); //ToDo:Hay que girar el Sprite ya que no tenemos animacion left

	sprite->setAnimationSpeed(FALL_TO_STAND, 8);
	sprite->addKeyframe(FALL_TO_STAND, glm::vec2(0.066f * 4, 0.098 * 1));

	sprite->setAnimationSpeed(BUTT_FALL, 8);
	sprite->addKeyframe(BUTT_FALL, glm::vec2(0.066f * 5, 0.098 * 1));

	sprite->setAnimationSpeed(BUTT_JUMP, 8);
	sprite->addKeyframe(BUTT_JUMP, glm::vec2(0.066f * 6, 0.098 * 1));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (Game::instance().getKey(GLFW_KEY_LEFT))
	{
		if (sprite->animation() != WALK)
			sprite->changeAnimation(WALK);
		posPlayer.x -= 2;
		if (map->collisionMoveLeft(posPlayer, sizePlayer))
		{
			posPlayer.x += 2;
			sprite->changeAnimation(STAND);
		}
	}
	else if (Game::instance().getKey(GLFW_KEY_RIGHT))
	{
		if (sprite->animation() != WALK)
			sprite->changeAnimation(WALK);
		posPlayer.x += 2;
		if (map->collisionMoveRight(posPlayer, sizePlayer))
		{
			posPlayer.x -= 2;
			sprite->changeAnimation(STAND);
		}
	}
	else
	{
		if (sprite->animation() == WALK)
			sprite->changeAnimation(STAND);
		else if (sprite->animation() == WALK)
			sprite->changeAnimation(STAND);
	}

	if (bJumping)
	{
		velocity += 0.5f;
		posPlayer.y += int(velocity);

		if (velocity < 0)
		{
			if (sprite->animation() != JUMP)
				sprite->changeAnimation(JUMP);

			if (map->collisionMoveUp(posPlayer, sizePlayer, &posPlayer.y))
			{

			}
		}
		if (velocity > 0)
		{
			if (sprite->animation() != FALL)
				sprite->changeAnimation(FALL);

			if (map->collisionMoveDown(posPlayer, sizePlayer, &posPlayer.y))
			{ 
				for (int i = 0; i < int(abs(velocity)); i++)
				{
					posPlayer.y--;
					if (!map->collisionMoveDown(posPlayer, sizePlayer, &posPlayer.y))
						break;

				}
				velocity = 0;
				bJumping = false;
			}
		}
	}
	else
	{
		
		posPlayer.y += FALL_STEP;
		if (map->collisionMoveDown(posPlayer, sizePlayer, &posPlayer.y))
		{
			if (Game::instance().getKey(GLFW_KEY_UP))
			{
				bJumping = true;
				velocity = -10.f;
				startY = posPlayer.y;
			}
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}




