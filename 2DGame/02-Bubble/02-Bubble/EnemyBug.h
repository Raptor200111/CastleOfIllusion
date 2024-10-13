#pragma once

#include "Sprite.h"
#include "TileMap.h"
#include <cmath>

enum EnemyBugStates
{
	BUG_WALK_LEFT, BUG_WALK_RIGHT, BUG_ROLL_LEFT, BUG_ROLL_RIGHT, BUG_DIE
};

class EnemyBug
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, bool right);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	glm::ivec2 getEnemyBugPos() const { return posEnemyBug; }
private:
	glm::ivec2 tileMapDispl, posEnemyBug, sizeEnemyBug = glm::ivec2(24, 32), initPos;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	float velocity;
	bool right;

	EnemyBugStates enemyBugState = EnemyBugStates::BUG_WALK_RIGHT;
};

