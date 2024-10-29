#pragma once
#include "Block.h"

class BlockCake : public Block
{
public:
	BlockCake(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
};