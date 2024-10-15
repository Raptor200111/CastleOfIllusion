#pragma once

struct Cam {
	float left, right, bottom, top;
};

#ifndef _LEVEL_SCENE_INCLUDE
#define _LEVEL_SCENE_INCLUDE

#include "Scene.h"
#include "TileMap.h"
#include "Player.h"
#include "EnemyTree.h"
#include "EnemyBug.h"
#include "Block.h"
#include <map>

class LevelScene : public Scene
{
public:
	LevelScene();
	~LevelScene();
	void init() override;
	void update(int deltaTime) override;
	void render() override;

private:
	void initShaders();
	void updateCamera();
	void initZoneEnemyTree();
	void initZoneEnemyBug();
	bool insideEnemyTreeZone(glm::ivec2& posPlayer);

	TileMap* map;
	Player* player;
	std::map<int, std::vector<Block*>> blocksByType;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	float zoomLevel;
	glm::vec2 cameraPosition;
	Sprite* bgQuad;
	Texture bgTexture;

	EnemyTree* enemyTree;
	EnemyBug* enemyBug;
	bool isInsideEnemyTreeZone;
	vector<ZoneEnemy> enemyTreeZones;
	vector<ZoneEnemy> enemyBugZones;
};

#endif // _LEVEL_SCENE_INCLUDE



