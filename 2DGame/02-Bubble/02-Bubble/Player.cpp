#include "Player.h"
#include "Game.h"

#define FALL_STEP 2
#define WALK_SPEED 2
#define GRAVITY 0.5f
#define JUMP_SPEED -9.f
#define BUTT_JUMP_SPEED -12.f
#define STANDART_SIZE glm::ivec2(24, 32)
#define DODGE_SIZE glm::ivec2(24, 21)
#define OFFSET glm::ivec2(4, 8)

Player::~Player()
{
	if (particleEfect != NULL)
		delete particleEfect;
}

void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	jumpAvailable = true;
	objInteractionAvailable = true;
	yAxisSpeed = 0.f;
	oldState = newState = IDLE;

	pickedUpBlock = nullptr;

	sizeSprite = glm::ivec2(32, 48);

	//AÑADIR ANIMACIONES MAS CHULAS
	glm::vec2* vec2Array = new glm::vec2[6];
	
	vec2Array[0] = glm::vec2(0.066f * 2, 0.f);
	vec2Array[1] = glm::vec2(0.066f * 3, 0.f);
	vec2Array[2] = glm::vec2(0.066f * 4, 0.f);
	vec2Array[3] = glm::vec2(0.066f * 5, 0.f);
	vec2Array[4] = glm::vec2(0.066f * 6, 0.f);
	vec2Array[5] = glm::vec2(0.066f * 7, 0.f);

	particleEfect = new ParticleEfect(tileMapPos, position, sizeSprite, shaderProgram, "images/Mickey_Mouse.png", glm::vec2(0.066, 0.098), 1);
	particleEfect->addAnimation(8, vec2Array, 6);

	delete vec2Array;
	//HASTA AQUI

	sizeObject = STANDART_SIZE;
	offset = OFFSET;

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

	sprite->setAnimationSpeed(B_PICK, 6);
	sprite->addKeyframe(B_PICK, glm::vec2(0.066f * 3, 0.098 * 3));
	sprite->addKeyframe(B_PICK, glm::vec2(0.066f * 2, 0.098 * 3));
	sprite->addKeyframe(B_PICK, glm::vec2(0.066f * 4, 0.098 * 3));

	sprite->setAnimationSpeed(B_IDLE, 8);
	sprite->addKeyframe(B_IDLE, glm::vec2(0.066f * 4, 0.098 * 3));
	sprite->addKeyframe(B_IDLE, glm::vec2(0.066f * 4, 0.098 * 3));
	sprite->addKeyframe(B_IDLE, glm::vec2(0.066f * 5, 0.098 * 3));

	sprite->setAnimationSpeed(B_WALK, 8);
	sprite->addKeyframe(B_WALK, glm::vec2(0.066f * 0, 0.098 * 4));
	sprite->addKeyframe(B_WALK, glm::vec2(0.066f * 1, 0.098 * 4));
	sprite->addKeyframe(B_WALK, glm::vec2(0.066f * 2, 0.098 * 4));
	sprite->addKeyframe(B_WALK, glm::vec2(0.066f * 3, 0.098 * 4));
	sprite->addKeyframe(B_WALK, glm::vec2(0.066f * 4, 0.098 * 4));
	sprite->addKeyframe(B_WALK, glm::vec2(0.066f * 5, 0.098 * 4));

	sprite->setAnimationSpeed(B_JUMP, 8);
	sprite->addKeyframe(B_JUMP, glm::vec2(0.066f * 9, 0.098 * 4));

	sprite->setAnimationSpeed(B_FALL, 8);
	sprite->addKeyframe(B_FALL, glm::vec2(0.066f * 10, 0.098 * 4));

	sprite->changeAnimation(IDLE);
	tileMapDispl = tileMapPos;
	//setPosition(glm::ivec2(0, 0));
}

