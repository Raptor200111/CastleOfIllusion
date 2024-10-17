#include "EnemyTree.h"

#define WALK_SPEED 1
#define GRAVITY 0.5f


void EnemyTree::initMov(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const ZoneEnemy& initParams)
{
	velocity = 0.f;
	initPos = tileMapPos;
	this->left = initParams.left;
	this->initParams = initParams;
	enemyTreeState = WALK_RIGHT;

	spritesheet.loadFromFile("images/tree0.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sizeEnemy = glm::ivec2(24, 32);
	sprite = Sprite::createSprite(sizeEnemy, glm::vec2(0.25f, 1.f), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(WALK_RIGHT, 8);
	sprite->addKeyframe(WALK_RIGHT, glm::vec2(0.f, 0.f)); //ToDo:Hay que girar el Sprite ya que no tenemos animacion left
	sprite->addKeyframe(WALK_RIGHT, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(WALK_RIGHT, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(DIE_RIGHT, 8);
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.75f, 0.f));

	
	sprite->changeAnimation(enemyTreeState);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void EnemyTree::update(int deltaTime)
{
	sprite->update(deltaTime);
	enemyTreeState = WALK_RIGHT;
	moveHorizontal(left, WALK_SPEED);
	//if outside limit
	if (posEnemy.x < initParams.limit.min_x || initParams.limit.max_x < posEnemy.x) {
		posEnemy = initParams.initPos* map->getTileSize();
	}
	velocity += GRAVITY;
	posEnemy.y += int(velocity);
	if (map->collisionMoveDown(posEnemy, sizeEnemy, &posEnemy.y))
	{
		velocity = 0.f;
	}
	sprite->setLeft(left);
	if (sprite->animation() != enemyTreeState)
		sprite->changeAnimation(enemyTreeState);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}
void EnemyTree::render()
{
	sprite->render();
}

