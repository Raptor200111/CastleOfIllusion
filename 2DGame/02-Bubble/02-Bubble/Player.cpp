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

	sprite->setAnimationSpeed(CLIMB, 8);
	sprite->addKeyframe(CLIMB, glm::vec2(0.066f * 12, 0.098 * 1));

	sprite->setAnimationSpeed(TOUCH_BLOCK, 8);
	sprite->addKeyframe(TOUCH_BLOCK, glm::vec2(0.066f * 12, 0.098 * 1));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	bool oldbClimbing = bClimbing;

	bool newbTouchBlock = false;
	bool right = false;
	if (Game::instance().getKey(GLFW_KEY_LEFT))
	{
		playerState = WALK;
		posPlayer.x -= WALK_SPEED;		
		if (map->collisionBlockLeft(posPlayer, sizePlayer))
		{
			newbTouchBlock = true;
			posPlayer.x += WALK_SPEED;
			sprite->changeAnimation(DODGE);
		}
		else if (map->collisionMoveLeft(posPlayer, sizePlayer))
		{
			posPlayer.x += WALK_SPEED;
			sprite->changeAnimation(STAND);
		}
		
	}
	else if (Game::instance().getKey(GLFW_KEY_RIGHT))
	{
		playerState = WALK;
		posPlayer.x += WALK_SPEED;
		right = true;
		if (map->collisionBlockRight(posPlayer, sizePlayer))
		{
			newbTouchBlock = true;
			posPlayer.x -= WALK_SPEED;
			playerState = DODGE;
		}
		else if (map->collisionMoveRight(posPlayer, sizePlayer))
		{
			posPlayer.x -= WALK_SPEED;
			playerState = STAND;
		}
		 
	}

	else if (Game::instance().getKey(GLFW_KEY_DOWN) && bJumping == false)
	{
		playerState = DODGE;
		//ToDo: Change collision size;
	}
	else
	{
		if (bTouchBlock)
			newbTouchBlock = true;
		
		playerState = STAND;
	}
	
	bClimbing = map->collisionStairs(posPlayer, sizePlayer);
	if (bClimbing)
	{
		playerState = CLIMB;
		//sprite->changeAnimation(CLIMB);
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

			if (Game::instance().getKey(GLFW_KEY_DOWN))
			{
				playerState = PlayerStates::BUTT_FALL;
				buttJumping = true;
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
				velocity = 0.f;
				if (Game::instance().getKey(GLFW_KEY_UP))
				{
					bJumping = true;
					velocity = -10.f;
					startY = posPlayer.y;
					newbTouchBlock = false;
				}
			}
		}
	}

	if (newbTouchBlock != bTouchBlock) {
		if (newbTouchBlock) {
			cout << "Tocado\n";
			playerState = TOUCH_BLOCK;
		}
		else {
			cout << "Dejo de Tocar\n";
		}
	}

	if (oldbClimbing != bClimbing) {
		if (bClimbing) {
			cout << "Collision stairs\n";
			playerState = CLIMB;
		}
		else {
			cout << "NOO Collision stairs\n";
		}
	}
	bTouchBlock = newbTouchBlock;

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

glm::ivec2 Player::getPlayerPos() const
{
	return posPlayer;
}




