#include "BossDragon.h"
#include "CollisionManager.h"
#define WALK_SPEED 1
#define GRAVITY 0.5f

void BossDragon::initMov(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const ZoneEnemy& initParams)
{
	velocity = 0.f;
	tileMapDispl = tileMapPos;
	this->left = initParams.left;
	this->initParams = initParams;
	attackSpeed = WALK_SPEED;

	setBodyAnimations(shaderProgram);
	//tots els bodys tenen corte cabeza pos 43
	
	setHeadAnimations(shaderProgram);
	
	tileMapDispl = tileMapPos;
	bodySprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));

}

void BossDragon::setBodyAnimations(ShaderProgram& shaderProgram)
{
	bodySpritesheet.loadFromFile("images/dragonBody.png", TEXTURE_PIXEL_FORMAT_RGBA);

	glm::vec2 totalSizeSpriteSheet = glm::vec2(225.f, 110.f);
	sizeObject = glm::ivec2(67.f, 110.f);
	glm::vec2 sizeSpriteSheet = glm::vec2(sizeObject.x / totalSizeSpriteSheet.x, sizeObject.y / totalSizeSpriteSheet.y);
	bodySprite = Sprite::createSprite(sizeObject, sizeSpriteSheet, &bodySpritesheet, &shaderProgram);
	bodySprite->setNumberAnimations(1);


	bodySprite->setAnimationSpeed(BOSS_BODY_IDLE, 8);
	sizeObject = glm::vec2(67.f, 110.f);
	sizeSpriteSheet = glm::vec2(sizeObject.x / totalSizeSpriteSheet.x, sizeObject.y / totalSizeSpriteSheet.y);
	bodySprite->addKeyframeDiffSize(BOSS_IDLE, glm::vec2(0.f, 0.f), sizeObject, sizeSpriteSheet);

	sizeObject = glm::vec2(63.f, 110.f);
	sizeSpriteSheet = glm::vec2(sizeObject.x / totalSizeSpriteSheet.x, sizeObject.y / totalSizeSpriteSheet.y);
	glm::vec2 frame = glm::vec2(76.f / sizeSpriteSheet.x, 0.f);
	bodySprite->addKeyframeDiffSize(BOSS_IDLE, frame, sizeObject, sizeSpriteSheet);

	sizeObject = glm::vec2(77.f, 110.f);
	sizeSpriteSheet = glm::vec2(sizeObject.x / totalSizeSpriteSheet.x, sizeObject.y / totalSizeSpriteSheet.y);
	frame = glm::vec2(148.f / sizeSpriteSheet.x, 0.f);
	bodySprite->addKeyframeDiffSize(BOSS_IDLE, frame, sizeObject, sizeSpriteSheet);

	bodySprite->changeAnimationDiffSize(bossBodyState);
}

void BossDragon::setHeadAnimations(ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/dragonHeads.png", TEXTURE_PIXEL_FORMAT_RGBA);

	glm::vec2 totalSizeSpriteSheet = glm::vec2(85.f, 73.f);
	sizeObject = glm::ivec2(39.f, 32.f);
	glm::vec2 sizeSpriteSheet = glm::vec2(sizeObject.x / totalSizeSpriteSheet.x, sizeObject.y / totalSizeSpriteSheet.y);
	sprite = Sprite::createSprite(sizeObject, sizeSpriteSheet, &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	glm::vec2 frame;
	sprite->setAnimationSpeed(BOSS_IDLE, 8);
	sprite->addKeyframeDiffSize(BOSS_IDLE, glm::vec2(0.f, 0.f), sizeObject, sizeSpriteSheet);
	//sprite->addKeyframeDiffSize(BOSS_IDLE, glm::vec2(sizeSpriteSheet.x, 0.f), sizeObject, sizeSpriteSheet);

	glm::vec2 aux = sprite->changeAnimationDiffSize(bossDragonState);
	if (aux != glm::vec2(0.f))
		sizeObject = aux;
}


void BossDragon::update(int deltaTime, const glm::ivec2& posPlayer)
{
	glm::vec2 aux = sprite->updateDiffSize(deltaTime);
	if (aux != glm::vec2(0.f))
		sizeObject = aux;
	bodySprite->updateDiffSize(deltaTime);
	int min_x_attack = position.x - attackDistance;
	int max_x_attack = position.x + attackDistance;

	if (posPlayer.x < min_x_attack || max_x_attack < posPlayer.x) {
		attackSpeed = WALK_SPEED;
		//bossDragonState = BOSS_ATTACK;
		moveHorizontal(left, WALK_SPEED);
	}
	//player inside attack range rols x2_Speed
	else
	{
		attackSpeed = 1;
		//bossDragonState = BOSS_MOVE_RIGHT;
		//change to dash
		moveHorizontal(left, attackSpeed);
	}

	//if after mov enemy outside limits, turn around
	if (position.x < initParams.limit.min_x || initParams.limit.max_x < position.x) {
		//add if collision tile or block (no stair nor ramp)
		//bossDragonState = BOSS_MOVE_RIGHT;
		left = !left;
		moveHorizontal(left, WALK_SPEED);
	}


	velocity += GRAVITY;
	position.y += int(velocity);

	for (auto block : CollisionManager::instance().blocks)
	{
		if (CollisionManager::instance().checkCollisionBlockVertical(this, block.second) == Down) {
			position.y = block.second->getPosition().y - sizeObject.y;
			velocity = 0.f;
		}
	}

	if (CollisionManager::instance().checkCollisionVertical(this) == Tile)
	{
		velocity = 0.f;
	}
	sprite->setLeft(left);
	bodySprite->setLeft(left);
	if (sprite->animation() != bossDragonState) {
		glm::vec2 aux = sprite->changeAnimationDiffSize(bossDragonState);
		if (aux != glm::vec2(0.f))
			sizeObject = aux;
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
	bodySprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));

}
void BossDragon::render()
{
	bodySprite->render(); 
	sprite->render();
}
