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
	void setActive() { active = true; }
	void setDirection(glm::vec2 direction);

private:
	void setShootAnimations(ShaderProgram& shaderProgram);
	
	glm::vec2 endDirection;   
	bool active = false;
	float speed = 100.0f;

	int maxTime = 160*100;
	int elapsedTime = 0;
	BossShootStates shootState = BossShootStates::BOSS_SHOOT_START;
};

