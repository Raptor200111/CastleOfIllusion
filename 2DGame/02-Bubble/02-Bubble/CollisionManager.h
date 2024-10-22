#pragma once

#include "Scene.h"
#include "Entity.h"
#include "Enemy.h"
#include "Block.h"
#include <map>

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();
	static CollisionManager& instance()
	{
		static CollisionManager CM;

		return CM;
	}
	void init(TileMap* tileMap);
	void sceneInit(Cam camera, const std::vector<Block*>& blocksObj, const vector<Enemy*>& enemiesObj);
	bool checkCollisionObject(Entity* objectA, Entity* objectB);
	CollisionType checkCollisionHorizontal(Entity* entity);
	CollisionType checkCollisionVertical(Entity* entity);

	void update(int deltaTime, Cam camera);
	
	Block* collisionEntityBlockH(Entity* entity);
	Block* collisionEntityBlockV(Entity* entity);

	std::map<string, Enemy*> enemies;
	std::map<string, Block*> blocks;
private:
	int correctRampPos(int tileX, int tileY, int sizeY, glm::ivec2 pos, bool left);
	VColType checkCollisionBlockVertical(Entity* objectA, Entity* objectB);
	HColType checkCollisionBlockHorizontal(Entity* objectA, Entity* objectB);
	void insideScreenObj(Cam cam);
	bool insideScreen(glm::ivec2 pos, Cam cam);

	vector<Enemy*> enemiesObj;
	vector<Block*> blocksObj;
	int tileSize;
	TileMap* tileMap;
	glm::ivec2 mapSize;
};

