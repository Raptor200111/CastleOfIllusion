#pragma once
#include "Block.h"

class BlockChestCoin : public Block
{
public:
	BlockChestCoin(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void explode();
	void collisionVertical(CollisionType verticalCollision);

};

