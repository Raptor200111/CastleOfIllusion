#pragma once

#include "Sprite.h"
#include "TileMap.h"
#include <cmath>

class Block
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int numBlock);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	glm::ivec2 getBlockPos() const;

private:
	glm::ivec2 tileMapDispl, posBlock, sizeBlock = glm::ivec2(16, 16);
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
};

