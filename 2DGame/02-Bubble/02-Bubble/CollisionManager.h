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
	

//---shoud disappear
	void update(const std::map<string, Block*>& screenBlocks);

	//functions check collision that player calls
	Block* collisionEntityBlockH(Entity* entity);
	Block* collisionEntityBlockV(Entity* entity);
//---


private:
	int correctRampPos(int tileX, int tileY, int sizeY, glm::ivec2 pos, bool left);


//---shoud disappear
	std::map<string, Block*> screenBlocks;
//---


	int tileSize;
	TileMap* tileMap;
	glm::ivec2 mapSize;
};

