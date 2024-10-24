#pragma once

#include "Enemy.h"

enum EnemyBugStates
{
	BUG_WALK_RIGHT, BUG_ROLL_RIGHT, BUG_DIE
};

class EnemyBug : public Enemy
{
public:
	EnemyBug() { }
	~EnemyBug() override {}
	//void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override {}
	void update(int deltaTime) override;
	void initMov(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const ZoneEnemy& initParams) override;
	void render() override;
	void collideVertical() override;
	void collideHorizontal(Block* b) override;

private:

	int attackDistance = 3*16;//5*mapTileSize
	int attackSpeed;
	int elapsedTime = 0;
	int timeDyingAnim = 500; //0,5 s

	EnemyBugStates enemyBugState = EnemyBugStates::BUG_WALK_RIGHT;
};

