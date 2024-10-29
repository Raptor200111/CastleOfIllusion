#include "Block.h"
#include "CollisionManager.h"

Block::~Block()
{
	if (explosionEfect != NULL)
		delete explosionEfect;
}

Block::Block(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) : Entity()
{
	explosionEfect = new ParticleEfect(tileMapPos, position, glm::ivec2(32, 36), shaderProgram, "images/explosion.png", glm::vec2(0.333, 0.5), 1);

	glm::vec2* vec2Array = new glm::vec2[6];
	vec2Array[0] = glm::vec2(0.f, 0.f);
	vec2Array[1] = glm::vec2(0.333f * 3, 0.f);
	vec2Array[2] = glm::vec2(0.666f * 4, 0.f);
	vec2Array[3] = glm::vec2(0.f * 5, 0.5f);
	vec2Array[4] = glm::vec2(0.333f * 6, 0.5f);
	vec2Array[5] = glm::vec2(0.666f * 7, 0.5f);

	explosionEfect->addAnimation(15, vec2Array, 6);
	delete vec2Array;
}

void Block::update(int deltaTime)
{	
	switch (entityState)
	{
		case ALIVE:
		{
			speed.y += 0.5;
			position += speed;
			break;
		}
		case DEAD:
		{
			break;
		}
		case DYING:
		{
			elapsedTime += deltaTime;
			if (elapsedTime >= timeDyingAnim)
			{
				elapsedTime = 0;
				entityState = DEAD;
			}
			break;
		}
	}
	setPosition(position);
	sprite->update(deltaTime);
	/*if (sprite->animation() != blockStatus) {
		sprite->changeAnimation(blockStatus);
	}*/
	explosionEfect->update(deltaTime);
}

void Block::render()
{
	sprite->render();
	explosionEfect->render();
}

void Block::collisionEnemy(const glm::ivec2& posEnemy)
{
	if (entityState == ALIVE)
	{
		explode();
	}
}
void Block::collisionBlockHorizontal(HColType hBlockCollision, const Block*& b)
{
	if (entityState == ALIVE)
	{
		explode();
	}
}
void Block::collisionBlockVertical(VColType vBlockCollision, const Block*& b)
{
	if (entityState == ALIVE)
	{
		explode();
	}
}
void Block::collisionVertical(CollisionType verticalCollision)
{
	if (entityState == ALIVE)
	{
		explode();
	}
}
void Block::collisionHorizontal(CollisionType horizontalCollision)
{
	if (entityState == ALIVE)
	{
		explode();
	}
}

void Block::throwBlock(glm::vec2 speed)
{
	this->speed = speed;
	entityState = ALIVE;
}

void Block::dropBlock(glm::ivec2 pos, glm::ivec2 size, bool left)
{
	if(left)
		position = pos + glm::ivec2(0, size.y) - sizeObject;
	else
		position = pos + size - glm::ivec2(0, sizeObject.y);

	speed = glm::vec2(0, 0);
	entityState = ALIVE;
}

void Block::explode()
{
	entityState = DYING;
	speed = glm::vec2(0, 0);
	explosionEfect->play(position + (getSize()/2) - glm::ivec2(16, 18), 0);
}