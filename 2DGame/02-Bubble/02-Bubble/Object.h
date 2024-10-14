#ifndef _OBJECT_INCLUDE
#define _OBJECT_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include <cmath>
#include "Player.h"

class Object
{
public:
	void init(const glm::ivec2& initialPos, const glm::ivec2& size, const string file, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

private:

	glm::ivec2 tileMapDispl, posObject, sizeObject;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	Player *player;
};

#endif // _OBJECT_INCLUDE


