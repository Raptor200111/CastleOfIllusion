#pragma once
#include "Block.h"

class BlockNonDestroyable: public Block
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override;
};

