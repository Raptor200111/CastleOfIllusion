#include "EnemyBug.h"

#define WALK_SPEED 1
#define GRAVITY 0.5f

void EnemyBug::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, bool right)
{
	velocity = 0.f;
	initPos = tileMapPos;
	enemyBugState = BUG_WALK_RIGHT;
	this->right = right;
	spritesheet.loadFromFile("images/bug1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizeEnemyBug, glm::vec2(0.25f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);

	glm::vec2 totalSizeSpriteSheet = glm::vec2(115.f, 34.f);
	sizeEnemyBug = glm::vec2(24.f, 17.f);
	glm::vec2 sizeSpriteSheet = glm::vec2(sizeEnemyBug.x / totalSizeSpriteSheet.x, 0.5);
	sprite->setAnimationSpeed(BUG_WALK_LEFT, 8);
	sprite->addKeyframeDiffSize(BUG_WALK_LEFT, glm::vec2(0.f, 0.f), sizeEnemyBug, sizeSpriteSheet);
	sprite->addKeyframeDiffSize(BUG_WALK_LEFT, glm::vec2(sizeSpriteSheet.x, 0.f), sizeEnemyBug, sizeSpriteSheet);

	sprite->setAnimationSpeed(BUG_WALK_RIGHT, 8);
	sprite->addKeyframeDiffSize(BUG_WALK_RIGHT, glm::vec2(0.f, 0.5f), sizeEnemyBug, sizeSpriteSheet); //ToDo:Hay que girar el Sprite ya que no tenemos animacion left
	sprite->addKeyframeDiffSize(BUG_WALK_RIGHT, glm::vec2(sizeSpriteSheet.x, 0.5f), sizeEnemyBug, sizeSpriteSheet);

	sizeEnemyBug = glm::vec2(17.f, 17.f);
	sizeSpriteSheet = glm::vec2(sizeEnemyBug.x / totalSizeSpriteSheet.x, sizeEnemyBug.y / totalSizeSpriteSheet.y);
	sprite->setAnimationSpeed(BUG_ROLL_LEFT, 8);
	sprite->addKeyframeDiffSize(BUG_ROLL_LEFT, glm::vec2((47.f)/totalSizeSpriteSheet.x, 0.f), sizeEnemyBug, sizeSpriteSheet);
	sprite->addKeyframeDiffSize(BUG_ROLL_LEFT, glm::vec2((47.f+17.f) / totalSizeSpriteSheet.x, 0.f), sizeEnemyBug, sizeSpriteSheet);
	sprite->addKeyframeDiffSize(BUG_ROLL_LEFT, glm::vec2((47.f+17.f) / totalSizeSpriteSheet.x, 0.f), sizeEnemyBug, sizeSpriteSheet);

	sprite->setAnimationSpeed(BUG_ROLL_RIGHT, 8);
	sprite->addKeyframeDiffSize(BUG_ROLL_RIGHT, glm::vec2((48.f) / totalSizeSpriteSheet.x, 0.5), sizeEnemyBug, sizeSpriteSheet);
	sprite->addKeyframeDiffSize(BUG_ROLL_RIGHT, glm::vec2((48.f + 17.f) / totalSizeSpriteSheet.x, 0.5f), sizeEnemyBug, sizeSpriteSheet);
	sprite->addKeyframeDiffSize(BUG_ROLL_RIGHT, glm::vec2((48.f + 17.f) / totalSizeSpriteSheet.x, 0.5f), sizeEnemyBug, sizeSpriteSheet);

	sizeEnemyBug = glm::vec2(16.f, 22.f);
	sizeSpriteSheet = glm::vec2(sizeEnemyBug.x / totalSizeSpriteSheet.x, sizeEnemyBug.y / totalSizeSpriteSheet.y);
	sprite->setAnimationSpeed(BUG_DIE, 8);
	sprite->addKeyframeDiffSize(BUG_DIE, glm::vec2(1.f-sizeSpriteSheet.x, 0.f), sizeEnemyBug, sizeSpriteSheet);
	
	if (right) {
		sprite->changeAnimationDiffSize(BUG_WALK_RIGHT);
	}
	else {
		sprite->changeAnimationDiffSize(BUG_WALK_LEFT);
	}
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemyBug.x), float(tileMapDispl.y + posEnemyBug.y)));

}
void EnemyBug::update(int deltaTime)
{
	sprite->updateDiffSize(deltaTime);
	if (right)
	{
		enemyBugState = BUG_WALK_RIGHT;
		posEnemyBug.x += WALK_SPEED;
	}
	else {
		enemyBugState = BUG_DIE;
		posEnemyBug.x -= WALK_SPEED;

	}
	velocity += GRAVITY;
	posEnemyBug.y += int(velocity);
	if (map->collisionMoveDown(posEnemyBug, sizeEnemyBug, &posEnemyBug.y))
	{
		velocity = 0.f;
	}
	if (sprite->animation() != enemyBugState)
		sprite->changeAnimationDiffSize(enemyBugState);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemyBug.x), float(tileMapDispl.y + posEnemyBug.y)));
}
void EnemyBug::render()
{
	sprite->render();
}

void EnemyBug::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}
void EnemyBug::setPosition(const glm::vec2& pos)
{
	posEnemyBug = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemyBug.x), float(tileMapDispl.y + posEnemyBug.y)));
}
