#pragma once
#include "Block.h"
class BlockDestroyable: public Block
{
public:
	BlockDestroyable(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	//void explode();
};

