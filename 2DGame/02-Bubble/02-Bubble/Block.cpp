#include "Block.h"
#include "CollisionManager.h"

void Block::update(int deltaTime)
{
	switch (state)
	{
	case STILL:
		break;
	case GRABBED:
		break;
	case FALLING:
		speed.y += 0.5;
		position += speed;
		break;
	default:
		break;
	}
	
	setPosition(position);
	sprite->update(deltaTime);
	if (sprite->animation() != blockStatus) {
		sprite->changeAnimation(blockStatus);
	}
}

void Block::render()
{
	sprite->render();
}

void Block::collisionEnemy(const glm::ivec2& posEnemy)
{
	explode();
	//destroy enemy
}
void Block::collisionBlockHorizontal(HColType hBlockCollision, const Block*& b)
{
	explode();
}
void Block::collisionBlockVertical(VColType vBlockCollision, const Block*& b)
{
	explode();
}
void Block::collisionVertical(CollisionType verticalCollision)
{
	explode();
}
void Block::collisionHorizontal(CollisionType horizontalCollision)
{
	explode();
}

void Block::throwBlock(glm::vec2 speed)
{
	this->speed = speed;
	state = FALLING;
}

void Block::grabbed()
{
	state = GRABBED;
}

void Block::explode()
{
	if (state == FALLING)
	{
		entityState = Dead;
		state = STILL;
		speed = glm::vec2(0, 0);
		//explode
	}
}