#pragma once
#include "Sprite.h"
#include "TileMap.h"
#include <cmath>

class Entity
{
public:
	Entity() : sprite(nullptr), map(nullptr), tileMapDispl(0), position(0), sizeObject(0), offset(0) {}

	virtual ~Entity() { if (sprite) delete sprite; }

	
	glm::ivec2 getPosition() { return position; }
	glm::ivec2 getSize() { return sizeObject; }
	bool getLeft() { return left; }
	void setPositionY(int y) { position.y = y; }
	void setTileMap(TileMap* tileMap) { map = tileMap; }
	void setPosition(const glm::vec2& pos) {
		position = pos;
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x - offset.x), float(tileMapDispl.y + position.y - offset.y)));
	}
	void setSize(glm::ivec2 size) { sizeObject = size; }
	void setOffset(glm::ivec2 offset) { this->offset = offset; }

	EntityState getEntityState() { return entityState; }
	void setEntityState(EntityState s) { entityState = s; }

protected:
	EntityState entityState = Alive;
	bool left;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	glm::ivec2 tileMapDispl, position, sizeObject, sizeSprite, offset;

};

