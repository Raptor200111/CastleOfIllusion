#pragma once

#ifndef _LEVEL_SCENE_INCLUDE
#define _LEVEL_SCENE_INCLUDE

#include "PlayScene.h"
#include "BossDragon.h"

class LevelScene : public PlayScene
{
public:
	LevelScene();
	~LevelScene();
	void init() override;   // Initialize NormalPlayScene-specific elements

protected:
	// Override these methods to handle boss logic and room detection for NormalPlayScene
	void updateCollisionsWithBoss(int deltaTime) override;
	bool checkIfInsideBossRoom() override;
	void renderBoss() override;
	void initZoneEnemyTree();
	void initZoneEnemyBug();
	BossDragon boss;
	vector<BossShoot*> shoots;
};

#endif // _LEVEL_SCENE_INCLUDE


