#include "BossDragon.h"
#include "CollisionManager.h"
#include "Player.h"
#define WALK_SPEED 1
#define GRAVITY 0.5f
#define MAX_TIME_BW_SHOOTS = 24

BossDragon::BossDragon() {
	bodySprite =NULL;
	shoots = vector<BossShoot*>(3);
}
BossDragon::~BossDragon()
{
	if (bodySprite != NULL) 
		delete bodySprite;
	for (auto shoot : shoots)
	{
		if (shoot != NULL)
			delete shoot;
	}
}


void BossDragon::initMov(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const ZoneEnemy& initParams)
{
	velocity = 0.f;
	tileMapDispl = tileMapPos;
	this->left = initParams.left;
	this->initParams = initParams;
	states = { BOSS_LEFT, BOSS_LEFT_DOWN, BOSS_DOWN, BOSS_RIGHT_DOWN, BOSS_DOWN, BOSS_LEFT_DOWN };
	setBodyAnimations(shaderProgram);
	
	setHeadAnimations(shaderProgram);
	
	bodySprite->setPosition(glm::vec2(float(tileMapDispl.x + posBody.x), float(tileMapDispl.y + posBody.y)));

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));

}

void BossDragon::setBodyAnimations(ShaderProgram& shaderProgram)
{
	bodySpritesheet.loadFromFile("images/dragonBody.png", TEXTURE_PIXEL_FORMAT_RGBA);

	glm::vec2 totalSizeSpriteSheet = glm::vec2(225.f, 110.f);
	sizeObjBody = glm::ivec2(75.f, 110.f);
	glm::vec2 sizeSpriteSheet = glm::vec2(sizeObjBody.x / totalSizeSpriteSheet.x, 1);
	bodySprite = Sprite::createSprite(sizeObjBody, sizeSpriteSheet, &bodySpritesheet, &shaderProgram);
	bodySprite->setNumberAnimations(1);


	bodySprite->setAnimationSpeed(BOSS_BODY_IDLE, 8);
	
	sizeObjBody = glm::vec2(67.f, 110.f);
	sizeSpriteSheet = glm::vec2(sizeObjBody.x / totalSizeSpriteSheet.x, 1);
	bodySprite->addKeyframeDiffSize(BOSS_BODY_IDLE, glm::vec2(0.f, 0.f), sizeObjBody, sizeSpriteSheet);

	
	sizeObjBody = glm::vec2(64.f, 110.f);
	sizeSpriteSheet = glm::vec2(sizeObjBody.x / totalSizeSpriteSheet.x, 1);
	glm::vec2 frame = glm::vec2(76.f / totalSizeSpriteSheet.x, 0.f);
	bodySprite->addKeyframeDiffSize(BOSS_BODY_IDLE, frame, sizeObjBody, sizeSpriteSheet);

	sizeObjBody = glm::vec2(77.f, 110.f);
	sizeSpriteSheet = glm::vec2(sizeObjBody.x / totalSizeSpriteSheet.x, 1);
	frame = glm::vec2(148.f / totalSizeSpriteSheet.x, 0.f);
	bodySprite->addKeyframeDiffSize(BOSS_BODY_IDLE, frame, sizeObjBody, sizeSpriteSheet);

	
	glm::vec2 aux = bodySprite->changeAnimationDiffSize(bossBodyState);
	if (aux != glm::vec2(0.f))
		sizeObjBody = aux;
}

void BossDragon::setHeadAnimations(ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/dragonHeads.png", TEXTURE_PIXEL_FORMAT_RGBA);

	glm::vec2 totalSizeSpriteSheet = glm::vec2(85.f, 73.f);
	sizeObject = glm::ivec2(39.f, 32.f);
	glm::vec2 sizeSpriteSheet = glm::vec2(sizeObject.x / totalSizeSpriteSheet.x, sizeObject.y / totalSizeSpriteSheet.y);
	glm::vec2 frame;
	sprite = Sprite::createSprite(sizeObject, sizeSpriteSheet, &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);
	sprite->setAnimationSpeed(BOSS_IDLE, 8);

	sizeObject = glm::vec2(39.f, 32.f);
	sprite->setAnimationSpeed(BOSS_LEFT, 8);
	sprite->addKeyframeDiffSize(BOSS_LEFT, glm::vec2(0.f, 0.f), sizeObject, sizeSpriteSheet);
	sprite->addKeyframeDiffSize(BOSS_IDLE, glm::vec2(0.f, 0.f), sizeObject, sizeSpriteSheet);

	sizeObject = glm::vec2(32.f, 32.f);
	sizeSpriteSheet = glm::vec2(sizeObject.x / totalSizeSpriteSheet.x, sizeObject.y / totalSizeSpriteSheet.y);
	sprite->setAnimationSpeed(BOSS_LEFT_DOWN, 8);
	sprite->addKeyframeDiffSize(BOSS_LEFT_DOWN, glm::vec2(53.f/ totalSizeSpriteSheet.x, 0.f), sizeObject, sizeSpriteSheet);
	sprite->addKeyframeDiffSize(BOSS_IDLE, glm::vec2(53.f / totalSizeSpriteSheet.x, 0.f), sizeObject, sizeSpriteSheet);


	sizeObject = glm::vec2(32.f, 32.f);
	sizeSpriteSheet = glm::vec2(sizeObject.x / totalSizeSpriteSheet.x, sizeObject.y / totalSizeSpriteSheet.y);
	sprite->setAnimationSpeed(BOSS_RIGHT_DOWN, 8);
	sprite->addKeyframeDiffSize(BOSS_RIGHT_DOWN, glm::vec2(15.f / totalSizeSpriteSheet.x, 41.f / totalSizeSpriteSheet.y), sizeObject, sizeSpriteSheet);
	sprite->addKeyframeDiffSize(BOSS_IDLE, glm::vec2(15.f / totalSizeSpriteSheet.x, 41.f / totalSizeSpriteSheet.y), sizeObject, sizeSpriteSheet);

	sizeObject = glm::vec2(16.f, 32.f);
	sizeSpriteSheet = glm::vec2(sizeObject.x / totalSizeSpriteSheet.x, sizeObject.y / totalSizeSpriteSheet.y);
	sprite->setAnimationSpeed(BOSS_DOWN, 8);
	sprite->addKeyframeDiffSize(BOSS_DOWN, glm::vec2(62.f / totalSizeSpriteSheet.x, 41.f / totalSizeSpriteSheet.y), sizeObject, sizeSpriteSheet);
	sprite->addKeyframeDiffSize(BOSS_IDLE, glm::vec2(62.f / totalSizeSpriteSheet.x, 41.f / totalSizeSpriteSheet.y), sizeObject, sizeSpriteSheet);

	glm::vec2 aux = sprite->changeAnimationDiffSize(bossDragonState);
	if (aux != glm::vec2(0.f))
		sizeObject = aux;
}

