#include "BossDragon.h"
#include "CollisionManager.h"
#include "Player.h"
#include <iostream>

#define WALK_SPEED 1
#define GRAVITY 0.5f
#define MAX_TIME_BW_SHOOTS = 24

BossDragon::BossDragon() {
	bodySprite = NULL;
	shoots = vector<BossShoot*>();
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
	entityState = Dead;
	states = { BOSS_LEFT, BOSS_LEFT_DOWN, BOSS_DOWN, BOSS_RIGHT_DOWN, BOSS_DOWN, BOSS_LEFT_DOWN };
	angleShoots = vector<vector<float>>{
		{185.f, 160.f, 140.f},
		{130.f, 110.f, 90.f},
		{85.f, 70.f, 35.f} };
	setBodyAnimations(shaderProgram);

	setHeadAnimations(shaderProgram);
	posBody = glm::ivec2(0, 0);
	setHeadSpritePos();
	for (int i = 0; i < MaxShoots; ++i) {
		BossShoot* s = new BossShoot();
		s->init(tileMapDispl, shaderProgram);
		s->setPosition(posHead);
		s->setTileMap(map);
		shoots.push_back(s);
	}

	bodySprite->setPosition(glm::vec2(float(tileMapDispl.x + posBody.x), float(tileMapDispl.y + posBody.y)));

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posHead.x), float(tileMapDispl.y + posHead.y)));

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
	sizeObjHead = glm::ivec2(39.f, 32.f);
	glm::vec2 sizeSpriteSheet = glm::vec2(sizeObjHead.x / totalSizeSpriteSheet.x, sizeObjHead.y / totalSizeSpriteSheet.y);
	glm::vec2 frame;
	sprite = Sprite::createSprite(sizeObjHead, sizeSpriteSheet, &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);
	sprite->setAnimationSpeed(BOSS_IDLE, 8);

	sizeObjHead = glm::vec2(39.f, 32.f);
	sprite->setAnimationSpeed(BOSS_LEFT, 8);
	sprite->addKeyframeDiffSize(BOSS_LEFT, glm::vec2(0.f, 0.f), sizeObjHead, sizeSpriteSheet);
	sprite->addKeyframeDiffSize(BOSS_IDLE, glm::vec2(0.f, 0.f), sizeObjHead, sizeSpriteSheet);

	sizeObjHead = glm::vec2(32.f, 32.f);
	sizeSpriteSheet = glm::vec2(sizeObjHead.x / totalSizeSpriteSheet.x, sizeObjHead.y / totalSizeSpriteSheet.y);
	sprite->setAnimationSpeed(BOSS_LEFT_DOWN, 8);
	sprite->addKeyframeDiffSize(BOSS_LEFT_DOWN, glm::vec2(53.f / totalSizeSpriteSheet.x, 0.f), sizeObjHead, sizeSpriteSheet);
	sprite->addKeyframeDiffSize(BOSS_IDLE, glm::vec2(53.f / totalSizeSpriteSheet.x, 0.f), sizeObjHead, sizeSpriteSheet);


	sizeObjHead = glm::vec2(32.f, 32.f);
	sizeSpriteSheet = glm::vec2(sizeObjHead.x / totalSizeSpriteSheet.x, sizeObjHead.y / totalSizeSpriteSheet.y);
	sprite->setAnimationSpeed(BOSS_RIGHT_DOWN, 8);
	sprite->addKeyframeDiffSize(BOSS_RIGHT_DOWN, glm::vec2(15.f / totalSizeSpriteSheet.x, 41.f / totalSizeSpriteSheet.y), sizeObjHead, sizeSpriteSheet);
	sprite->addKeyframeDiffSize(BOSS_IDLE, glm::vec2(15.f / totalSizeSpriteSheet.x, 41.f / totalSizeSpriteSheet.y), sizeObjHead, sizeSpriteSheet);

	sizeObjHead = glm::vec2(16.f, 32.f);
	sizeSpriteSheet = glm::vec2(sizeObjHead.x / totalSizeSpriteSheet.x, sizeObjHead.y / totalSizeSpriteSheet.y);
	sprite->setAnimationSpeed(BOSS_DOWN, 8);
	sprite->addKeyframeDiffSize(BOSS_DOWN, glm::vec2(62.f / totalSizeSpriteSheet.x, 41.f / totalSizeSpriteSheet.y), sizeObjHead, sizeSpriteSheet);
	sprite->addKeyframeDiffSize(BOSS_IDLE, glm::vec2(62.f / totalSizeSpriteSheet.x, 41.f / totalSizeSpriteSheet.y), sizeObjHead, sizeSpriteSheet);

	glm::vec2 aux = sprite->changeAnimationDiffSize(bossDragonState);
	if (aux != glm::vec2(0.f))
		sizeObjHead = aux;
}

