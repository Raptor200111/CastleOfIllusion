#include "EnemyBug.h"

#define WALK_SPEED 1
#define GRAVITY 0.5f

void EnemyBug::initMov(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const ZoneEnemy& initParams)
{
	velocity = 0.f;
	initPos = tileMapPos;
	this->left = initParams.left;
	this->initParams = initParams;
	spritesheet.loadFromFile("images/bug1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sizeEnemy = glm::ivec2(24.f, 32.f);
	sprite = Sprite::createSprite(sizeEnemy, glm::vec2(0.25f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	glm::vec2 totalSizeSpriteSheet = glm::vec2(115.f, 22.f);
	sizeEnemy = glm::vec2(24.f, 17.f);
	glm::vec2 sizeSpriteSheet = glm::vec2(sizeEnemy.x / totalSizeSpriteSheet.x, sizeEnemy.y / totalSizeSpriteSheet.y);
	sprite->setAnimationSpeed(BUG_WALK_RIGHT, 8);
	sprite->addKeyframeDiffSize(BUG_WALK_RIGHT, glm::vec2(0.f, 0.f), sizeEnemy, sizeSpriteSheet);
	sprite->addKeyframeDiffSize(BUG_WALK_RIGHT, glm::vec2(sizeSpriteSheet.x, 0.f), sizeEnemy, sizeSpriteSheet);

	sizeEnemy = glm::vec2(17.f, 17.f);
	sizeSpriteSheet = glm::vec2(sizeEnemy.x / totalSizeSpriteSheet.x, sizeEnemy.y / totalSizeSpriteSheet.y);
	sprite->setAnimationSpeed(BUG_ROLL_RIGHT, 8);
	sprite->addKeyframeDiffSize(BUG_ROLL_RIGHT, glm::vec2((47.f)/totalSizeSpriteSheet.x, 0.f), sizeEnemy, sizeSpriteSheet);
	sprite->addKeyframeDiffSize(BUG_ROLL_RIGHT, glm::vec2((47.f+17.f) / totalSizeSpriteSheet.x, 0.f), sizeEnemy, sizeSpriteSheet);
	sprite->addKeyframeDiffSize(BUG_ROLL_RIGHT, glm::vec2((47.f+17.f) / totalSizeSpriteSheet.x, 0.f), sizeEnemy, sizeSpriteSheet);


	sizeEnemy = glm::vec2(16.f, 22.f);
	sizeSpriteSheet = glm::vec2(sizeEnemy.x / totalSizeSpriteSheet.x, 1);
	sprite->setAnimationSpeed(BUG_DIE, 8);
	sprite->addKeyframeDiffSize(BUG_DIE, glm::vec2(1.f-sizeSpriteSheet.x, 0.f), sizeEnemy, sizeSpriteSheet);

	sprite->changeAnimationDiffSize(enemyBugState);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));

}
void EnemyBug::update(int deltaTime, const glm::ivec2& posPlayer)
{
	sprite->updateDiffSize(deltaTime);

	int min_x_attack = posEnemy.x - attackDistance;
	int max_x_attack = posEnemy.x + attackDistance;

	//player outside attack range walks		
	if (posPlayer.x< min_x_attack || max_x_attack < posPlayer.x) {
		enemyBugState = BUG_WALK_RIGHT;
		moveHorizontal(left, WALK_SPEED);
	}
	//player inside attack range rols x2_Speed
	else
	{
		attaking = true;
		enemyBugState = BUG_ROLL_RIGHT;
		//change to dash
		moveHorizontal(left, WALK_SPEED);
	}
	
	//if after mov enemy outside limits, turn around
	if (posEnemy.x < initParams.limit.min_x || initParams.limit.max_x < posEnemy.x) {
		attaking = false;
		enemyBugState = BUG_WALK_RIGHT;
		left = !left;
		moveHorizontal(left, WALK_SPEED);
	}


	velocity += GRAVITY;
	posEnemy.y += int(velocity);
	if (map->collisionMoveDown(posEnemy, sizeEnemy, &posEnemy.y))
	{
		velocity = 0.f;
	}
	sprite->setLeft(left);
	if (sprite->animation() != enemyBugState)
		sprite->changeAnimationDiffSize(enemyBugState);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}
void EnemyBug::render()
{
	sprite->render();
}
