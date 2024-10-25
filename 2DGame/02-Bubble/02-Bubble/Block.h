#pragma once

#include "Entity.h"
#include <cmath>

class Block : public Entity
{
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) = 0;
	void update(int deltaTime);
	void render();


protected:
	BlockType blockType;
};

