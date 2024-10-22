#include "EnemyTree.h"
#include "CollisionManager.h"

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

void EnemyTree::update(int deltaTime)
{
	glm::vec2 aux = sprite->updateDiffSize(deltaTime);
	if (aux != glm::vec2(0.f))
		sizeObject = aux;
	enemyTreeState = WALK_RIGHT;
	moveHorizontal(left, WALK_SPEED);
	//if outside limit
	if (position.x < initParams.limit.min_x || initParams.limit.max_x < position.x) {
		position = initParams.initPos* map->getTileSize();
	}

	velocity += GRAVITY;
	position.y += int(velocity);
	for (auto block : CollisionManager::instance().blocks)
	{
		if (CollisionManager::instance().checkCollisionBlockVertical(this, block.second) == Down) {
			position.y = block.second->getPosition().y - sizeObject.y;
			velocity = 0.f;
		}
		if (CollisionManager::instance().checkCollisionBlockHorizontal(this, block.second) != NoHcol) {
			position = initParams.initPos;
			left = initParams.left;
			moveHorizontal(left, WALK_SPEED);
		}	
	}
	if (CollisionManager::instance().checkCollisionVertical(this) == Tile)
	{
		velocity = 0.f;
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
	sprite->render();
}