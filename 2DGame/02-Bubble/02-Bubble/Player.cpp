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

	if (oldState == PlayerStates::BUTT_FALL) {
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
	}
	if (oldState == PlayerStates::BUTT_JUMP) {
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
			position.y ++;
		}
		if (block != nullptr)
		{
			yAxisSpeed = BUTT_JUMP_SPEED;
			newState = BUTT_JUMP;
			// destruccion del objeto que devuelve
			// delete b;
		}
	}
	if (oldState == PlayerStates::CLIMB_IDLE || oldState == PlayerStates::CLIMB) {
		yAxisSpeed = 0;
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
		
		if (stopFallingCollision(block, colType))
			newState = IDLE;
		else if (colType == CollisionType::None)
			newState == FALL;
	}
	if (oldState == PlayerStates::DODGE) {
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
	}
	if (oldState == PlayerStates::WALK) {
		bool movement = false;
		if (stopFallingCollision(block, colType))
		{
			yAxisSpeed = 0;
			if (Game::instance().getKey(GLFW_KEY_K)) {
				newState = JUMP;
				yAxisSpeed = JUMP_SPEED;
				movement = true;
			}
			else if (colType == CollisionType::TileStairs && Game::instance().getKey(GLFW_KEY_S))
			{
				changeToClimb();
				movement = true;
			}
		}
		else
		{
			newState = FALL;
			movement = true;
		}
			
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
		if (Game::instance().getKey(GLFW_KEY_S)) {
			newState = DODGE;
			//setSize(DODGE_SIZE); PIENSATELO
		}
	}
	if (oldState == PlayerStates::IDLE) {
		if (Game::instance().getKey(GLFW_KEY_A)) {
			newState = WALK;
			leftMove();
		}
		if (Game::instance().getKey(GLFW_KEY_D)) {
			newState = WALK;
			rightMove();
		}
	
		if (stopFallingCollision(block, colType))
		{
			yAxisSpeed = 0;
			if (Game::instance().getKey(GLFW_KEY_K)) {
				newState = JUMP;
				yAxisSpeed = JUMP_SPEED;
			}

			if (Game::instance().getKey(GLFW_KEY_S)) {
				if (colType == CollisionType::TileStairs)
					changeToClimb();
				else
					newState = DODGE;
				// change size, piensatelo: ???
			}
		}
		else
		{
			newState = FALL;
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
		if (stopFallingCollision(block, colType)) 
		{
			yAxisSpeed = 0;
			newState = IDLE;
		}

		if (colType == CollisionType::Stairs && Game::instance().getKey(GLFW_KEY_W))
		{
			changeToClimb();
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
		if ((stopFallingCollision(block, colType) && yAxisSpeed < 0) || yAxisSpeed > 0) 
		{
			yAxisSpeed = 0;
			newState = FALL;
		}

		if (colType == CollisionType::Stairs && Game::instance().getKey(GLFW_KEY_W))
		{
			changeToClimb();
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
	else if (colType == CollisionType::Stairs && Game::instance().getKey(GLFW_KEY_W))
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
	else if (colType == CollisionType::Stairs && Game::instance().getKey(GLFW_KEY_W))
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
	newState = CLIMB;
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