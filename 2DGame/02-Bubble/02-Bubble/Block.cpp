#include "Block.h"

void Block::update(int deltaTime)
{
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
{}
void Block::collisionBlockHorizontal(HColType hBlockCollision, const Block*& b)
{}
void Block::collisionBlockVertical(VColType vBlockCollision, const Block*& b)
{}
void Block::collisionVertical(CollisionType verticalCollision)
{}
void Block::collisionHorizontal(CollisionType horizontalCollision)
{}