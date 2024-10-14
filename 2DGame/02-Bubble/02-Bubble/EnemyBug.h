#pragma once

#include "Sprite.h"
#include "TileMap.h"
#include <cmath>
#include "Defs.h"

enum EnemyBugStates
{
	BUG_WALK_LEFT, BUG_WALK_RIGHT, BUG_ROLL_LEFT, BUG_ROLL_RIGHT, BUG_DIE
};

class EnemyBug
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const Zone& limit);
	void update(int deltaTime, const glm::ivec2& posPlayer);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	glm::ivec2 getEnemyBugPos() const { return posEnemyBug; }
private:
	glm::ivec2 tileMapDispl, posEnemyBug, sizeEnemyBug = glm::ivec2(24, 32), initPos;
	Zone limit;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	float velocity;
	bool right;
	int attackDistance = 5*16;//5*mapTileSize
	bool attaking = false;

	EnemyBugStates enemyBugState = EnemyBugStates::BUG_WALK_RIGHT;
};

