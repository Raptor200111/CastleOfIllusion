#include "EnemyBee.h"

#include "CollisionManager.h"
#include "Player.h"

#define WALK_SPEED 1
#define GRAVITY 0.5f

void EnemyBee::initMov(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const ZoneEnemy& initParams)
{
	velocity = 0.f;
	tileMapDispl = tileMapPos;
	this->left = initParams.left;
	this->initParams = initParams;
	attackSpeed = WALK_SPEED;
	enemyType = Bee;
	spritesheet.loadFromFile("images/bee.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sizeObject = glm::ivec2(16.f, 17.f);
	sprite = Sprite::createSprite(sizeObject, glm::vec2( 1/6.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	glm::vec2 totalSizeSpriteSheet = glm::vec2(103.f, 17.f);
	sizeObject = glm::vec2(24.f, 17.f);
	sprite->setAnimationSpeed(BEE_FLY_RIGHT, 8);
	sprite->addKeyframe(BEE_FLY_RIGHT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(BEE_FLY_RIGHT, glm::vec2(17.f/totalSizeSpriteSheet.x, 0.f));
	sprite->addKeyframe(BEE_FLY_RIGHT, glm::vec2(34.f / totalSizeSpriteSheet.x, 0.f));
	sprite->addKeyframe(BEE_FLY_RIGHT, glm::vec2(70.f / totalSizeSpriteSheet.x, 0.f));
	sprite->addKeyframe(BEE_FLY_RIGHT, glm::vec2(87.f / totalSizeSpriteSheet.x, 0.f));

	sprite->setAnimationSpeed(BEE_DIE, 8);
	sprite->addKeyframe(BEE_DIE, glm::vec2(52.f/totalSizeSpriteSheet.x, 0.f));

	sprite->changeAnimation(enemyBeeState);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));

}
void EnemyBee::update(int deltaTime)
{
	sprite->update(deltaTime);

	elapsedTime += deltaTime;
	if (entityState == Dead)
	{
	}
	else if (entityState == Dying) {
		enemyBeeState = BEE_DIE;
		if (elapsedTime >= timeDyingAnim)
		{
			entityState = Dead;
			elapsedTime = 0;
			velocity += GRAVITY;
			position.y += int(velocity);
			moveHorizontal(left, attackSpeed);
		}
	}
	//!dying=animDying ended && !dead=alive
	else if (entityState == Alive) {
		elapsedTime = 0;
		int min_x_attack = position.x - attackDistance;
		int max_x_attack = position.x + attackDistance;
		int min_y_attack = position.y - attackDistance;
		int max_y_attack = position.y + attackDistance;
		glm::ivec2 posPlayer = Player::instance().getPosition();
		if (posPlayer.x < min_x_attack || position.x < posPlayer.x
			|| posPlayer.y < min_y_attack || max_y_attack < posPlayer.y) {
			attackSpeed = WALK_SPEED;
			enemyBeeState = BEE_FLY_RIGHT;
			moveHorizontal(left, WALK_SPEED);
			/*countY--;
			if (countY < 0) {
				moveY = moveY * -1;
				countY = 2;
			}
			position.y += moveY;*/
		}
		//player inside attack range rols x2_Speed
		else
		{
			enemyBeeState = BEE_FLY_RIGHT;
			glm::vec2 direction = glm::vec2(glm::vec2(posPlayer.x - position.x, 0.f));
			glm::vec2 normalizedDirection = glm::normalize(direction);
			glm::vec2 movement = normalizedDirection * 2.f;
			glm::ivec2 newPosition = position + glm::ivec2(glm::round(movement));
			position += glm::ivec2(glm::round(movement));
		}

		//if after mov enemy outside limits, turn around
		if (position.x < initParams.limit.min_x || initParams.limit.max_x < position.x) {
			enemyBeeState = BEE_FLY_RIGHT;
			position = initParams.initPos * map->getTileSize();
			left = initParams.left;
		}

	}
	sprite->setLeft(left);
	if (sprite->animation() != enemyBeeState) {
		sprite->changeAnimation(enemyBeeState);
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}
void EnemyBee::render()
{
	if (entityState != Dead) {
		sprite->render();
	}
}


void EnemyBee::collideVertical()
{
}

void EnemyBee::collisionBlockHorizontal(Block* b)
{
}

void EnemyBee::Damaged()
{
	entityState = Dying;
}