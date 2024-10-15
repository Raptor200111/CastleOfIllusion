#pragma once

#include "Enemy.h"

enum EnemyBugStates
{
	BUG_WALK_LEFT, BUG_WALK_RIGHT, BUG_ROLL_LEFT, BUG_ROLL_RIGHT, BUG_DIE
};

class EnemyBug : public Enemy
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const Zone& limit);
	void update(int deltaTime, const glm::ivec2& posPlayer);
	void render() override;


private:

	Zone limit;

	int attackDistance = 5*16;//5*mapTileSize
	bool attaking = false;

	EnemyBugStates enemyBugState = EnemyBugStates::BUG_WALK_RIGHT;
};