void BossDragon::update(int deltaTime)
{
	glm::vec2 aux = sprite->updateDiffSize(deltaTime);
	if (aux != glm::vec2(0.f))
		sizeObject = aux;
	bodySprite->updateDiffSize(deltaTime);


	cycleTime += deltaTime;

	// If within the idle period (first 160 ms)
	if (cycleTime < idleDuration) {
		// Accumulate time since the last state change
		timeSinceLastStateChange += deltaTime;

		// Check if 16ms have passed to change the state
		if (timeSinceLastStateChange >= stateChangeInterval) {
			state++;  // Increment state
			int index = state % states.size();
			bossDragonState = states[index];
			timeSinceLastStateChange -= stateChangeInterval;  // Reset the timer for state change
		}
	}
	// After 160ms, start the moving phase (next 24 ms)
	else if (cycleTime < idleDuration + 3* moveInterval) {
		timeSinceLastShoot += deltaTime;

		// Move every 8ms, up to 3 moves
		if (timeSinceLastShoot >= moveInterval && shootCount < 3) {
			shoot();  // Move the player to the right
			shootCount++;   // Increment move counter
			timeSinceLastShoot -= moveInterval;  // Reset the timer for the next move
		}
	}
	// Reset cycle after 160ms idle + 24ms moving
	else {
		cycleTime = 0;               // Reset cycle time
		state = 0;                   // Reset state
		shootCount = 0;               // Reset move count
		timeSinceLastStateChange = 0; // Reset idle state change timer
		timeSinceLastShoot = 0;       // Reset movement timer
	}
	
	if (bossDragonState != sprite->animation())
	{
		glm::vec2 aux = sprite->changeAnimationDiffSize(bossDragonState);
		if (aux != glm::vec2(0.f))
			sizeObject = aux;
	}
	bodySprite->setPosition(glm::vec2(float(tileMapDispl.x + posBody.x), float(tileMapDispl.y + posBody.y)));
	setHeadSpritePos();

}
void BossDragon::render()
{
	
	bodySprite->render(); 
	sprite->render();
}

void BossDragon::setHeadSpritePos()
{
	glm::ivec2 diffPos = glm::ivec2(0);

	//body from 0 to neck at 18x 
	int y = 43 - 32;
	switch (bossDragonState) {

	case BOSS_LEFT:
		diffPos = glm::ivec2(18-18, y);
		break;
	case BOSS_LEFT_DOWN:
		diffPos = glm::ivec2(18-11, y);
		break;
	case BOSS_RIGHT_DOWN:
		diffPos = glm::ivec2(18-3, y);
		break;

	case BOSS_DOWN:
		diffPos = glm::ivec2(18-0, y);
		break;
	default:
		diffPos = glm::ivec2(0, y);
		break;
	}
	position = posBody + diffPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

void BossDragon::changeHeadState(BossDragonStates objective)
{
	int newIndexStates = -1;
	int statesSize = states.size();
	state = state % statesSize;
	int fin = state + statesSize;
	bool found = false;
	for (int i = state; i < fin; i++) {
		int index = i % statesSize;
		if (states[index] == objective) {
			newIndexStates = index;
			break;
		}
	}
	if (newIndexStates != state) {
		state = newIndexStates;
		bossDragonState = objective;
	}
}


void BossDragon::shoot()
{
	/*
	shoots[shootCount]->init(tileMapDispl, shader);
	shoots[shootCount]->setPosition();
	shoots[shootCount]->setTileMap(map);
	*/
	glm::ivec2 direction = Player::instance().getPosition() - position;
	float angle = atan2(-direction.y, direction.x);  // Invert y-axis since we're interested in the bottom half.
	float angleDegrees = glm::degrees(angle);
	if (angleDegrees < 0) angleDegrees += 180;  // Ensure the angle is positive
	BossDragonStates objective;
	if (angleDegrees >= 0 && angleDegrees < 45) {
		objective = BOSS_LEFT ;
	}
	else if (angleDegrees >= 45 && angleDegrees < 90) {
		objective = BOSS_LEFT_DOWN;
	}
	else {
		//if (angleDegrees >= 90 && angleDegrees < 135)
		objective = BOSS_RIGHT_DOWN;
	}
	changeHeadState(objective);

}