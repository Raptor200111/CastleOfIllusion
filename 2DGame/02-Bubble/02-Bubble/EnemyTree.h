#pragma once

#include "Sprite.h"
#include "TileMap.h"
#include <cmath>

enum EnemyTreeStates
{
	WALK_LEFT, WALK_RIGHT, DIE_LEFT, DIE_RIGHT
};

class EnemyTree
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, bool right);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	glm::ivec2 getEnemyTreePos() const { return posEnemyTree; }
private:
	glm::ivec2 tileMapDispl, posEnemyTree, sizeEnemyTree = glm::ivec2(24, 32), initPos;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	float velocity;
	bool right;

	EnemyTreeStates enemyTreeState = EnemyTreeStates::WALK_RIGHT;

};