void Player::reStartStatePlayer()
{
	jumpAvailable = true;
	objInteractionAvailable = true;
	yAxisSpeed = 0.f;
	oldState = newState = IDLE;

	pickedUpBlock = nullptr;
	readyToPickBlock = nullptr;
	block = nullptr;
	colType = CollisionType::None;
	sizeSprite = glm::ivec2(32, 48);
	setSize(glm::ivec2(24, 32));
	setOffset(glm::ivec2(4, 8));
	sprite->changeAnimation(IDLE);

}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);

	yAxisSpeed += GRAVITY;
	position.y += int(yAxisSpeed) + 1;

	colType = CollisionType::None;
	block = nullptr;

	if (entityState == DYING) {
		elapsedTime += deltaTime;
		if (elapsedTime >= timeDyingAnim) {
			elapsedTime = 0;
			if (Game::instance().getStars() > 0)
				entityState = STILL;
			else
				entityState = DEAD;
		}
	}

	switch (oldState)
	{
		case IDLE:
		{
			movementBehaviour();
			break;
		}
		case WALK:
		{
			newState = IDLE;
			movementBehaviour();
			break;
		}
		case JUMP:
		{
			if (Game::instance().getKey(GLFW_KEY_A)) {
				horizontalMove(true);
			}
			if (Game::instance().getKey(GLFW_KEY_D)) {
				horizontalMove(false);
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
				horizontalMove(true);
			}
			if (Game::instance().getKey(GLFW_KEY_D)) {
				horizontalMove(false);
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
				sizeObject = STANDART_SIZE;
				offset = OFFSET;
				position.y -= 11;
				newState = IDLE;
			}
			if (stopFallingCollision(block, colType))
			{
				yAxisSpeed = 0;
			}

			if (checkJumpButton()) {
				newState = JUMP;
				yAxisSpeed = JUMP_SPEED;
			}
			break;
		}
		case BUTT_FALL:
		{
			buttJumpBehaviour();
			break;
		}
		case BUTT_JUMP:
		{
			buttJumpBehaviour();
			break;
		}
		case READY_TO_PICK:
		{
			newState = READY_TO_PICK;
			movementBehaviour();
			if (checkObjInteractionButton()) {
				pickUpBlock();
			}
			break;
		}
		case CLIMB_IDLE:
		{
			climbBehaviour();
			break;
		}
		case CLIMB:
		{
			newState = CLIMB_IDLE;
			climbBehaviour();
			break;
		}
		case B_PICK:
		{
			position.y -= int(yAxisSpeed) + 1;
			auto keyframe = sprite->getCurrentKeyframe();
			if (keyframe.x == keyframe.y-1)
				newState = B_IDLE;
			if (keyframe.x > 0)
			{
				int y = position.y + sizeObject.y / 2 - pickedUpBlock->getSize().y / 2;
				int x;
				if (left)
					x = position.x - pickedUpBlock->getSize().x;
				else
					x = position.x + sizeObject.x;
				pickedUpBlock->setPosition(glm::ivec2(x, y));
			}
			break;
		}
		case B_IDLE:
		{
			blockMovementBehaviour();
			break;
		}
		case B_WALK:
		{
			newState = B_IDLE;
			blockMovementBehaviour();
			break;
		}
		case B_JUMP:
		{
			if (Game::instance().getKey(GLFW_KEY_A)) {
				horizontalMove(true);
			}
			if (Game::instance().getKey(GLFW_KEY_D)) {
				horizontalMove(false);
			}
			if (checkObjInteractionButton()) {
				throwBlock();
			}
			if ((stopFallingCollision(block, colType) && yAxisSpeed < 0) || yAxisSpeed > 0)
			{
				yAxisSpeed = 0;
				newState = B_FALL;
			}
			break;
		}
		case B_FALL:
		{
			if (Game::instance().getKey(GLFW_KEY_A)) {
				horizontalMove(true);
			}
			if (Game::instance().getKey(GLFW_KEY_D)) {
				horizontalMove(false);
			}
			if (checkObjInteractionButton()) {
				//newState = BUTT_FALL;
				throwBlock();
			}
			if (stopFallingCollision(block, colType))
			{
				yAxisSpeed = 0;
				newState = B_IDLE;
			}
			break;
		}
	}

	if (oldState != newState) {
		oldState = newState;
		sprite->changeAnimation(newState);
	}

	setPosition(position);
	if (Game::instance().getKey(GLFW_KEY_P))
		cout << position.x << " " << position.y << " - State: " << PlayerStates(oldState) << endl;
	
	particleEfect->update(deltaTime);
	if (pickedUpBlock != nullptr)
	{
		pickedUpBlock->update(deltaTime);
		if (oldState != B_PICK)
		{
			if (left)
			{
				pickedUpBlock->setPosition(glm::ivec2(position.x, position.y + 10 - pickedUpBlock->getSize().y));
			}
			else
			{
				pickedUpBlock->setPosition(glm::ivec2(position.x + this->getSize().x - pickedUpBlock->getSize().x, position.y + 10 - pickedUpBlock->getSize().y));
			}
		}
	}
}

