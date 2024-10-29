#pragma once
#include "Block.h"

class BlockGem: public Block
{
public:
	BlockGem(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void explode();
};

