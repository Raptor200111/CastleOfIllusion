#pragma once

#include "Entity.h"
#include <cmath>

class Block : public Entity
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int numBlock);
	void update(int deltaTime);
	void render();


private:

};