void Player::render()
{
	if (pickedUpBlock != nullptr)
		pickedUpBlock->render();
	sprite->render();
	particleEfect->render();
}

void Player::pickUpBlock()
{
	pickedUpBlock = readyToPickBlock;
	readyToPickBlock = nullptr;
	CollisionManager::instance().disAttachBlock(pickedUpBlock);
	newState = B_PICK;
}

void Player::throwBlock()
{
	//collider atach block
	glm::vec2 dir = glm::vec2(4, -4);
	if (left)
		dir.x *= -1;
	pickedUpBlock->throwBlock(dir);
	CollisionManager::instance().attachBlock(pickedUpBlock);
	pickedUpBlock = nullptr;

	switch (oldState)
	{
	case B_IDLE:
		newState = IDLE;
		break;
	case B_WALK:
		newState = WALK;
		break;
	case B_JUMP:
		newState = JUMP;
		break;
	case B_FALL:
		newState = FALL;
		break;
	}
}

void Player::dropBlock()
{
	//pickedUpBlock->throwBlock(glm::vec2(0, -10));
	pickedUpBlock->dropBlock(position, sizeObject, left);
	CollisionManager::instance().attachBlock(pickedUpBlock);
	pickedUpBlock = nullptr;

	switch (oldState)
	{
	case B_IDLE:
		newState = IDLE;
		break;
	case B_WALK:
		newState = WALK;
		break;
	case B_JUMP:
		newState = JUMP;
		break;
	case B_FALL:
		newState = FALL;
		break;
	}
}

bool Player::checkJumpButton()
{
	if ((Game::instance().getKey(GLFW_KEY_K) || Game::instance().getKey(GLFW_KEY_W)) && jumpAvailable)
	{
		jumpAvailable = false;
		return true;
	}
	if (!Game::instance().getKey(GLFW_KEY_K) && !Game::instance().getKey(GLFW_KEY_W))
		jumpAvailable = true;
	return false;
}

bool Player::checkObjInteractionButton()
{
	if (Game::instance().getKey(GLFW_KEY_L)) {
		if (objInteractionAvailable)
		{
			objInteractionAvailable = false;
			return true;
		}
	}
	else
		objInteractionAvailable = true;
	return false;
}

bool Player::cakeCoinCollide(Block* b)
{
	if (b->getBlockType() == BlockType::Coin)
	{
		Game::instance().onGetCoin();
		return true;
	}
	else if (b->getBlockType() == BlockType::Cake)
	{
		Game::instance().onGetCake();
		return true;
	}
	return false;
}

void Player::playerButtJump()
{
	if (oldState == BUTT_FALL || oldState == BUTT_JUMP)
	{
		oldState = BUTT_JUMP;
		yAxisSpeed = BUTT_JUMP_SPEED;
		newState = BUTT_JUMP;
	}
}

void Player::horizontalMove(bool left)
{
	this->left = left;
	sprite->setLeft(left);
	int direction = 1;
	if (left)
		direction = -1;
	position.x += direction * WALK_SPEED;

	CollisionType colType = CollisionManager::instance().checkCollisionHorizontal(this);
	if (colType == CollisionType::Tile)
		position.x += direction * WALK_SPEED * -1;
	else if ((colType == CollisionType::Stairs || colType == CollisionType::TileStairs) && Game::instance().getKey(GLFW_KEY_W))
		changeToClimb();

	Block* b = CollisionManager::instance().collisionEntityBlockH(this);
	if (b != nullptr && b->getEntityState() != EntityState::DEAD)
	{
		if (!cakeCoinCollide(b))
		{
			position.x += direction * WALK_SPEED * -1;
			if (oldState == WALK || oldState == READY_TO_PICK)
			{
				newState = READY_TO_PICK;
				readyToPickBlock = b;
			}
		}
		else {
			b->explode();
			CollisionManager::instance().disAttachBlock(b);
		}
	}
}

