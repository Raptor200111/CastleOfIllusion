#pragma once

struct Cam {
	float left, right, bottom, top;
};

#ifndef _LEVEL_SCENE_INCLUDE
#define _LEVEL_SCENE_INCLUDE

#include "Scene.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
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
	void insideEnemyTreeZone(glm::ivec2& posPlayer);

	TileMap* map;
	Player* player;
	std::map<int, std::vector<Block*>> blocksByType; //key == typeBlock
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	float zoomLevel;
	glm::vec2 cameraPosition;

	//activeEnemies
	std::map<int,Enemy*> enemies;

	//all enemies
	vector<InitEnemy> enemyZones;
	InitEnemy debug;


	//background
	Sprite* bgQuad;
	Texture bgTexture;
	TileMap* bgMap;
};

#endif // _LEVEL_SCENE_INCLUDE



