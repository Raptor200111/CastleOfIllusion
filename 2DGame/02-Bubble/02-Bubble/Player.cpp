#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"

#define FALL_STEP 2
#define WALK_SPEED 2
#define GRAVITY 0.5f


void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	bJumping = false;
	buttJumping = false;
	bClimbing = false;
	bTouchBlock = false;
	velocity = 0.f;
	playerState = STAND;

	spritesheet.loadFromFile("images/Mickey_Mouse.png", TEXTURE_PIXEL_FORMAT_RGBA);	
	sprite = Sprite::createSprite(sizePlayer, glm::vec2(0.066, 0.098), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(8);

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
	sprite->addKeyframe(DODGE, glm::vec2(0, 0.098 * 1)); //ToDo:Hay que girar el Sprite ya que no tenemos animacion left
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

	sprite->setAnimationSpeed(CLIMB, 8);
	sprite->addKeyframe(CLIMB, glm::vec2(0.066f * 12, 0.098 * 1));

	//sprite->setAnimationSpeed(TOUCH_BLOCK, 8);
	//sprite->addKeyframe(TOUCH_BLOCK, glm::vec2(0.066f * 12, 0.098 * 1));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);

	bool otherState = false;
	bClimbing = map->collisionStairs(posPlayer, sizePlayer); //Check
	bool newbTouchBlock = false; //Why?

	if (Game::instance().getKey(GLFW_KEY_LEFT))
	{
		playerState = WALK;
		otherState = true;
		posPlayer.x -= WALK_SPEED;
		sprite->setLeft(true);
		
		if (map->collisionBlockLeft(posPlayer, sizePlayer))
		{
			newbTouchBlock = true; // WHY ?
			posPlayer.x += WALK_SPEED;
			playerState = STAND;
		}
		else if (map->collisionMoveLeft(posPlayer, sizePlayer))
		{
			posPlayer.x += WALK_SPEED;
			playerState = STAND;
		}
		
	}
	else if (Game::instance().getKey(GLFW_KEY_RIGHT))
	{
		playerState = WALK;
		otherState = true;
		posPlayer.x += WALK_SPEED;
		sprite->setLeft(false);

		if (map->collisionBlockRight(posPlayer, sizePlayer))
		{
			newbTouchBlock = true;
			posPlayer.x -= WALK_SPEED;
			playerState = STAND;
		}
		else if (map->collisionMoveRight(posPlayer, sizePlayer))
		{
			posPlayer.x -= WALK_SPEED;
			playerState = STAND;
		}
		 
	}
	else if (Game::instance().getKey(GLFW_KEY_DOWN) && !bJumping)
	{
		playerState = DODGE;
		otherState = true;
	}
	else
	{
		if (bTouchBlock) //Again, Why?
			newbTouchBlock = true;
		playerState = STAND;
	}
	
	if (bClimbing)
	{
		otherState = true;
		playerState = CLIMB_IDLE;
		velocity = 0;  // Disable gravity while on stairs

		if (Game::instance().getKey(GLFW_KEY_UP))
		{
			posPlayer.y -= WALK_SPEED;  // Move up the stairs
		}
		if (Game::instance().getKey(GLFW_KEY_DOWN))
		{
			posPlayer.y += WALK_SPEED;  // Move down the stairs
		}
	}
	else
	{
		if (bJumping)
		{
			velocity += GRAVITY;
			posPlayer.y += int(velocity);

			if (Game::instance().getKey(GLFW_KEY_DOWN) || buttJumping == true)
			{
				playerState = BUTT_FALL;
				buttJumping = true;
				otherState = true;

				if (map->collisionBlockDown(posPlayer, sizePlayer, &posPlayer.y)) {
					playerState = BUTT_JUMP;
					velocity = -15.f;
					//Se tiene que destrozar el objeto (creo)
					//booleano de butt_jump
				}
			}

			if (velocity < 0)
			{
				if (!buttJumping)
					playerState = JUMP;

				if (map->collisionMoveUp(posPlayer, sizePlayer, &posPlayer.y))
				{
					velocity = 0;
				}
			}
			if (velocity > 0)
			{
				if (!buttJumping)
					playerState = FALL;

				if (map->collisionMoveDown(posPlayer, sizePlayer, &posPlayer.y))
				{
					velocity = 0;
					bJumping = false;
				}
			}
		}
		else
		{
			velocity += GRAVITY;
			posPlayer.y += int(velocity);
			buttJumping = false;

			if (map->collisionMoveDown(posPlayer, sizePlayer, &posPlayer.y))
			{
				if (Game::instance().getKey(GLFW_KEY_UP))
				{
					bJumping = true;
					velocity = -10.f;
					newbTouchBlock = false;
				}
				else
					velocity = 0.f;
			}
		}
	}

	if (newbTouchBlock != bTouchBlock) { //From here to...
		if (newbTouchBlock) {
			cout << "Tocado\n";
			//playerState = TOUCH_BLOCK;
		}
		else {
			cout << "Dejo de Tocar\n";
		}
	}
	bTouchBlock = newbTouchBlock; //...here, why?

	if (!otherState)
	{
		playerState = STAND;
	}

	if (sprite->animation() != playerState)
		sprite->changeAnimation(playerState);
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

glm::vec2 Player::getPosition()
{
	return posPlayer;
}

glm::vec2 Player::getSize()
{
	return sizePlayer;
}