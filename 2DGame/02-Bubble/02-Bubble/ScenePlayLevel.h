#pragma once

#include "ScenePlay.h"
#include "BossDragon.h"

class ScenePlayLevel : public ScenePlay
{
public:
	ScenePlayLevel();
	~ScenePlayLevel();
	void init() override;   // Initialize NormalPlayScene-specific elements

protected:
	// Override these methods to handle boss logic and room detection for NormalPlayScene
	void updateCollisionsWithBoss(int deltaTime) override;
	bool checkIfInsideBossRoom() override;
	void renderBoss() override;
	void reStartLevelSpecific() override {}
	void initZoneEnemyTree();
	void initZoneEnemyBug();
	BossDragon boss;
	Cam bossRoom;
	vector<BossShoot*> shoots;
};

