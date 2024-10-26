#pragma once
#include "Block.h"

class BlockChestCake : public Block
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override;
protected:
};