#pragma once
#include "Entity.h"

enum BossShootStates
{
	BOSS_SHOOT_START, BOSS_SHOOT_END
};

class BossShoot : public Entity
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();
private:
	void setShootAnimations(ShaderProgram& shaderProgram);
	float velocity;
	BossShootStates shootState = BossShootStates::BOSS_SHOOT_START;
};

