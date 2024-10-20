#include "EnemyBug.h"
#include "CollisionManager.h"
#define WALK_SPEED 1
#define GRAVITY 0.5f

void EnemyBug::initMov(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const ZoneEnemy& initParams)
{
	velocity = 0.f;
	tileMapDispl = tileMapPos;
	this->left = initParams.left;
	this->initParams = initParams;
	attackSpeed = WALK_SPEED;
	spritesheet.loadFromFile("images/bug1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sizeObject = glm::ivec2(24.f, 32.f);
	sprite = Sprite::createSprite(sizeObject, glm::vec2(0.25f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	glm::vec2 totalSizeSpriteSheet = glm::vec2(115.f, 22.f);
	sizeObject = glm::vec2(24.f, 17.f);
	glm::vec2 sizeSpriteSheet = glm::vec2(sizeObject.x / totalSizeSpriteSheet.x, sizeObject.y / totalSizeSpriteSheet.y);
	sprite->setAnimationSpeed(BUG_WALK_RIGHT, 8);
	sprite->addKeyframeDiffSize(BUG_WALK_RIGHT, glm::vec2(0.f, 0.f), sizeObject, sizeSpriteSheet);
	sprite->addKeyframeDiffSize(BUG_WALK_RIGHT, glm::vec2(sizeSpriteSheet.x, 0.f), sizeObject, sizeSpriteSheet);

	sizeObject = glm::vec2(17.f, 17.f);
	sizeSpriteSheet = glm::vec2(sizeObject.x / totalSizeSpriteSheet.x, sizeObject.y / totalSizeSpriteSheet.y);
	sprite->setAnimationSpeed(BUG_ROLL_RIGHT, 8);
	sprite->addKeyframeDiffSize(BUG_ROLL_RIGHT, glm::vec2((47.f)/totalSizeSpriteSheet.x, 0.f), sizeObject, sizeSpriteSheet);
	sprite->addKeyframeDiffSize(BUG_ROLL_RIGHT, glm::vec2((47.f+17.f) / totalSizeSpriteSheet.x, 0.f), sizeObject, sizeSpriteSheet);
	sprite->addKeyframeDiffSize(BUG_ROLL_RIGHT, glm::vec2((47.f+17.f) / totalSizeSpriteSheet.x, 0.f), sizeObject, sizeSpriteSheet);


	sizeObject = glm::vec2(16.f, 22.f);
	sizeSpriteSheet = glm::vec2(sizeObject.x / totalSizeSpriteSheet.x, 1);
	sprite->setAnimationSpeed(BUG_DIE, 8);
	sprite->addKeyframeDiffSize(BUG_DIE, glm::vec2(1.f-sizeSpriteSheet.x, 0.f), sizeObject, sizeSpriteSheet);

	glm::vec2 aux = sprite->changeAnimationDiffSize(enemyBugState);
	if (aux != glm::vec2(0.f))
		sizeObject = aux;
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));

}
void EnemyBug::update(int deltaTime, const glm::ivec2& posPlayer)
{
	glm::vec2 aux = sprite->updateDiffSize(deltaTime);
	if (aux != glm::vec2(0.f))
		sizeObject = aux;
	int min_x_attack = position.x - attackDistance;
	int max_x_attack = position.x + attackDistance;

	if (posPlayer.x< min_x_attack || max_x_attack < posPlayer.x) {
		attackSpeed = WALK_SPEED;
		enemyBugState = BUG_WALK_RIGHT;
		moveHorizontal(left, WALK_SPEED);
	}
	//player inside attack range rols x2_Speed
	else
	{
		attackSpeed =1;
		enemyBugState = BUG_ROLL_RIGHT;
		//change to dash
		moveHorizontal(left, attackSpeed);
	}
	
	//if after mov enemy outside limits, turn around
	if (position.x < initParams.limit.min_x || initParams.limit.max_x < position.x) {
		//add if collision tile or block (no stair nor ramp)
		enemyBugState = BUG_WALK_RIGHT;
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
		if (CollisionManager::instance().checkCollisionBlockHorizontal(this, block.second) != NoHcol) {
			left = !left;
			moveHorizontal(left, WALK_SPEED);
		}
	}

	if (CollisionManager::instance().checkCollisionVertical(this) == Tile)
	{
		velocity = 0.f;
	}
	sprite->setLeft(left);
	if (sprite->animation() != enemyBugState) {
		glm::vec2 aux = sprite->changeAnimationDiffSize(enemyBugState);
		if (aux != glm::vec2(0.f))
			sizeObject = aux;
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}
void EnemyBug::render()
{
	sprite->render();
}