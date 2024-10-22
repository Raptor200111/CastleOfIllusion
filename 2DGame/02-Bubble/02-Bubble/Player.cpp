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
	bJumping = false;
	velocity = 0.f;
	oldState = newState = IDLE;

	sizeSprite = glm::ivec2(32, 48);
	position = glm::ivec2(0, 0);
	sizeObject = glm::ivec2(24, 32);
	offset = glm::ivec2(4, 8);
	
	glm::vec2 vec2Array[] = {glm::vec2(0.066f * 2, 0.f), glm::vec2(0.066f * 3, 0.f), glm::vec2(0.066f * 4, 0.f), glm::vec2(0.066f * 5, 0.f), glm::vec2(0.066f * 6, 0.f), glm::vec2(0.066f * 7, 0.f), glm::vec2(0.066f * 8, 0.f)};
	
	particleEfect.init(tileMapPos, getPosition(), sizeSprite, shaderProgram, "images/Mickey_Mouse.png", 8, glm::vec2(0.066, 0.098), vec2Array);

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
	//setPosition(position);
	//sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	bool oldbClimbing = bClimbing;
	bool newbTouchBlock = false; //Why?

	velocity += GRAVITY;
	position.y += int(velocity) + 1;

	if (Game::instance().getKey(GLFW_KEY_A))
	{
		leftMove();
	}
	else if (Game::instance().getKey(GLFW_KEY_D))
	{
		rightMove();
	}
	else
		newState = oldState;

	if (Game::instance().getKey(GLFW_KEY_M))
	{
		particleEfect.play(getPosition() - glm::ivec2(-32, 0));
	}

	if (bJumping)
	{
		if (Game::instance().getKey(GLFW_KEY_S)) {
			newState = BUTT_FALL;
		}
		if (velocity > 0 || CollisionManager::instance().checkCollisionVertical(this) == CollisionType::Tile)
		{
			velocity = 0;
		}
	}
	else
	{
		if (CollisionManager::instance().checkCollisionVertical(this) == CollisionType::Tile)
		{
			if (Game::instance().getKey(GLFW_KEY_W))
			{
				bJumping = false;
				newState = IDLE;
				jump(JUMP_SPEED);
			}
			else {
				velocity = 0;
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

	bTouchBlock = newbTouchBlock;

	if (oldState != newState) {
		oldState = newState;
		sprite->changeAnimation(newState);
	}
	sprite->setLeft(left);

	//sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
	setPosition(position);
}

void Player::render()
{
	sprite->render();
}

void Player::leftMove()
{
	left = true;	
	position.x -= WALK_SPEED;
	/*if (map->collisionBlockLeft(posPlayer, sizePlayer))
	{
		//newbTouchBlock = true; // WHY ?
		position.x += WALK_SPEED;
	}
	else*/
	if (CollisionManager::instance().checkCollisionHorizontal(this) == CollisionType::Tile)
	{
		position.x += WALK_SPEED;
	}
}

void Player::rightMove()
{
	
	left = false;
	position.x += WALK_SPEED;
	/*
	if (map->collisionBlockRight(position, sizeObject))
	{
		//newbTouchBlock = true;
		position.x -= WALK_SPEED;
	}
	else*/
	if (CollisionManager::instance().checkCollisionHorizontal(this) == CollisionType::Tile)
	{
		position.x -= WALK_SPEED;
	}
	
}

void Player::jump(int speed)
{
	velocity = speed;
}

void Player::setSize(glm::ivec2 newSize)
{
	sizeObject = newSize;
}
