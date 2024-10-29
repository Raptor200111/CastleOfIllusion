#include "ScenePlay.h"
#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cmath>
#include "Enemy.h"
#include "EnemyTree.h"
#include "EnemyBug.h"

#include "BlockChestCake.h"
#include "BlockChestCoin.h"
#include "BlockDestroyable.h"
#include "BlockNonDestroyable.h"

#include "CollisionManager.h"
//debug
#include <chrono>
#include <glm/gtc/random.hpp>

ScenePlay::ScenePlay()
{
    map = NULL;
    player = NULL;
    zoomLevel = 2.f;
    bgMap = NULL;
    bgQuad = NULL;
	quad = NULL;
    screenEnemies = std::map<string, Enemy*>();
    screenBlocks = std::map<string, Block*>();
    playrunMovBlocks = std::map<string, Block*>();
	playrunEnemies = std::vector<vector<Enemy*>>();
	playrunBlocks = std::vector<vector<Block*>>();
	allEnemies = playrunEnemies;
	allBlocks = playrunBlocks;
}

ScenePlay::~ScenePlay()
{
	if (map != NULL)
		delete map;
	/*
	if (player != NULL)
		delete player;
		*/
	if (bgMap != NULL)
		delete bgMap;
	if (bgQuad != NULL)
		delete bgQuad;
	if (quad != NULL)
		delete quad;
	//quitar
	for (auto& row : allBlocks) {
		for (Block* block : row) {
			delete block; // Delete each Block* pointer
		}
	}

	// Clear the containers to remove all references
	screenBlocks.clear();
	playrunMovBlocks.clear();
	playrunBlocks.clear();
	allBlocks.clear();
	for (auto& row : allEnemies) {
		for (Enemy* enemy : row) {
			delete enemy; // Delete each Block* pointer
		}
	}
	screenEnemies.clear();
	playrunEnemies.clear();
	allEnemies.clear();
}

void ScenePlay::init() {
	initShaders();
}

