#pragma once
#include "Block.h"

class BlockCoin : public Block
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override;
};

