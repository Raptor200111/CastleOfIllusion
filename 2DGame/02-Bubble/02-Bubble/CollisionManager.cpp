#include "CollisionManager.h"
#include <iostream>
#include "Game.h"
#include "Enemy.h"
#include "EnemyTree.h"
#include "EnemyBug.h"
#include "Player.h"
#include <string>

CollisionManager::CollisionManager()
{
	tileMap = NULL;
	blocks = std::map<string, Block*>();
	enemies = std::map<string, Enemy*>();
}

CollisionManager::~CollisionManager()
{/*
	for (auto block : blocksObj) {
			delete block;
	}
	blocksObj.clear();
	blocks.clear();
	for (auto e : enemiesObj)
		delete e;
	enemiesObj.clear();
	enemies.clear();*/
}


void CollisionManager::init(TileMap* tileMap)
{
	this->tileMap = tileMap;
	tileSize = tileMap->getTileSize();
	mapSize = tileMap->getMapSize();
/*	cout << "Collision\n";

	for (auto block : blocksObj)
	{
		cout << block->getPosition().x/16 << " " << block->getPosition().y/16 << "\n";
	}*/
}

void CollisionManager::sceneInit(Cam camera, const std::vector<Block*>& blocksObj, const vector<Enemy*>& enemiesObj)
{
	this->blocksObj = blocksObj;
	this->enemiesObj = enemiesObj;
	cout << tileSize;
}


bool CollisionManager::checkCollisionObject(Entity* objectA, Entity* objectB)
{
	glm::ivec2 pos1 = objectA->getPosition();
	glm::ivec2 size1 = pos1 + objectA->getSize();
	glm::ivec2 pos2 = objectB->getPosition();
	glm::ivec2 size2 = pos2 + objectB->getSize();

	if (size1.x < pos2.x || size2.x < pos1.x) return false;
	if (size1.y < pos2.y || size2.y < pos1.y) return false;
	return true;
}

VColType CollisionManager::checkCollisionBlockVertical(Entity* objectA, Entity* objectB)
{
	glm::ivec2 pos1 = objectA->getPosition();
	glm::ivec2 size1 = pos1 + objectA->getSize();
	glm::ivec2 pos2 = objectB->getPosition();
	glm::ivec2 size2 = pos2 + objectB->getSize();

	if (size1.x <= pos2.x || pos1.x >= size2.x || size1.y <= pos2.y || pos1.y >= size2.y)
		return NoVcol;

	// From here we know there is a collision, now we check the direction of the horizontal collision

	// Check if the player's bottom side is colliding with the block's top side
	if (size1.y > pos2.y && pos1.y < pos2.y) {
		return Down; 
	}

	// Check if the player's top side is colliding with the block's bottom side
	if (pos1.y < size2.y && size1.y > size2.y) {
		return Up;
	}

	return NoVcol;

}

HColType CollisionManager::checkCollisionBlockHorizontal(Entity* objectA, Entity* objectB)
{
	glm::ivec2 pos1 = objectA->getPosition();
	glm::ivec2 size1 = pos1 + objectA->getSize(); // Bottom-right corner of objectA (x1, y1)
	glm::ivec2 pos2 = objectB->getPosition();
	glm::ivec2 size2 = pos2 + objectB->getSize(); // Bottom-right corner of objectB (x2, y2)

	// If there is no overlap in either x or y axis, return no horizontal collision
	if (size1.x <= pos2.x || pos1.x >= size2.x || size1.y <= pos2.y || pos1.y >= size2.y)
		return NoHcol;

	// From here we know there is a collision, now we check the direction of the horizontal collision

	// Check if the player's right side is colliding with the block's left side
	if (size1.x > pos2.x && pos1.x < pos2.x) {
		return Right;  // Player's right side collides with block's left side
	}

	// Check if the player's left side is colliding with the block's right side
	if (pos1.x < size2.x && size1.x > size2.x) {
		return Left;   // Player's left side collides with block's right side
	}

	return NoHcol;
}


CollisionType CollisionManager::checkCollisionHorizontal(Entity* entity)
{
	glm::ivec2 pos = entity->getPosition();
	glm::ivec2 size = entity->getSize();

	//left
	int xLeft = pos.x / tileSize;
	//right
	int xRight = (pos.x + size.x - 1) / tileSize;

	int y0 = pos.y / tileSize;
	int y1 = (pos.y + size.y - 1) / tileSize;
	int inc = xRight - xLeft;
	CollisionType value = None;
	bool stairs = false;
	bool tile = false;
	// Check the tiles around the entity for collisions
	for (int i = xLeft; i <= xRight; i+=inc) {
		for (int j = y0; j < y1; ++j) {
			int tileType = tileMap->getTileType(i, j);
			if (tileType != 0 && tileType != 7 && tileType != 8) {
				if (tileType == 9)
				{
					stairs = true;
				}
				/*else if (tileType != 7 && tileType != 8)
				{

					tile = true;
				}*/
				else {
					tile = true;
				}
			}
		}
	}
	if (stairs && tile)
		return TileStairs;
	else if (stairs)
		return Stairs;
	else if (tile)
		return Tile;
	return None;
}


