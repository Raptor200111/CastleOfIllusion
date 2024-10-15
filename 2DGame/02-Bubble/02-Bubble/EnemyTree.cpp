#include "EnemyTree.h"

#define WALK_SPEED 1
#define GRAVITY 0.5f


void EnemyTree::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) { //, bool right
	this->right = false;
	velocity = 0.f;
	initPos = tileMapPos;
	enemyTreeState = WALK_RIGHT;

	spritesheet.loadFromFile("images/tree.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sizeEnemy = glm::ivec2(24, 32);
	sprite = Sprite::createSprite(sizeEnemy, glm::vec2(0.25f, 0.5f), &spritesheet, &shaderProgram);

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
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void EnemyTree::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (right)
	{
		enemyTreeState = WALK_RIGHT;
		posEnemy.x += WALK_SPEED;
	}
	else {	
		enemyTreeState = WALK_LEFT;
		posEnemy.x -= WALK_SPEED;

	}
	velocity += GRAVITY;
	posEnemy.y += int(velocity);
	if (map->collisionMoveDown(posEnemy, sizeEnemy, &posEnemy.y))
	{
		velocity = 0.f;
	}
	if (sprite->animation() != enemyTreeState)
		sprite->changeAnimation(enemyTreeState);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}
void EnemyTree::render()
{
	sprite->render();
}

