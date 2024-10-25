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
	bool checkCollisionObject(Entity* objectA, Entity* objectB);
	CollisionType checkCollisionHorizontal(Entity* entity);
	CollisionType checkCollisionVertical(Entity* entity);
	VColType checkCollisionBlockVertical(Entity* objectA, Entity* objectB);
	HColType checkCollisionBlockHorizontal(Entity* objectA, Entity* objectB);


	//---Coupling: functions needed by player
	//functions level calls bcs coupling
	std::map<string, Block*> getScreenBlocks() { return screenBlocks; }
	std::map<string, Block*> getMovBlocks() { return screenMovBlocks; }
	vector<Block*> getPlayrunBlocks() { return playrunBlocks; }
	void update(const std::map<string, Block*>& screenBlocks, std::map<string, Block*>& screenMovBlocks, vector<Block*>& playrunBlocks);

	//functions check collision that player calls
	Block* collisionEntityBlockH(Entity* entity);
	Block* collisionEntityBlockV(Entity* entity);
	void attachBlock(Block* b);
	void disAttachBlock(Block* b);
	//---


private:
	bool correctRamp(Entity* entity);


	//---shoud disappear
	std::map<string, Block*> screenBlocks;
	std::map<string, Block*> screenMovBlocks;
	vector<Block*> playrunBlocks;
	//---


	int tileSize;
	TileMap* tileMap;
	glm::ivec2 mapSize;
};
