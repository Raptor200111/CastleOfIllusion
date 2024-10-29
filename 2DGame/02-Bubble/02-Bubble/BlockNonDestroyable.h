#pragma once
#include "Block.h"

class BlockNonDestroyable: public Block
{
public:
	BlockNonDestroyable(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void explode();
};

