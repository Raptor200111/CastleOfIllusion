#pragma once
#include "Block.h"

class BlockCoin : public Block
{
public:
	BlockCoin(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
};

