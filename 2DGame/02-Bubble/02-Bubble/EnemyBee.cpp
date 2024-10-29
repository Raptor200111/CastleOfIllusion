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
	timeDyingAnim = 800;
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
	if (entityState == DEAD)
	{
	}
	else if (entityState == DYING) {
		enemyBeeState = BEE_DIE;
		if (elapsedTime >= timeDyingAnim)
		{
			entityState = DEAD;
			elapsedTime = 0;
			velocity += GRAVITY;
			position.y += int(velocity);
			moveHorizontal(left, attackSpeed);
		}
	}
	//!dying=animDying ended && !dead=alive
	else if (entityState == STILL) {
		elapsedTime = 0;
		int min_x_attack = position.x - attackDistance;
		int max_x_attack = position.x + attackDistance;
		int min_y_attack = position.y - attackDistance;
		int max_y_attack = position.y + attackDistance;
		glm::ivec2 posPlayer = Player::instance().getPosition();
		bool playerInFront = left && posPlayer.x < position.x || !left && position.x < posPlayer.x;
		if (posPlayer.x < min_x_attack || max_x_attack < posPlayer.x || !playerInFront
			|| posPlayer.y < min_y_attack || max_y_attack < posPlayer.y) {
			attackSpeed = WALK_SPEED;
			enemyBeeState = BEE_FLY_RIGHT;
			moveHorizontal(left, WALK_SPEED);
			moveY += 1;
			moveY %= countY;
			if (moveY < countY/2)
				position.y -= 1;
			else
				position.y += 1;
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
			//glm::vec2 direction = glm::vec2(glm::vec2(posPlayer.x - position.x, 0.f));
			glm::vec2 direction = glm::vec2(posPlayer - position);
			glm::vec2 normalizedDirection = glm::normalize(direction);
			glm::vec2 movement = glm::vec2(normalizedDirection.x * 2.f, normalizedDirection.y*1.f);
			glm::ivec2 newPosition = position + glm::ivec2(glm::round(movement));
			position += glm::ivec2(glm::round(movement));
		}

		//if after mov enemy outside limits, turn around
		if (position.x <= initParams.limit.min_x || initParams.limit.max_x <= position.x) {
			enemyBeeState = BEE_FLY_RIGHT;
			int tilesize = map->getTileSize();
			if(left)
				position = glm::ivec2(initParams.limit.max_x, initParams.initPos.y * tilesize);
			else
				position = glm::ivec2(initParams.limit.min_x, initParams.initPos.y * tilesize);
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
	if (entityState != DEAD) {
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
	entityState = DYING;
}


void EnemyBee::reLive()
{
	enemyBeeState = BEE_FLY_RIGHT;
	entityState = EntityState::STILL;
	left = initParams.left;
	if (left)
		position = glm::ivec2(initParams.limit.max_x, initParams.initPos.y * map->getTileSize());
	else
		position = glm::ivec2(initParams.limit.min_x, initParams.initPos.y * map->getTileSize());
	sprite->setLeft(left);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}
