#pragma once

#include "Enemy.h"

enum EnemyTreeStates
{
	WALK_LEFT, WALK_RIGHT, DIE_LEFT, DIE_RIGHT
};

class EnemyTree: public Enemy
{
public:
	EnemyTree() {}
	~EnemyTree() override {}
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override;//, bool right
	void update(int deltaTime) override;
	void render() override;

private:
	
	EnemyTreeStates enemyTreeState = EnemyTreeStates::WALK_RIGHT;

};

