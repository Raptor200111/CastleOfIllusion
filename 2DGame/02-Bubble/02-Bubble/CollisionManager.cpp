#include "CollisionManager.h"
#include <iostream>
#include "Game.h"
#include "Enemy.h"
#include "EnemyTree.h"
#include "EnemyBug.h"
#include "Player.h"
#include <string>
#include <algorithm> // for std::remove


CollisionManager::CollisionManager()
{
	tileMap = NULL;
	screenBlocks = std::map<string, Block*>();
}

CollisionManager::~CollisionManager()
{
	for (auto& screenBlock : screenBlocks)
		screenBlock.second = NULL;
	screenBlocks.clear();
}


void CollisionManager::init(TileMap* tileMap)
{
	this->tileMap = tileMap;
	tileSize = tileMap->getTileSize();
	mapSize = tileMap->getMapSize();
}

void CollisionManager::update(const std::map<string, Block*>& screenBlocks, std::map<string, Block*>& screenMovBlocks, vector<Block*>& playrunBlocks)
{
	this->screenBlocks = screenBlocks;
	this->screenMovBlocks = screenMovBlocks;
	this->playrunBlocks = playrunBlocks;
}


bool CollisionManager::checkCollisionObject(Entity* objectA, Entity* objectB)
{
	glm::ivec2 pos1 = objectA->getPosition();
	glm::ivec2 size1 = pos1 + objectA->getSize();
	glm::ivec2 pos2 = objectB->getPosition();
	glm::ivec2 size2 = pos2 + objectB->getSize();

	if (size1.x <= pos2.x || size2.x <= pos1.x) return false;
	if (size1.y <= pos2.y || size2.y <= pos1.y) return false;
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

	if (objectB->getEntityState() == DEAD || objectB->getEntityState() == DYING)
	{
		return NoVcol;
	}
	// From here we know there is a collision, now we check the direction of the horizontal collision

	// Check if the player's bottom side is colliding with the block's top side
	if (size1.y > pos2.y && pos1.y < pos2.y) {
		objectA->setPositionY(pos2.y - objectA->getSize().y);
		return Down;
	}

	// Check if the player's top side is colliding with the block's bottom side
	if (pos1.y < size2.y && size1.y > size2.y) {
		objectA->setPositionY(size2.y);
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

	if (objectB->getEntityState() == DEAD || objectB->getEntityState() == DYING)
	{
		return NoHcol;
	}
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
	for (int i = xLeft; i <= xRight; i += 1) {
		for (int j = y0; j <= y1; ++j) {
			int tileType = tileMap->getTileType(i, j);
			if (tileType != 0 && tileType != 7 && tileType != 8) {
				if (tileType == 9)
				{
					stairs = true;
				}
				else if (tileType != 0) {
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

	vector<int> upDown = vector<int>(2, 0);
	int yUp = pos.y / tileSize;						upDown[0] = yUp;
	int yDown = (pos.y + size.y - 1) / tileSize;	upDown[1] = yDown;

	bool correctPos = false;
	int tileToCorrect = 0;
	bool stairs = false;
	bool tile = false;
	bool hole = false;
	// Check the tiles around the entity for collisions
	for (int i = x0; i <= x1; ++i) {
		for (int j = yUp; j <= yDown; j++) {
			int tileType = tileMap->getTileType(i, j);
			if (tileType != 0) {
				if (tileType == 9)
					stairs = true;

				if (tileType == 10) {
					hole = true;
				}
				if (j == yDown && tileType != 9)
				{
					if (pos.y + size.y > tileSize * j)
					{
						//int posY = tileSize * j - size.y;
						//entity->setPositionY(posY);
						tile = true;
						correctPos = true;
						tileToCorrect = j;
					}
				}
				//Up collision
				else if (j == yUp && pos.y < tileSize * (j + 1) && tileType != 9)
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
	if (tile && correctPos) {
		int posY = tileSize * tileToCorrect - size.y;
		entity->setPositionY(posY);
	}
	else if (stairs) {
		entity->setPosition(pos);
	}
	if (correctRamp(entity))
		tile = true;
	if (hole && !Game::instance().isOnGodMode())
		return Hole;
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

	int playerFeetX = (pos.x + size.x / 2.0f) / tileSize;
	//i dont put -1 cause before it already corrected it
	int y = (pos.y + size.y) / tileSize;
	int correctedY;
	if (left)
	{
		int x = pos.x / tileSize;
		int tileInFront = tileMap->getTileType(x, y - 1);
		if (tileInFront == 7 || tileInFront == 8) {
			int xInTile = tileSize - (pos.x % tileSize);
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

Block* CollisionManager::collisionEntityBlockH(Entity* entity) {
	for (auto& it = screenBlocks.begin(); it != screenBlocks.end(); ++it) {
		if (checkCollisionBlockHorizontal(entity, it->second) != NoHcol) {

			return it->second;
			break;
		}
	}
	return NULL;
}

Block* CollisionManager::collisionEntityBlockV(Entity* entity) {
	for (auto& it = screenBlocks.begin(); it != screenBlocks.end(); ++it) {
			VColType vColType = checkCollisionBlockVertical(entity, it->second);
			if (vColType != NoVcol) {
				return it->second;
				break;
			}
	}
	return NULL;
}

void CollisionManager::attachBlock(Block* b) 
{
	string idBlock = std::to_string(b->getPosition().x) + " " + std::to_string(b->getPosition().y);
	auto it = screenBlocks.find(idBlock);
	if (it == screenBlocks.end()) {
		screenMovBlocks.insert(std::pair<string, Block*>(idBlock, b));
	}
}
void CollisionManager::disAttachBlock(Block* b)
{
	for (auto it = playrunBlocks.begin(); it != playrunBlocks.end(); it++)
	{
		if (*it == b) {
			//string idBlock = std::to_string(*it->getOgPosition().x) + " " + std::to_string(*it->getOgPosition().y);
			//screenBlocks.erase(idBlock);
			playrunBlocks.erase(it);
			break;
		}
	}
}