vector<vector<Block*>> ScenePlay::initBlocks()
{
	
	vector<vector<Block*>> blocks(4);
	for (auto block : map->getBlocksPos()) {
		Block* b;
		switch (block.type) {
		case 1:
			b = new BlockChestCake(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			break;
		case 3:
			b = new BlockDestroyable(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			break;
		case 4:
			b = new BlockChestCoin(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		break; 
		default:
			b = new BlockNonDestroyable(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			break;
		}
		b->setPosition(glm::ivec2(block.pos.x * map->getTileSize(), block.pos.y * map->getTileSize()));
		b->setOgPosition(glm::ivec2(block.pos.x * map->getTileSize(), block.pos.y * map->getTileSize()));
		b->setTileMap(map);
		int floorIndex = calcFloorIndex(block.pos.y);
		blocks[floorIndex].push_back(b);
	}
	return blocks;
}

void ScenePlay::reStart()
{
	playrunEnemies = allEnemies;
	playrunBlocks = allBlocks;
	for (auto& row : allBlocks)
	{
		for (auto& b : row) {
			glm::ivec2 ogPosB = b->getOgPosition();
			b->setPosition(ogPosB);
		}
	}
	screenEnemies.clear();
	screenBlocks.clear();
	playrunMovBlocks.clear();
	winAnimScenePlay = false;
	reStartLevelSpecific();
}

void ScenePlay::update(int deltaTime) {
	//update screenBlocks and screenEnemies
	
	int floorIndex = calcFloorIndex(player->getPosition().y/map->getTileSize());
	insideScreenObj(floorIndex);

	CollisionManager::instance().update(screenBlocks, playrunMovBlocks, playrunBlocks[floorIndex]);
	player->update(deltaTime);
	updateCamera();
	screenBlocks = CollisionManager::instance().getScreenBlocks();
	playrunMovBlocks = CollisionManager::instance().getMovBlocks();
	playrunBlocks[floorIndex] = CollisionManager::instance().getPlayrunBlocks();


	if (insideBossRoom) {
		//auto start = std::chrono::high_resolution_clock::now();
		updateCollisionsWithBoss(deltaTime);
		//auto end = std::chrono::high_resolution_clock::now();
		//std::chrono::duration<float, std::milli> duration = end - start;
		//std::cout << "updateCollisionsWithBoss execution time: " << duration.count() << "ms" << std::endl;
	}
	else {
		collisionsEnemies(deltaTime);
	}

	for (const auto& screenBlock : screenBlocks)
	{
		screenBlock.second->update(deltaTime);
	}

	collisionsMovingBlocks(deltaTime);

	gameUI.update(deltaTime);
}

void ScenePlay::render() {
	glm::mat4 modelview = glm::mat4(1.0f);
	simpleProgram.use();
	simpleProgram.setUniformMatrix4f("projection", projection);
	simpleProgram.setUniformMatrix4f("modelview", modelview);

	float shakeIntensity = 10.0f;
	float offsetX = glm::linearRand(-shakeIntensity, shakeIntensity);
	float offsetY = glm::linearRand(-shakeIntensity, shakeIntensity);
	glm::mat4 projectionWithShake = glm::translate(projection, glm::vec3(offsetX, offsetY, 0.0f));
	if (!winAnimScenePlay) {
		simpleProgram.setUniform4f("color", 48 / 255.f, 188 / 255.f, 1.f, 0.9f);
	}
	else {
		int rand = std::rand();
		//int rand1 = std::rand();
		simpleProgram.setUniform4f("color", rand / 255.f, 0/ 255.f, (255-rand) / 255.f, 0.9f);
		simpleProgram.setUniformMatrix4f("projection", projectionWithShake);

	}
	quad->render();

	texProgram.use();
	if (!winAnimScenePlay)
		texProgram.setUniformMatrix4f("projection", projection); // Projection is now affected by camera
	else
		texProgram.setUniformMatrix4f("projection", projectionWithShake); // Projection is now affected by camera

	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	//background
	
	//bgTexture.use();
	//bgQuad->render();
	if(bgMap != NULL)
		bgMap->render();

	//level
	map->render();
	if (insideBossRoom) {
		renderBoss();
	}
	else
	{
		for (const auto& screenEnemy : screenEnemies) {
			if (insideScreen(screenEnemy.second->getPosition()))
				screenEnemy.second->render();
			else if(screenEnemy.second->getEnemyType() == EntityState::DEAD)
				screenEnemy.second->reLive();
		}
	}
	for (const auto& screenBlock : screenBlocks)
	{
		if (insideScreen(screenBlock.second->getPosition()))
			screenBlock.second->render();
	}

	for (const auto& movBlock : playrunMovBlocks)
	{
		movBlock.second->render();
	}
	player->render();

	gameUI.render();

}


void ScenePlay::updateCamera()
{
	glm::vec2 playerPos = player->getPosition();
	float zoomScreenWidth = SCREEN_WIDTH / zoomLevel;
	float zoomScreenHeight = SCREEN_HEIGHT / zoomLevel;

	// Calculate the desired camera position based on player's position
	float halfScreenWidth = zoomScreenWidth / 2.0f;
	float halfScreenHeight = zoomScreenHeight / 2.0f;

	// Center camera on player but with some offset leading the player when moving right
	cameraPosition.x = playerPos.x - halfScreenWidth + 100; // Adjust the '100' for horizontal leading effect
	cameraPosition.y = playerPos.y - halfScreenHeight;

	// Constrain the camera within the map boundaries
	float maxCameraX = map->getMapSize().x * map->getTileSize() - zoomScreenWidth;
	float maxCameraY = map->getMapSize().y * map->getTileSize() - zoomScreenHeight + 52;

	if (cameraPosition.x < 0) cameraPosition.x = 0;
	if (cameraPosition.y < 0) cameraPosition.y = 0;
	if (cameraPosition.x > maxCameraX) cameraPosition.x = maxCameraX;
	if (cameraPosition.y > maxCameraY) cameraPosition.y = maxCameraY;
	cam = { cameraPosition.x, cameraPosition.x + zoomScreenWidth,
		cameraPosition.y + zoomScreenHeight, cameraPosition.y };

	// Update projection matrix to account for camera movement
	projection = glm::ortho(cam.left, cam.right,
		cam.bottom, cam.top);

	cam = { cam.left - 50, cam.right + 50, cam.bottom + 50, cam.top - 50 };

}

void ScenePlay::insideScreenObj(int floorIndex)
{
	int tileSize = map->getTileSize();
	glm::ivec2 posP = player->getPosition();
	insideBossRoom = checkIfInsideBossRoom();
	if(!insideBossRoom) {
		if (!screenEnemies.empty()) {
			int enemyPosY = screenEnemies.begin()->second->getPosition().y;
			if (calcFloorIndex(enemyPosY) != floorIndex)
			{
				screenEnemies.clear();
			}
		}
		if(screenEnemies.empty()){
			for (auto& playrunEnemy : playrunEnemies[floorIndex])
			{
				glm::ivec2 posEnemyId = playrunEnemy->getInitPos();
				string idEnemy = std::to_string(posEnemyId.x) + " " + std::to_string(posEnemyId.y);
				screenEnemies.insert(std::pair<string, Enemy*>(idEnemy, playrunEnemy));
			}			
		}
	}
	if (!screenBlocks.empty()) {
		int blocksPosY = screenBlocks.begin()->second->getPosition().y;
		if (calcFloorIndex(blocksPosY) != floorIndex)
		{
			screenBlocks.clear();
		}
	}
	if (screenBlocks.empty()) {
		for (auto& playrunBlock : playrunBlocks[floorIndex])
		{
			glm::ivec2 posBlock = playrunBlock->getPosition();
			string idBlock = std::to_string(posBlock.x) + " " + std::to_string(posBlock.y);
			screenBlocks.insert(std::pair<string, Block*>(idBlock, playrunBlock));
		}
	}
}

int ScenePlay::calcFloorIndex(int posY)
{
	if (posY <= MAX_HEIGHT_F0)
		return 0;
	else if (posY <= MAX_HEIGHT_F1)
		return 1;
	else if (posY <= MAX_HEIGHT_F2)
		return 2;
	else
		return 3;
}

bool ScenePlay::insideScreen(const glm::ivec2& pos)
{

	if (cam.left < pos.x && pos.x < cam.right && cam.top < pos.y && pos.y < cam.bottom) {
		return true;
	}
	return false;
}


void ScenePlay::collisionsEnemies(int deltaTime)
{
	for (auto& itEnemy = screenEnemies.begin(); itEnemy != screenEnemies.end();)
	{
		bool reStarted = false;
		itEnemy->second->update(deltaTime);
		EnemyType enemyType = itEnemy->second->getEnemyType();
		if (itEnemy->second->getEntityState() == EntityState::STILL && player->getEntityState() == EntityState::STILL
			&& !Game::instance().isOnGodMode() && CollisionManager::instance().checkCollisionBlockVertical(player, itEnemy->second) == Down) {
			if (player->isAttacking()) {
				Game::instance().onPlayerKilledEnemy();
				itEnemy->second->Damaged();
			}
			else {
				string idEnemy = itEnemy->first;
				Game::instance().onPlayerKilled();
				player->setEntityState(DYING);
				if (screenEnemies.find(idEnemy) == screenEnemies.end()) {
					reStarted = true;
					break;
				}
			}
		}
		if (!reStarted) {
			if (enemyType != Bee) {
				if (itEnemy->second->getEntityState() == STILL && CollisionManager::instance().checkCollisionVertical(itEnemy->second) == Tile)
				{
					itEnemy->second->collideVertical();
				}

				if (itEnemy->second->getEntityState() == STILL) {
					int countBlockCollisions = 0;
					for (auto& itBlock = screenBlocks.begin(); itBlock != screenBlocks.end(); ++itBlock)
					{
						bool collided = false;
						HColType hBlockCollision = CollisionManager::instance().checkCollisionBlockHorizontal(itEnemy->second, itBlock->second);
						if (hBlockCollision != NoHcol) {
							itEnemy->second->collisionBlockHorizontal(itBlock->second);
							collided = true;
							countBlockCollisions += 1;
						}
						else if (!collided)
						{
							VColType vBlockCollision = CollisionManager::instance().checkCollisionBlockVertical(itEnemy->second, itBlock->second);
							if (vBlockCollision != NoVcol)
							{
								itEnemy->second->collideVertical();
								countBlockCollisions += 1;
							}
						}
						if (countBlockCollisions >= 2) {
							break;
						}
					}
				}

				
				if (itEnemy->second->getEntityState() == STILL && CollisionManager::instance().checkCollisionHorizontal(itEnemy->second) == Tile)
				{					
          itEnemy->second->collideHorizontal();
				}
			}
			++itEnemy;
		}
	}
}

void ScenePlay::collisionsMovingBlocks(int deltaTime)
{
	//remember movingBlocks will always be inside screen: player cannot throw them far enough
	for (auto& itMovBlock = playrunMovBlocks.begin(); itMovBlock != playrunMovBlocks.end();)
	{
		bool elementErased = false;
		itMovBlock->second->update(deltaTime);
		BlockType blockType = itMovBlock->second->getBlockType();
		//block alive === moving

		//check collisions with enemies
		if (itMovBlock->second->getEntityState() == FALLING) {
			if (insideBossRoom) {
				collisionMovBlockInsideBossRoom(itMovBlock->second);
			}
			else
			{
				for (auto& screenEnemy : screenEnemies) {
					if (screenEnemy.second->getEntityState() == FALLING &&
						CollisionManager::instance().checkCollisionObject(screenEnemy.second, itMovBlock->second))
					{
						Game::instance().onPlayerKilledEnemy();
						screenEnemy.second->Damaged();
						itMovBlock->second->collisionEnemy(screenEnemy.second->getPosition());
						break;
					}

				}
			}
		}

		//check collisions with other blocks
		if (itMovBlock->second->getEntityState() == FALLING) {
			int countBlockCollisions = 0;
			for (auto& itBlock = screenBlocks.begin(); itBlock != screenBlocks.end(); ++itBlock)
			{
				bool collided = false;
				const Block* cBlock = itBlock->second;
				VColType vBlockCollision = CollisionManager::instance().checkCollisionBlockVertical(itMovBlock->second, itBlock->second);
				if (vBlockCollision != NoVcol)
				{
					itMovBlock->second->collisionBlockVertical(vBlockCollision, cBlock);
					collided = true;
					countBlockCollisions += 1;
				}

				HColType hBlockCollision = CollisionManager::instance().checkCollisionBlockHorizontal(itMovBlock->second, itBlock->second);
				if (hBlockCollision != NoHcol) {
					itMovBlock->second->collisionBlockHorizontal(hBlockCollision, cBlock);
					countBlockCollisions += 1;
				}

				if (countBlockCollisions >= 2) {
					break;
				}
			}
		}

		//check collision tilemap vertical
		if (itMovBlock->second->getEntityState() == FALLING) {
			CollisionType verticalCollision = CollisionManager::instance().checkCollisionVertical(itMovBlock->second);
			if (verticalCollision != None)
				itMovBlock->second->collisionVertical(verticalCollision);
		}

		//check collision tilemap horizontal
		if (itMovBlock->second->getEntityState() == FALLING) {
			CollisionType horizontalCollision = CollisionManager::instance().checkCollisionHorizontal(itMovBlock->second);
			if (horizontalCollision != None)
				itMovBlock->second->collisionHorizontal(horizontalCollision);
		}

		//block Dead == has stopped moving
		if (itMovBlock->second->getEntityState() == DEAD) {
			if (blockType == NonDestroyable) {
				itMovBlock->second->setEntityState(STILL);
				int movBlockFloorIndex = calcFloorIndex(itMovBlock->second->getPosition().y / map->getTileSize());
				playrunBlocks[movBlockFloorIndex].push_back(itMovBlock->second);
			}
			elementErased = true;
			itMovBlock->second = NULL;
			itMovBlock = playrunMovBlocks.erase(itMovBlock);
				//once dead it can be removed from movingBlocks
				//it will only exists in allBlocks
		}
		if (elementErased == false) {
			++itMovBlock;
		}
	}
}


void ScenePlay::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/simple.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/simple.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleProgram.init();
	simpleProgram.addShader(vShader);
	simpleProgram.addShader(fShader);
	simpleProgram.link();
	if (!simpleProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleProgram.log() << endl << endl;
	}
	simpleProgram.bindFragmentOutput("outColor");

	vShader.free();
	fShader.free();
	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		std::cout << "Vertex Shader Error" << endl;
		std::cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		std::cout << "Fragment Shader Error" << endl;
		std::cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		std::cout << "Shader Linking Error" << endl;
		std::cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
}