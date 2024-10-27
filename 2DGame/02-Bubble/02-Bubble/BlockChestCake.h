#pragma once
#include "Block.h"

class BlockChestCake : public Block
{
public:
	BlockChestCake(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
};