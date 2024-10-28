#ifndef _PARTICLEEFECT_INCLUDE
#define _PARTICLEEFECT_INCLUDE

#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Sprite.h"
#include "TileMap.h"
#include "Entity.h"

class ParticleEfect : public Entity
{
public:
	ParticleEfect(const glm::ivec2& tileMapPos, glm::ivec2& pos, glm::ivec2& siz, ShaderProgram& shaderProgram, string file, glm::vec2 weirdSize, int numAnims);
	void update(int deltaTime);
	void render();

	void addAnimation(int frameRate, glm::vec2* framesArray, int length);
	void play(const glm::vec2& pos, int anim);

private:
	//glm::ivec2 tileMapDispl, position, size;
	int numAnims;
};
#endif