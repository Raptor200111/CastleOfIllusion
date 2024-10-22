#ifndef _PARTICLEEFECT_INCLUDE
#define _PARTICLEEFECT_INCLUDE

#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Sprite.h"
#include "TileMap.h"

class ParticleEfect
{
public:
	void init(const glm::ivec2& tileMapPos, glm::ivec2& pos, glm::ivec2& siz, ShaderProgram& shaderProgram, string file, int frameRate, glm::vec2 weirdSize, glm::vec2* framesArray);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void play(const glm::vec2& pos);

private:
	glm::ivec2 tileMapDispl, position, size;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
};
#endif