void BossDragon::update(int deltaTime)
{
	if (entityState == Dead) return;
	glm::vec2 aux = sprite->updateDiffSize(deltaTime);
	if (aux != glm::vec2(0.f))
		sizeObjHead = aux;
	bodySprite->updateDiffSize(deltaTime);

	if (initParams.initPos.y > posBody.y) {
		posBody.y += 1;
		timeSinceLastStateChange += deltaTime;
		if (timeSinceLastStateChange >= stateChangeInterval) {
			state++;  // Increment state
			int index = state % states.size();
			bossDragonState = states[index];
			timeSinceLastStateChange -= stateChangeInterval;  // Reset the timer for state change
		}
	}
	else {
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
		// After 160ms, start the shooting phase
		else if (cycleTime < idleDuration + MaxShoots * shootInterval) {
			timeSinceLastShoot += deltaTime;

			// Shoot every moveInterval, up to 3 shoots
			if (timeSinceLastShoot >= shootInterval && shootCount < 3) {
				shoot(deltaTime);  // Move the player to the right
				shootCount++;   // Increment move counter
				timeSinceLastShoot -= shootInterval;  // Reset the timer for the next move
			}
		}
		// Reset cycle
		else {
			cycleTime = 0;               // Reset cycle time
			state = 0;                   // Reset state
			shootCount = 0;               // Reset move count
			timeSinceLastStateChange = 0; // Reset idle state change timer
			timeSinceLastShoot = 0;       // Reset movement timer
		}

	}
	if (bossDragonState != sprite->animation())
	{
		glm::vec2 aux = sprite->changeAnimationDiffSize(bossDragonState);
		if (aux != glm::vec2(0.f))
			sizeObjHead = aux;
	}
	bodySprite->setPosition(glm::vec2(float(tileMapDispl.x + posBody.x), float(tileMapDispl.y + posBody.y)));
	setHeadSpritePos();

}
void BossDragon::render()
{
	if (entityState == Dead) return;
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
		diffPos = glm::ivec2(18 - 18, y);
		sizeObject = glm::ivec2(22, 19);
		positionStartShoot = glm::ivec2(0, 13);
		break;
	case BOSS_LEFT_DOWN:
		diffPos = glm::ivec2(18 - 11, y);
		sizeObject = glm::ivec2(15, 24);
		positionStartShoot = glm::ivec2(3, 20);
		break;
	case BOSS_RIGHT_DOWN:
		diffPos = glm::ivec2(18 - 3, y);
		sizeObject = glm::ivec2(15, 24);
		positionStartShoot = glm::ivec2(21, 21);
		break;

	case BOSS_DOWN:
		diffPos = glm::ivec2(18 - 0, y);
		sizeObject = glm::ivec2(14, 30);
		positionStartShoot = glm::ivec2(0, 0);
		break;
	default:
		diffPos = glm::ivec2(0, y);
		sizeObject = glm::ivec2(22, 19);
		positionStartShoot = glm::ivec2(0, 13);
		break;
	}
	posHead = posBody + diffPos;
	position = posHead;
	positionStartShoot = posHead + positionStartShoot;
	if (BOSS_RIGHT_DOWN)
		position.x = 17;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posHead.x), float(tileMapDispl.y + posHead.y)));
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


void BossDragon::shoot(int deltaTime)
{
	glm::ivec2 direction = Player::instance().getPosition() - posHead;
	float angle = atan2(direction.y, direction.x);  // Invert y-axis since we're interested in the bottom half.
	float angleDegrees = glm::degrees(angle);

	if (angleDegrees < 0) angleDegrees += 180;  // Ensure the angle is positive
	BossDragonStates objective;
	if (angleDegrees >= 135 && angleDegrees < 180) {
		objective = BOSS_LEFT;
		indexQuadrantShoot = 0;
	}
	else if (angleDegrees >= 90 && angleDegrees < 135) {
		objective = BOSS_LEFT_DOWN;
		indexQuadrantShoot = 1;
	}
	else {
		//(angleDegrees0-90
		objective = BOSS_RIGHT_DOWN;
		indexQuadrantShoot = 2;
	}
	indexAngleShoot++;
	indexAngleShoot %= MaxShoots;
	//std::cout << "indexAngleShoot" << indexAngleShoot << "\n";
	changeHeadState(objective);
	setHeadSpritePos();
	shoots[indexAngleShoot]->setPosition(positionStartShoot);
	float a = angleShoots[indexQuadrantShoot][indexAngleShoot];
	a = glm::radians(a);
	glm::vec2 dir = glm::vec2(positionStartShoot.x + 350 * cos(a), positionStartShoot.y + 350 * sin(a));
	shoots[indexAngleShoot]->setDirection(dir);
	shoots[indexAngleShoot]->setEntityState(Alive);
}