void Player::changeToClimb()
{
	newState = CLIMB_IDLE;
	//calcular posicion de la escalera
}

void Player::changeToDodge() {
	newState = DODGE;
	sizeObject = DODGE_SIZE;
	offset = OFFSET + glm::ivec2(0, 11);
	position.y += 11;
}

bool Player::stopFallingCollision(Block*& block, CollisionType& colType)
{
	auto originalPos = position;
	colType = CollisionManager::instance().checkCollisionVertical(this);
	
	auto tileCorrectedPos = position;
	block = CollisionManager::instance().collisionEntityBlockV(this);

	if (originalPos != tileCorrectedPos && tileCorrectedPos != position)
		position = tileCorrectedPos;

	if (colType == CollisionType::Hole)
		Game::instance().onPlayerFallDownHole();

	if (colType == CollisionType::Tile || colType == CollisionType::TileStairs)
		return true;
	if (block != nullptr && block->getEntityState() != EntityState::DEAD)
	{
		if (!cakeCoinCollide(block))
			return true;
		else {
			block->explode();
			CollisionManager::instance().disAttachBlock(block);
		}
	}
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

void Player::movementBehaviour()
{
	if (stopFallingCollision(block, colType))
	{
		yAxisSpeed = 0;
		if (checkJumpButton()) {
			newState = JUMP;
			yAxisSpeed = JUMP_SPEED;
			return;
		}
		else if (Game::instance().getKey(GLFW_KEY_S)) {
			if (colType == CollisionType::TileStairs) {
				changeToClimb();
				//position.y++;
				return;
			}
			else {
				changeToDodge();
				return;
			}
				
			// change size, piensatelo: ???
		}
	}
	else
	{
		newState = FALL;
		return;
	}
	if (Game::instance().getKey(GLFW_KEY_A)) {
		newState = WALK;
		horizontalMove(true);
	}
	if (Game::instance().getKey(GLFW_KEY_D)) {
		newState = WALK;
		horizontalMove(false);
	}
}

void Player::climbBehaviour()
{
	position.y -= int(yAxisSpeed);
	yAxisSpeed = 0;

	if (!stairCollision())
	{
		newState = FALL;
	}
	else 
	{
		position.y--;
		if (Game::instance().getKey(GLFW_KEY_W)) {
			position.y -= WALK_SPEED;
			newState = CLIMB;
		}
		if (Game::instance().getKey(GLFW_KEY_S)) {
			position.y += WALK_SPEED;
			newState = CLIMB;
		}
		if (checkObjInteractionButton() && colType == CollisionType::Stairs) {
			newState = FALL;
		}
	}
}

void Player::buttJumpBehaviour()
{
	if (Game::instance().getKey(GLFW_KEY_A)) {
		horizontalMove(true);
	}
	if (Game::instance().getKey(GLFW_KEY_D)) {
		horizontalMove(false);
	}
	if (stopFallingCollision(block, colType))
	{
		if (yAxisSpeed > 0)
			newState = IDLE;
		yAxisSpeed = 0;
	}
	if (block != nullptr && block->getEntityState() != EntityState::DEAD)
	{
		yAxisSpeed = BUTT_JUMP_SPEED;
		newState = BUTT_JUMP;
		
		//cakeCoinCollide(block);
		block->explode();
		//CollisionManager::instance().disAttachBlock(block);
	}
}

void Player::blockMovementBehaviour()
{
	if (stopFallingCollision(block, colType))
	{
		yAxisSpeed = 0;
		if (checkJumpButton()) {
			newState = B_JUMP;
			yAxisSpeed = JUMP_SPEED;
			return;
		}
	}
	else
	{
		newState = B_FALL;
		return;
	}
	if (Game::instance().getKey(GLFW_KEY_A)) {
		newState = B_WALK;
		horizontalMove(true);
	}
	if (Game::instance().getKey(GLFW_KEY_D)) {
		newState = B_WALK;
		horizontalMove(false);
	}
	if (checkObjInteractionButton()) {
		if (Game::instance().getKey(GLFW_KEY_S)) {
			dropBlock();
		}
		else
			throwBlock();
	}
}
