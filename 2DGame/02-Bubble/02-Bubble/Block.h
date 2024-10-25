#pragma once

#include "Entity.h"
#include <cmath>

class Block : public Entity
{
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) = 0;
	void update(int deltaTime);
	void render();
	BlockType getBlockType() { return blockType; }
	void collisionEnemy(const glm::ivec2& posEnemy);
	void collisionBlockHorizontal(HColType hBlockCollision, const Block*& b);
	void collisionBlockVertical(VColType vBlockCollision, const Block*& b);
	void collisionVertical(CollisionType verticalCollision);
	void collisionHorizontal(CollisionType horizontalCollision);

protected:
	BlockType blockType;
};

