#pragma once

#include "Enemy.h"

enum EnemyTreeStates
{
	WALK_RIGHT, DIE_RIGHT
};

class EnemyTree: public Enemy
{
public:
	EnemyTree() {}
	~EnemyTree() override {}
	//void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override {}
	void initMov(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const ZoneEnemy& initParams) override;
	void update(int deltaTime) override;
	void render() override;
	void collideVertical() override;
	void collideHorizontal(Block* b) override;
private:
	int elapsedTime = 0;
	int timeDyingAnim = 500; //0,5 s
	int timeToRegenerate = 2000; //2s
	EnemyTreeStates enemyTreeState = EnemyTreeStates::WALK_RIGHT;

};