CollisionType CollisionManager::checkCollisionVertical(Entity* entity)
{
	// Get the current tile coordinates
	glm::ivec2 pos = entity->getPosition();
	glm::ivec2 size = entity->getSize();

	int x0 = pos.x / tileSize;
	int x1 = (pos.x + size.x - 1) / tileSize;

	vector<int> upDown= vector<int> (2, 0);
	int yUp = pos.y / tileSize;						upDown[0] = yUp;
	int yDown = (pos.y + size.y - 1) / tileSize;	upDown[1] = yDown;

	bool stairs = false;
	bool tile = false;
	// Check the tiles around the entity for collisions
	for (int i = x0; i <= x1; ++i) {
		for (int j = yUp; j <= yDown; j++) {
			int tileType = tileMap->getTileType(i, j);
			if (tileType != 0) {
				if (tileType == 9)
					stairs = true;
				
				if (j == yDown)
				{
					
					if (pos.y + size.y > tileSize * j)
					{
						int posY = tileSize * j - size.y;
						entity->setPositionY(posY);
						tile = true;
					}

				}
				//Up collision
				else if (j == yUp && pos.y < tileSize * (j + 1))
				{
					if (!stairs) {
						int posY = tileSize * (j + 1);
						entity->setPositionY(posY);
					}
					tile = true;
				}
			}
		}
	}
	if (correctRamp(entity))
		tile = true;

	if (tile && stairs)
		return TileStairs;
	else if (tile)
		return Tile;
	else if (stairs)
		return Stairs;
	return None;
}
bool CollisionManager::correctRamp(Entity* entity)
{
	glm::ivec2 pos = entity->getPosition();
	glm::ivec2 size = entity->getSize();
	bool left = entity->getLeft();
	bool tile = false;

	int playerFeetX = (pos.x + size.x / 2.0f)/tileSize;
	//i dont put -1 cause before it already corrected it
	int y = (pos.y + size.y) / tileSize;
	int correctedY;
	if (left)
	{
		int x = pos.x / tileSize;
		int tileInFront = tileMap->getTileType(x, y - 1);
		if (tileInFront == 7 || tileInFront == 8) {
			int xInTile = tileSize- (pos.x % tileSize);
			correctedY = y * tileSize - size.y - xInTile;
			entity->setPositionY(correctedY);
			tile = true;
		}
	}
	else
	{
		//this option if only heels touch ramp
		int x = (pos.x) / tileSize;
		int tileBelow = tileMap->getTileType(x, y);
		if (tileBelow == 7 || tileBelow == 8) {
			int xInTile = pos.x % tileSize;
			correctedY = y * tileSize - size.y + xInTile;
			entity->setPositionY(correctedY);
			tile = true;
		}
	}
	return tile;

}


void CollisionManager::insideScreenObj(Cam cam)
{
	
	for (auto& enemyObj : enemiesObj)
	{
		glm::ivec2 posEnemyId = enemyObj->getInitPos();
		glm::ivec2 posEnemy = enemyObj->getPosition();
		glm::ivec2 posEnemyToCompare = glm::ivec2(posEnemyId.x * tileSize, posEnemyId.y * tileSize);
		string idEnemy = std::to_string(posEnemyId.x) + " " + std::to_string(posEnemyId.y);
		if (insideScreen(posEnemyToCompare, cam))
		{
			if (enemies.find(idEnemy) == enemies.end())
			{
				enemies.insert(std::pair<string, Enemy*>(idEnemy, enemyObj));
			}
		}
		else
		{
			/*
			cout << posEnemy.x << " " << posEnemy.y << "\n";
			cout << cam.left << " " << cam.right << " " << cam.bottom << "  " << cam.top << "\n";
			*/
			if (enemies.find(idEnemy) != enemies.end()) {
				enemies.erase(idEnemy);
			}
		}
	}
	for (auto& blockObj : blocksObj)
	{
		glm::ivec2 posBlock = blockObj->getPosition();
		string idBlock = std::to_string(posBlock.x) + " " + std::to_string(posBlock.y);
		if (insideScreen(posBlock, cam))
		{
			if (blocks.find(idBlock) == blocks.end())
			{
				blocks.insert(std::pair<string, Block*>(idBlock, blockObj));
			}
		}
		else
		{
			if (blocks.find(idBlock) != blocks.end()) {
				blocks.erase(idBlock);
			}
		}
	}

}

bool CollisionManager::insideScreen(glm::ivec2 pos, Cam cam)
{

	if (cam.left < pos.x && pos.x < cam.right && cam.top < pos.y && pos.y < cam.bottom) {
		/*cout << left << " " << right << "\n";
		cout << pos.x << "\n";*/
		return true;
	}
	return false;
}

// Implementation of the callback function
void CollisionManager::update(int deltaTime, Cam camera)
{
	// Update enemies
	insideScreenObj(camera);

	for (auto& it = enemies.begin(); it != enemies.end(); ++it) {
		it->second->update(deltaTime);

		if (checkCollisionObject(&Player::instance(), it->second)) {
			/*
			if (Player::instance().killEnemy()) {
				it = enemies.erase(it);
				break;
			}*/

		}
	}

}

Block* CollisionManager::collisionEntityBlockH(Entity* entity) {
	for (auto& it = blocks.begin(); it != blocks.end(); ++it) {
		if (checkCollisionBlockHorizontal(entity, it->second)) {
			return it->second;
			break;
		}
	}
}

Block* CollisionManager::collisionEntityBlockV(Entity* entity) {
	for (auto& it = blocks.begin(); it != blocks.end(); ++it) {
		if (checkCollisionBlockVertical(entity, it->second)) {
			return it->second;
			break;
		}
	}
}

