#pragma once

#include "Enemy.h"
#include <vector>
#include "BossShoot.h"

enum BossDragonStates
{
	BOSS_LEFT, BOSS_LEFT_DOWN, BOSS_RIGHT_DOWN, BOSS_DOWN, BOSS_IDLE
};

enum BossBodyStates
{
	BOSS_BODY_IDLE
};

class BossDragon : public Enemy
{
public:
	BossDragon();
	~BossDragon() override;
	//void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override {}
	void update(int deltaTime) override;
	void initMov(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const ZoneEnemy& initParams) override;
	void render() override;
	void setBossPosition(const glm::vec2& pos) {
		posBody = pos;
		bodySprite->setPosition(glm::vec2(float(tileMapDispl.x + posBody.x - offset.x), float(tileMapDispl.y + posBody.y - offset.y)));
		setHeadSpritePos();
	}

	void Damaged() override;
	void collideVertical() override {}
	void collisionBlockHorizontal(Block* b) override {}

	vector<BossShoot*> getShoots() { return shoots; }

	void setActive() { active = true; }

private:
	void setHeadSpritePos();
	void setBodyAnimations(ShaderProgram& shaderProgram);
	void setHeadAnimations(ShaderProgram& shaderProgram);
	void changeHeadState(BossDragonStates objective);
	void shoot(int deltaTime);

	bool active = false;

	//cycle var
	int state = 0;                       // Boss's state

	int timeSinceLastStateChange = 0;    // Time accumulator for state changes during idle
	int timeSinceLastShoot = 0;           // Time accumulator for shoot during move shooting
	int cycleTime = 0;                   // Total time for the current cycle

	const int idleDuration = 160 * 10;          // 160ms idle period
	const int stateChangeInterval = 16 * 10;   // 16ms interval for state change during idle
	const int shootInterval = 8 * 15;          // 8ms interval for each shoot during shooting phase

	int shootCount = 0;                   // Count the number of shoots made during the shooting phase

	int timeDyingAnim = 500; //0,5 s
	int elapsedTime = 0;

	Texture bodySpritesheet;
	Sprite* bodySprite;

	glm::ivec2 sizeObjBody, sizeObjHead;
	glm::ivec2 posBody, posHead;
	glm::ivec2 positionStartShoot;

	vector<BossShoot*> shoots;
	vector<vector<float>> angleShoots;
	int indexAngleShoot, indexQuadrantShoot = 0;
	int MaxShoots = 3;
	int actualLives = 5;

	BossDragonStates bossDragonState = BossDragonStates::BOSS_LEFT;
	BossBodyStates bossBodyState = BossBodyStates::BOSS_BODY_IDLE;
	vector<BossDragonStates> states;
};
