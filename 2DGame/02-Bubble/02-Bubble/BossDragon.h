#pragma once

#include "Enemy.h"

enum BossDragonStates
{
	BOSS_IDLE //, BOSS_MOVE_RIGHT, BOSS_DIE, BOSS_ATTACK
};

enum BossBodyStates
{
	BOSS_BODY_IDLE
};
class BossDragon : public Enemy
{
public:
	BossDragon() { bodySprite = NULL; }
	~BossDragon() override { if (bodySprite != NULL) delete bodySprite; }
	//void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override {}
	void update(int deltaTime) override {}
	void initMov(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const ZoneEnemy& initParams) override;
	void update(int deltaTime, const glm::ivec2& posPlayer);
	void render() override;

private:
	void setBodyAnimations(ShaderProgram& shaderProgram);
	void setHeadAnimations(ShaderProgram& shaderProgram);
	int attackDistance = 3 * 16;//5*mapTileSize
	int attackSpeed;
	Texture bodySpritesheet;
	Sprite* bodySprite;
	BossDragonStates bossDragonState = BossDragonStates::BOSS_IDLE;
	BossBodyStates bossBodyState = BossBodyStates::BOSS_BODY_IDLE;

};

