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
	buttFalling = false;
	bClimbing = false;
	bTouchBlock = false;
	buttJumping = false;
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
	bool oldbClimbing = bClimbing;

	bool otherState = false;
	bClimbing = map->collisionStairs(posPlayer, sizePlayer); //Check
	bool newbTouchBlock = false; //Why?

	if (Game::instance().getKey(GLFW_KEY_A))
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
	else if (Game::instance().getKey(GLFW_KEY_D))
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
	else if (Game::instance().getKey(GLFW_KEY_S) && !bJumping)
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
	
	bClimbing = map->collisionStairs(posPlayer, sizePlayer);
	if (bClimbing)
	{
		otherState = true;
		playerState = CLIMB_IDLE;
		velocity = 0;  // Disable gravity while on stairs

		if (Game::instance().getKey(GLFW_KEY_W))
		{
			posPlayer.y -= WALK_SPEED;  // Move up the stairs
		}
		if (Game::instance().getKey(GLFW_KEY_S))
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
			otherState = true;

			if (Game::instance().getKey(GLFW_KEY_S) || buttFalling == true)
			{
				playerState = BUTT_FALL;
				buttFalling = true;
				otherState = true;

				if (!buttJumping && map->collisionBlockDown(posPlayer, sizePlayer, &posPlayer.y)) {
					velocity = -15.f;
					buttJumping = true;
					//Se tiene que destrozar el objeto (creo)
					//booleano de butt_jump
				}

				if (buttJumping)
					playerState = BUTT_JUMP;
			}

			if (velocity < 0)
			{
				if (!buttFalling)
					playerState = JUMP;

				if (map->collisionMoveUp(posPlayer, sizePlayer, &posPlayer.y))
				{
					velocity = 0;
				}
			}
			if (velocity > 0)
			{
				if (!buttFalling)
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
			buttFalling = false;
			buttJumping = false;

			if (map->collisionMoveDown(posPlayer, sizePlayer, &posPlayer.y))
			{
				if (Game::instance().getKey(GLFW_KEY_W))
				{
					bJumping = true;
					velocity = -10.f;
					newbTouchBlock = false;
					otherState = true;
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

bool Player::checkCollisionObject(const PosSizeObject& object)
{
	glm::ivec2 pos1 = posPlayer;
	glm::ivec2 size1 = pos1 + sizePlayer;
	glm::ivec2 pos2 = object.posObject;
	glm::ivec2 size2 = pos2 + object.sizeObject;

	if (size1.x < pos2.x || size2.x < pos1.x) return false;
	if (size1.y < pos2.y || size2.y < pos1.y) return false;
	return true;
}

