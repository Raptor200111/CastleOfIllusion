#include "EnemyTree.h"
#include "CollisionManager.h"
#include <iostream>
#define WALK_SPEED 1
#define GRAVITY 0.5f


void EnemyTree::initMov(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const ZoneEnemy& initParams)
{
	velocity = 0.f;
	tileMapDispl = tileMapPos;
	this->left = initParams.left;
	this->initParams = initParams;
	enemyTreeState = WALK_RIGHT;
	glm::vec2 sizeSpriteSheet = glm::vec2(1.f);
	glm::vec2 totalSizeSpriteSheet = glm::vec2(98.f, 32.f);

	spritesheet.loadFromFile("images/tree0.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sizeObject = glm::ivec2(24, 32);
	sprite = Sprite::createSprite(sizeObject, glm::vec2(0.25f, 1.f), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(2);
	
	sprite->setAnimationSpeed(WALK_RIGHT, 8);
	
	sizeObject = glm::ivec2(23, 32);
	sizeSpriteSheet.x = sizeObject.x / totalSizeSpriteSheet.x;
	sprite->addKeyframeDiffSize(WALK_RIGHT, glm::vec2(0.f, 0.f), sizeObject, sizeSpriteSheet);

	sprite->addKeyframeDiffSize(WALK_RIGHT, glm::vec2(24.f/ totalSizeSpriteSheet.x, 0.f), sizeObject, sizeSpriteSheet);

	sizeObject = glm::ivec2(24, 32);
	sizeSpriteSheet.x = sizeObject.x / totalSizeSpriteSheet.x;
	sprite->addKeyframeDiffSize(WALK_RIGHT, glm::vec2(48.f/ totalSizeSpriteSheet.x, 0.f), sizeObject, sizeSpriteSheet);

	sprite->setAnimationSpeed(DIE_RIGHT, 8);
	sprite->addKeyframeDiffSize(DIE_RIGHT, glm::vec2(totalSizeSpriteSheet.x-sizeObject.x, 0.f), sizeObject, sizeSpriteSheet);

	
	glm::vec2 aux = sprite->changeAnimationDiffSize(enemyTreeState);
	if (aux != glm::vec2(0.f))
		sizeObject = aux;

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

//if enemy is dying activate die sequence that lasts x time
//		start counting until timeDyingAnim
//once dying seq ends, set dead to true;
//if enemy tree and dead, regenerates after x time
		//start counting until timeToRegenerate
void EnemyTree::update(int deltaTime)
{
	glm::vec2 aux = sprite->updateDiffSize(deltaTime);
	if (aux != glm::vec2(0.f))
		sizeObject = aux;
	elapsedTime += deltaTime;
	if (entityState == Dying) {
		enemyTreeState = DIE_RIGHT;
		if (elapsedTime >= timeDyingAnim)
		{
			entityState = Dead;
			elapsedTime = 0;
		}
	}
	else if (entityState == Dead) {
		if (elapsedTime >= timeToRegenerate)
		{
			entityState = Alive;
			elapsedTime = 0;
			position = initParams.initPos * map->getTileSize();
			left = initParams.left;
		}
	}
	else {
		elapsedTime = 0;
		enemyTreeState = WALK_RIGHT;
		moveHorizontal(left, WALK_SPEED);

		//change: if enemy outside screen start waiting period to restart
		//if outside limit
		if (position.x < initParams.limit.min_x || initParams.limit.max_x < position.x) {
			position = initParams.initPos* map->getTileSize();
			left = initParams.left;
		}

		velocity += GRAVITY;
		position.y += int(velocity);
	}
	sprite->setLeft(left);
	if (sprite->animation() != enemyTreeState) {
		glm::vec2 aux = sprite->changeAnimationDiffSize(enemyTreeState);
		if (aux != glm::vec2(0.f))
			sizeObject = aux;
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

void EnemyTree::render()
{
	if (entityState != Dead) {
		sprite->render();
	}
}

void EnemyTree::collideVertical() {
	velocity = 0;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

void EnemyTree::collisionBlockHorizontal(Block* b)
{
	//trees can jump over blocks;
	position.y = b->getPosition().y - sizeObject.y;
	sprite->setLeft(left);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));	
}

void EnemyTree::Damaged()
{
	entityState = Dying;
}