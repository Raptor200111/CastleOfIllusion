#include "EnemyTree.h"

#define WALK_SPEED 1
#define GRAVITY 0.5f

void EnemyTree::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, bool right)
{
	velocity = 0.f;
	initPos = tileMapPos;
	enemyTreeState = WALK_RIGHT;
	this->right = right;
	spritesheet.loadFromFile("images/tree.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizeEnemyTree, glm::vec2(0.25f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(WALK_LEFT, 8);
	sprite->addKeyframe(WALK_LEFT, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(WALK_LEFT, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(WALK_LEFT, glm::vec2(0.75f, 0.5f));

	sprite->setAnimationSpeed(DIE_LEFT, 8);
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(WALK_RIGHT, 8);
	sprite->addKeyframe(WALK_RIGHT, glm::vec2(0.f, 0.f)); //ToDo:Hay que girar el Sprite ya que no tenemos animacion left
	sprite->addKeyframe(WALK_RIGHT, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(WALK_RIGHT, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(DIE_RIGHT, 8);
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.75f, 0.f));
	
	if (right) {
		sprite->changeAnimation(WALK_RIGHT);
	}
	else {
		sprite->changeAnimation(WALK_LEFT);
	}
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemyTree.x), float(tileMapDispl.y + posEnemyTree.y)));

}
void EnemyTree::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (right)
	{
		enemyTreeState = WALK_RIGHT;
		posEnemyTree.x += WALK_SPEED;
	}
	else {	
		enemyTreeState = WALK_LEFT;
		posEnemyTree.x -= WALK_SPEED;

	}
	velocity += GRAVITY;
	posEnemyTree.y += int(velocity);
	if (map->collisionMoveDown(posEnemyTree, sizeEnemyTree, &posEnemyTree.y))
	{
		velocity = 0.f;
	}
	if (sprite->animation() != enemyTreeState)
		sprite->changeAnimation(enemyTreeState);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemyTree.x), float(tileMapDispl.y + posEnemyTree.y)));
}
void EnemyTree::render()
{
	sprite->render();
}

void EnemyTree::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}
void EnemyTree::setPosition(const glm::vec2& pos) 
{
	posEnemyTree = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemyTree.x), float(tileMapDispl.y + posEnemyTree.y)));
}

