#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"

#define FALL_STEP 2
#define WALK_SPEED 2
#define GRAVITY 0.5f
#define JUMP_SPEED -9.f
#define BUTT_JUMP_SPEED -12.f
#define STANDART_SIZE glm::ivec2(24, 32)
#define DODGE_SIZE glm::ivec2(24, 32)


void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	bClimbing = false;
	bTouchBlock = false;
	velocity = 0.f;
	oldState = newState = IDLE;

	sizeSprite = glm::ivec2(32, 48);
	posPlayer = glm::ivec2(0, 0);
	sizePlayer = glm::ivec2(24, 32);
	offset = glm::ivec2(4, 8);

	spritesheet.loadFromFile("images/Mickey_Mouse.png", TEXTURE_PIXEL_FORMAT_RGBA);	
	sprite = Sprite::createSprite(sizeSprite, glm::vec2(0.066, 0.098), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(TOTAL);

	sprite->setAnimationSpeed(IDLE, 4);
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.f)); 
	sprite->addKeyframe(IDLE, glm::vec2(0.066f, 0.f));

	sprite->setAnimationSpeed(WALK, 8);
	sprite->addKeyframe(WALK, glm::vec2(0.066f * 2, 0.f)); 
	sprite->addKeyframe(WALK, glm::vec2(0.066f * 3, 0.f));
	sprite->addKeyframe(WALK, glm::vec2(0.066f * 4, 0.f));
	sprite->addKeyframe(WALK, glm::vec2(0.066f * 5, 0.f));
	sprite->addKeyframe(WALK, glm::vec2(0.066f * 6, 0.f));
	sprite->addKeyframe(WALK, glm::vec2(0.066f * 7, 0.f));
	sprite->addKeyframe(WALK, glm::vec2(0.066f * 8, 0.f));

	sprite->setAnimationSpeed(DODGE, 8);
	sprite->addKeyframe(DODGE, glm::vec2(0, 0.098 * 1)); 
	sprite->addKeyframe(DODGE, glm::vec2(0.066f * 1, 0.098 * 1));

	sprite->setAnimationSpeed(JUMP, 8);
	sprite->addKeyframe(JUMP, glm::vec2(0.066f * 2, 0.098 * 1)); 

	sprite->setAnimationSpeed(FALL, 8);
	sprite->addKeyframe(FALL, glm::vec2(0.066f * 3, 0.098 * 1)); 

	sprite->setAnimationSpeed(READY_TO_PICK, 8);
	sprite->addKeyframe(READY_TO_PICK, glm::vec2(0.066f * 0, 0.098 * 3));

	sprite->setAnimationSpeed(BUTT_FALL, 8);
	sprite->addKeyframe(BUTT_FALL, glm::vec2(0.066f * 5, 0.098 * 1));

	sprite->setAnimationSpeed(BUTT_JUMP, 8);
	sprite->addKeyframe(BUTT_JUMP, glm::vec2(0.066f * 6, 0.098 * 1));

	sprite->setAnimationSpeed(CLIMB_IDLE, 8);
	sprite->addKeyframe(CLIMB_IDLE, glm::vec2(0.066f * 11, 0.098 * 0));

	sprite->setAnimationSpeed(CLIMB, 8);
	sprite->addKeyframe(CLIMB, glm::vec2(0.066f * 12, 0.098 * 0));
	sprite->addKeyframe(CLIMB, glm::vec2(0.066f * 13, 0.098 * 0));

	sprite->changeAnimation(IDLE);
	tileMapDispl = tileMapPos;
	//setPosition(posPlayer);
	//sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	bool oldbClimbing = bClimbing;
	bool newbTouchBlock = false; //Why?

	velocity += GRAVITY;
	posPlayer.y += int(velocity) + 1;

	if (map->collisionStairs(posPlayer, sizePlayer))
	{
		newState = CLIMB_IDLE;
		velocity = 0;
	}
	else if (oldState == CLIMB_IDLE || oldState == CLIMB)
	{
		oldState = IDLE;
		newState = IDLE;
		sprite->changeAnimation(IDLE);
	}
	else
		newState = oldState;

	if (oldState == PlayerStates::BUTT_FALL) {
		if (Game::instance().getKey(GLFW_KEY_A)) {
			leftMove();
		}
		if (Game::instance().getKey(GLFW_KEY_D)) {
			rightMove();
		}
		if (map->collisionMoveUp(posPlayer, sizePlayer, &posPlayer.y))
		{
			velocity = 0;
		}
		if (map->collisionMoveDown(posPlayer, sizePlayer, &posPlayer.y))
		{
			velocity = 0;
			newState = IDLE;
		}
		posPlayer.y += 1;
		if (map->collisionBlockDown(posPlayer, sizePlayer, &posPlayer.y))
		{
			jump(BUTT_JUMP_SPEED);
			newState = BUTT_JUMP;
			//cout << "Butt With Block" << endl;
		}
	}
	if (oldState == PlayerStates::BUTT_JUMP) {
		if (Game::instance().getKey(GLFW_KEY_A)) {
			leftMove();
		}
		if (Game::instance().getKey(GLFW_KEY_D)) {
			rightMove();
		}
		if (map->collisionMoveUp(posPlayer, sizePlayer, &posPlayer.y))
		{
			velocity = 0;
		}
		if (map->collisionMoveDown(posPlayer, sizePlayer, &posPlayer.y) || map->collisionBlockDown(posPlayer, sizePlayer, &posPlayer.y))
		{
			velocity = 0;
			newState = IDLE;
		}
	}
	
	if (oldState == PlayerStates::CLIMB_IDLE || oldState == PlayerStates::CLIMB) {
		posPlayer.y -= int(velocity) + 1;
		velocity = 0;
		if (Game::instance().getKey(GLFW_KEY_A)) {
			leftMove();
		}
		if (Game::instance().getKey(GLFW_KEY_D)) {
			rightMove();
		}
		if (Game::instance().getKey(GLFW_KEY_W)) {
			posPlayer.y -= WALK_SPEED;
			newState = CLIMB;
		}
		if (Game::instance().getKey(GLFW_KEY_S)) {
			posPlayer.y += WALK_SPEED;
			newState = CLIMB;
		}
	}
	if (oldState == PlayerStates::DODGE) {
		if (!Game::instance().getKey(GLFW_KEY_S)) {
			setSize(STANDART_SIZE);
			newState = IDLE;
		}
		if (map->collisionMoveDown(posPlayer, sizePlayer, &posPlayer.y))
		{
			velocity = 0;
		}
	}
	if (oldState == PlayerStates::WALK) {
		if (map->collisionMoveDown(posPlayer, sizePlayer, &posPlayer.y))
		{
			velocity = 0;
			if (Game::instance().getKey(GLFW_KEY_W)) {
				newState = JUMP;
				jump(JUMP_SPEED);
			}
			else if (Game::instance().getKey(GLFW_KEY_S)) {
				newState = DODGE;
				setSize(DODGE_SIZE);
			}
		}
		else
			newState = FALL;
		bool movement = false;
		if (Game::instance().getKey(GLFW_KEY_A)) {
			leftMove();
			movement = true;
		}
		if (Game::instance().getKey(GLFW_KEY_D)) {
			rightMove();
			movement = true;
		}
		if (!movement) {
			newState = IDLE;
		}
		else if (Game::instance().getKey(GLFW_KEY_L) && newState == WALK) {
			//grab item
		}

	}
	if (oldState == PlayerStates::IDLE) {
		if (map->collisionMoveDown(posPlayer, sizePlayer, &posPlayer.y))
		{
			velocity = 0;
			if (Game::instance().getKey(GLFW_KEY_W)) {
				newState = JUMP;
				jump(JUMP_SPEED);
			}
		}
		else
			newState = FALL;

		if (Game::instance().getKey(GLFW_KEY_A)) {
			newState = WALK;
			leftMove();
		}
		if (Game::instance().getKey(GLFW_KEY_D)) {
			newState = WALK;
			rightMove();
		}
		if (Game::instance().getKey(GLFW_KEY_S)) {
			newState = DODGE;
			setSize(DODGE_SIZE);
		}
	}
	if (oldState == PlayerStates::FALL) {
		if (Game::instance().getKey(GLFW_KEY_A)) {
			leftMove();
		}
		if (Game::instance().getKey(GLFW_KEY_D)) {
			rightMove();
		}
		if (Game::instance().getKey(GLFW_KEY_S)) {
			newState = BUTT_FALL;
		}
		if (map->collisionMoveDown(posPlayer, sizePlayer, &posPlayer.y))
		{
			velocity = 0;
			newState = IDLE;
		}
	}
	if (oldState == PlayerStates::JUMP) {
		if (Game::instance().getKey(GLFW_KEY_A)) {
			leftMove();
		}
		if (Game::instance().getKey(GLFW_KEY_D)) {
			rightMove();
		}
		if (Game::instance().getKey(GLFW_KEY_S)) {
			newState = BUTT_FALL;
		}
		if (velocity > 0 || map->collisionMoveUp(posPlayer, sizePlayer, &posPlayer.y))
		{
			velocity = 0;
			newState = FALL;
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

	bTouchBlock = newbTouchBlock;

	if (oldState != newState) {
		oldState = newState;
		sprite->changeAnimation(newState);
	}
		
	//sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	setPosition(posPlayer);
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
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x - offset.x), float(tileMapDispl.y + posPlayer.y - offset.y)));
}

glm::ivec2 Player::getPosition()
{
	return posPlayer;
}

glm::ivec2 Player::getSize()
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

void Player::leftMove()
{
	left = true;
	sprite->setLeft(left);
	
	posPlayer.x -= WALK_SPEED;
	if (map->collisionBlockLeft(posPlayer, sizePlayer))
	{
		//newbTouchBlock = true; // WHY ?
		posPlayer.x += WALK_SPEED;
	}
	else if (map->collisionMoveLeft(posPlayer, sizePlayer))
	{
		posPlayer.x += WALK_SPEED;
	}
}

void Player::rightMove()
{
	left = false;
	sprite->setLeft(left);
	posPlayer.x += WALK_SPEED;
	if (map->collisionBlockRight(posPlayer, sizePlayer))
	{
		//newbTouchBlock = true;
		posPlayer.x -= WALK_SPEED;
	}
	else if (map->collisionMoveRight(posPlayer, sizePlayer))
	{
		posPlayer.x -= WALK_SPEED;
	}
}

void Player::jump(int v)
{
	velocity = v;
}

void Player::setSize(glm::ivec2 newSize)
{
	sizePlayer = newSize;
}

