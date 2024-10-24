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
	yAxisSpeed = 0.f;
	oldState = newState = IDLE;

	sizeSprite = glm::ivec2(32, 48);
	
	glm::vec2 vec2Array[] = {glm::vec2(0.066f * 2, 0.f), glm::vec2(0.066f * 3, 0.f), glm::vec2(0.066f * 4, 0.f), glm::vec2(0.066f * 5, 0.f), glm::vec2(0.066f * 6, 0.f), glm::vec2(0.066f * 7, 0.f), glm::vec2(0.066f * 8, 0.f)};
	
	particleEfect.init(tileMapPos, getPosition(), sizeSprite, shaderProgram, "images/Mickey_Mouse.png", 8, glm::vec2(0.066, 0.098), vec2Array);

	setSize(glm::ivec2(24, 32));
	setOffset(glm::ivec2(4, 8));

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
	//setPosition(glm::ivec2(0, 0));

}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	bool oldbClimbing = bClimbing;
	bool newbTouchBlock = false; //Why?

	yAxisSpeed += GRAVITY;
	position.y += int(yAxisSpeed) + 1;

	CollisionType colType = CollisionType::None;
	Block* block = nullptr;

	switch (oldState)
	{
	case IDLE:
	{
		if (stopFallingCollision(block, colType))
		{
			if (Game::instance().getKey(GLFW_KEY_K)) {
				newState = JUMP;
				yAxisSpeed = JUMP_SPEED;
				break;
			}
			else if (Game::instance().getKey(GLFW_KEY_S)) {
				if (colType == CollisionType::TileStairs) {
					changeToClimb();
					position.y++;
				}
				else
					newState = DODGE;
				// change size, piensatelo: ???
			}
			yAxisSpeed = 0;
		}
		else
		{
			newState = FALL;
		}

		if (Game::instance().getKey(GLFW_KEY_A)) {
			newState = WALK;
			leftMove();
		}
		if (Game::instance().getKey(GLFW_KEY_D)) {
			newState = WALK;
			rightMove();
		}
		break;
	}
	case WALK:
	{
		newState = IDLE;

		if (stopFallingCollision(block, colType))
		{
			if (Game::instance().getKey(GLFW_KEY_K)) {
				newState = JUMP;
				yAxisSpeed = JUMP_SPEED;
				break;
			}
			else if (Game::instance().getKey(GLFW_KEY_S)) {
				newState = DODGE;
				//setSize(DODGE_SIZE); PIENSATELO
				if (colType == CollisionType::TileStairs)
				{
					changeToClimb();
					position.y++;
				}
			}
			yAxisSpeed = 0;
		}
		else
		{
			newState = FALL;
		}
		if (Game::instance().getKey(GLFW_KEY_A)) {
			leftMove();
			newState = WALK;
		}
		if (Game::instance().getKey(GLFW_KEY_D)) {
			rightMove();
			newState = WALK;
		}
		break;
	}
	case JUMP:
	{
		if (Game::instance().getKey(GLFW_KEY_A)) {
			leftMove();
		}
		if (Game::instance().getKey(GLFW_KEY_D)) {
			rightMove();
		}
		if (Game::instance().getKey(GLFW_KEY_S)) {
			newState = BUTT_FALL;
		}
		if ((stopFallingCollision(block, colType) && yAxisSpeed < 0) || yAxisSpeed > 0)
		{
			yAxisSpeed = 0;
			newState = FALL;
		}
		if (colType == CollisionType::Stairs && Game::instance().getKey(GLFW_KEY_W))
		{
			changeToClimb();
		}
		break;
	}
	case FALL:
	{
		if (Game::instance().getKey(GLFW_KEY_A)) {
			leftMove();
		}
		if (Game::instance().getKey(GLFW_KEY_D)) {
			rightMove();
		}
		if (Game::instance().getKey(GLFW_KEY_S)) {
			newState = BUTT_FALL;
		}
		if (stopFallingCollision(block, colType))
		{
			yAxisSpeed = 0;
			newState = IDLE;
		}

		if (colType == CollisionType::Stairs && Game::instance().getKey(GLFW_KEY_W))
		{
			changeToClimb();
		}
		break;
	}
	case DODGE:
	{
		if (!Game::instance().getKey(GLFW_KEY_S)) {
			setSize(STANDART_SIZE);
			newState = IDLE;
		}
		if (stopFallingCollision(block, colType))
		{
			yAxisSpeed = 0;
		}

		if (Game::instance().getKey(GLFW_KEY_K)) {
			newState = JUMP;
			yAxisSpeed = JUMP_SPEED;
		}
		break;
	}
	case BUTT_FALL:
	{
		if (Game::instance().getKey(GLFW_KEY_A)) {
			leftMove();
		}
		if (Game::instance().getKey(GLFW_KEY_D)) {
			rightMove();
		}
		if (stopFallingCollision(block, colType))
		{
			if (yAxisSpeed > 0)
				newState = IDLE;
			yAxisSpeed = 0;
		}
		if (block != nullptr)
		{
			yAxisSpeed = BUTT_JUMP_SPEED;
			newState = BUTT_JUMP;
			// destruccion del objeto que devuelve
			// delete b;
		}
		break;
	}
	case BUTT_JUMP:
	{
		if (Game::instance().getKey(GLFW_KEY_A)) {
			leftMove();
		}
		if (Game::instance().getKey(GLFW_KEY_D)) {
			rightMove();
		}
		if (stopFallingCollision(block, colType))
		{
			if (yAxisSpeed > 0)
				newState = IDLE;
			yAxisSpeed = 0;
			position.y++;
		}
		if (block != nullptr)
		{
			yAxisSpeed = BUTT_JUMP_SPEED;
			newState = BUTT_JUMP;
			// destruccion del objeto que devuelve
			// delete b;
		}
		break;
	}
	case READY_TO_PICK:
		break;
	case CLIMB_IDLE:
	{
		position.y -= int(yAxisSpeed) + 1;
		yAxisSpeed = 0;

		if (!stairCollision())
		{
			newState == FALL;
			break;
		}
		if (Game::instance().getKey(GLFW_KEY_W)) {
			position.y -= WALK_SPEED;
			newState = CLIMB;
		}
		if (Game::instance().getKey(GLFW_KEY_S)) {
			position.y += WALK_SPEED;
			newState = CLIMB;
		}
		if (Game::instance().getKey(GLFW_KEY_L)) {
			newState = FALL;
		}
		break;
	}
	case CLIMB:
		{
			position.y -= int(yAxisSpeed) + 1;
			yAxisSpeed = 0;

			if (!stairCollision())
			{
				newState = FALL;
				break;
			}
			else
				newState = CLIMB_IDLE;

			if (Game::instance().getKey(GLFW_KEY_W)) {
				position.y -= WALK_SPEED;
				newState = CLIMB;
			}
			if (Game::instance().getKey(GLFW_KEY_S)) {
				position.y += WALK_SPEED;
				newState = CLIMB;
			}
			if (Game::instance().getKey(GLFW_KEY_L)) {
				newState = FALL;
			}
			break;
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

	setPosition(position);
	if (Game::instance().getKey(GLFW_KEY_P))
		cout << position.x << " " << position.y << " - State: " << PlayerStates(oldState) << endl;
  if (Game::instance().getKey(GLFW_KEY_M))
	{
		particleEfect.play(getPosition() - glm::ivec2(-32, 0));
	}
}

void Player::render()
{
	sprite->render();
}

void Player::leftMove()
{
	left = true;
	sprite->setLeft(left);

	position.x -= WALK_SPEED;

	CollisionType colType = CollisionManager::instance().checkCollisionHorizontal(this);
	if (colType == CollisionType::Tile)
		position.x += WALK_SPEED;
	else if ((colType == CollisionType::Stairs || colType == CollisionType::TileStairs) && Game::instance().getKey(GLFW_KEY_W))
		changeToClimb();
	
	Block* b = CollisionManager::instance().collisionEntityBlockH(this);
	if (b != nullptr)
	{
		position.x += WALK_SPEED;
		if (Game::instance().getKey(GLFW_KEY_L))
			cout << "Take Item" << endl;
	}
}

void Player::rightMove()
{
	left = false;
	sprite->setLeft(left);
	position.x += WALK_SPEED;

	CollisionType colType = CollisionManager::instance().checkCollisionHorizontal(this);
	if (colType == CollisionType::Tile)
		position.x -= WALK_SPEED;
	else if ((colType == CollisionType::Stairs || colType == CollisionType::TileStairs) && Game::instance().getKey(GLFW_KEY_W))
		changeToClimb();

	Block* b = CollisionManager::instance().collisionEntityBlockH(this);
	if (b != nullptr)
	{
		position.x -= WALK_SPEED;
		if (Game::instance().getKey(GLFW_KEY_L))
			cout << "Take Item" << endl;
	}
}


void Player::changeToClimb()
{
	newState = CLIMB_IDLE;
	//calcular posicion de la escalera
}

bool Player::stopFallingCollision(Block*& block, CollisionType& colType)
{
	auto originalPos = position;
	colType = CollisionManager::instance().checkCollisionVertical(this);
	
	auto tileCorrectedPos = position;
	block = CollisionManager::instance().collisionEntityBlockV(this);

	if (originalPos != tileCorrectedPos && tileCorrectedPos != position)
		position = tileCorrectedPos;

	if (colType == CollisionType::Tile || colType == CollisionType::TileStairs)
		return true;
	if (block != nullptr) 
		return true;
	return false;
}

bool Player::stairCollision()
{
	auto oldPos = position;
	auto colType = CollisionManager::instance().checkCollisionVertical(this);
	position = oldPos;
	if (colType == CollisionType::Stairs || colType == CollisionType::TileStairs)
		return true;
	return false;
}
