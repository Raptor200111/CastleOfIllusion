#include "ScenePlay.h"
#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cmath>
#include "Enemy.h"
#include "EnemyTree.h"
#include "EnemyBug.h"
#include "CollisionManager.h"

ScenePlay::ScenePlay()
{
    map = NULL;
    player = NULL;
    zoomLevel = 2.5f;
    bgMap = NULL;
    bgQuad = NULL;
    screenEnemies = std::map<string, Enemy*>();
    screenBlocks = std::map<string, Block*>();
    playrunMovBlocks = std::map<string, Block*>();
	playrunEnemies = std::vector<Enemy*>();
	playrunBlocks = std::vector<Block*>();
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

	//quitar
	for (auto block : allBlocks) {
		delete block;
	}
	playrunBlocks.clear();
	screenBlocks.clear();
	playrunMovBlocks.clear();
	for (auto e : allEnemies)
		delete e;
	playrunEnemies.clear();
	screenEnemies.clear();
}

void ScenePlay::init() {
	initShaders();
}

void ScenePlay::reStart()
{
	playrunEnemies = allEnemies;
	playrunBlocks = allBlocks;
	screenEnemies.clear();
	screenBlocks.clear();
	playrunMovBlocks.clear();
	reStartLevelSpecific();
}

void ScenePlay::update(int deltaTime) {
	//update screenBlocks and screenEnemies
	insideScreenObj();


	CollisionManager::instance().update(screenBlocks, playrunMovBlocks, playrunBlocks);
	player->update(deltaTime);
	updateCamera();
	screenBlocks = CollisionManager::instance().getScreenBlocks();
	playrunMovBlocks = CollisionManager::instance().getMovBlocks();
	playrunBlocks = CollisionManager::instance().getPlayrunBlocks();

	if (insideBossRoom) {
		updateCollisionsWithBoss(deltaTime);
	}
	else {
		for (auto screenEnemy : screenEnemies) {
			//enemy is alive, dying or deadTree waiting to regenerate
			screenEnemy.second->update(deltaTime);
		}
		//collisions alive enemies: blocks, tiles, player
		for (auto& itEnemy = screenEnemies.begin(); itEnemy != screenEnemies.end(); ++itEnemy)
		{
			if (itEnemy->second->getEntityState() == Alive) {
				//add if !godMode
				if (player->getEntityState() == Alive && CollisionManager::instance().checkCollisionObject(player, itEnemy->second)) {
					//if (Player::instance().killEnemy()) {
					itEnemy->second->setEntityState(Dying);
					//} else
					//game onPlayerKilled
					//player to dying
				}
				else {
					//enemy can collide with max 2 blocks
					int countBlockCollisions = 0;
					for (auto& itBlock = screenBlocks.begin(); itBlock != screenBlocks.end(); ++itBlock)
					{
						bool collided = false;
						VColType vBlockCollision = CollisionManager::instance().checkCollisionBlockVertical(itEnemy->second, itBlock->second);
						if (vBlockCollision != NoVcol)
						{
							itEnemy->second->collideVertical();
							collided = true;
							countBlockCollisions += 1;
						}
						else if (!collided)
						{
							HColType hBlockCollision = CollisionManager::instance().checkCollisionBlockHorizontal(itEnemy->second, itBlock->second);
							if (hBlockCollision != NoHcol) {
								itEnemy->second->collideHorizontal(itBlock->second);
								countBlockCollisions += 1;
							}
						}
						if (countBlockCollisions >= 2) {
							break;
						}
					}
					if (CollisionManager::instance().checkCollisionVertical(itEnemy->second) == Tile)
					{
						itEnemy->second->collideVertical();
					}
				}
			}
		}
	}
	/*Collision Moving Blocks : enemies, tiles
	//remember movingBlocks will always be inside screen: player cannot throw them far enough
	for (auto& itMovBlock = playrunMovBlocks.begin(); itMovBlock != playrunMovBlocks.end(); ++itMovBlock)
	{
		EntityState movBlockState = itMovBlock->second->getEntityState();
		if(movBlockState == Alive){
			if(insideBossRoom){
				if (boss.getEntityState() == Alive && CollisionManager::instance().checkCollisionObject(boss, itMovBlock->second))
				{

				}
			}
			else
			{
				for (auto& screenEnemy : screenEnemies) {
					if (screenEnemy->second->getEntityState() == Alive && CollisionManager::instance().checkCollisionObject(screenEnemy->second, itMovBlock->second))
					{
						screenEnemy->second->setEntityState(Dying);
						//if destroyable
						//itMovBlock->second->setEntityState(Dying);
						break;
					}

				}
			}
			//add for check collision movBlocks with all screenBlocks

			if (itMovBlock->second->getEntityState() && CollisionManager::instance().checkCollisionVertical(itMovBlock->second))
			{
				//movBlock exists in allBlocks, notExists in playrunBlocks, screenBlocks
				//if destroyable
					itMovBlock->second->setEntityState(Dying);
					//moving block will start DyingAnimation,
					//once finished if Destroyable->Dead,
			}
			if (itMovBlock->second->getEntityState() && CollisionManager::instance().checkCollisionHorizontal(itMovBlock->second))
			{
				//movBlock exists in allBlocks, notExists in playrunBlocks, screenBlocks
				//if destroyable
					itMovBlock->second->setEntityState(Dying);
					//moving block will start DyingAnimation,
					//once finished if Destroyable->Dead,
			}
			//if(itMovBlock is NonDestroyable && itMovBlock no longer moves){
				//add it to playrunBlocks
			//}

		}
		else if(movBlockState == Dead) {
			//only destroyable blocks can be dead
			//once dead it can be removed from movingBlocks
			//ultimately it will only exist in allBlocks
		}
	}

	*/

	gameUI.update(deltaTime);
}

void ScenePlay::render() {
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection); // Projection is now affected by camera
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	//background

	bgTexture.use();
	bgQuad->render();
	bgMap->render();

	//level
	map->render();
	player->render();
	if (insideBossRoom) {
		renderBoss();
	}
	else
	{
		for (const auto& screenEnemy : screenEnemies) {
			screenEnemy.second->render();
		}
	}
	for (const auto& screenBlock : screenBlocks)
	{
		screenBlock.second->render();
	}

	for (const auto& movBlock : playrunMovBlocks)
	{
		movBlock.second->render();
	}

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
	float maxCameraY = map->getMapSize().y * map->getTileSize() - zoomScreenHeight + 50;

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


void ScenePlay::insideScreenObj()
{
	int tileSize = map->getTileSize();
	glm::ivec2 posP = player->getPosition();
	int tilesize = map->getTileSize();
	insideBossRoom = checkIfInsideBossRoom();
	if(!insideBossRoom) {
		for (auto& playrunEnemy : playrunEnemies)
		{
			glm::ivec2 posEnemyId = playrunEnemy->getInitPos();
			glm::ivec2 posEnemy = playrunEnemy->getPosition();
			glm::ivec2 posEnemyToCompare = glm::ivec2(posEnemyId.x * tileSize, posEnemyId.y * tileSize);
			string idEnemy = std::to_string(posEnemyId.x) + " " + std::to_string(posEnemyId.y);
			EntityState enemyState = playrunEnemy->getEntityState();
			if (enemyState == Dying) {
				if (insideScreen(posEnemyToCompare)) {
					if (screenEnemies.find(idEnemy) == screenEnemies.end())
					{
						screenEnemies.insert(std::pair<string, Enemy*>(idEnemy, playrunEnemy));
					}
				}
				else {
					playrunEnemy->setEntityState(Alive);
					auto it = screenEnemies.find(idEnemy);
					if (it != screenEnemies.end()) {
						it->second = NULL;
						screenEnemies.erase(it);
					}
				}
			}
			//only trees can be dead aka waiting to regenerate
			else if (enemyState == Dead)
			{
				EnemyTree* enemyTree = dynamic_cast<EnemyTree*>(playrunEnemy);
				if (enemyTree && insideScreen(posEnemyToCompare)) {
					if (screenEnemies.find(idEnemy) == screenEnemies.end())
					{
						screenEnemies.insert(std::pair<string, Enemy*>(idEnemy, playrunEnemy));
					}
				}
				else if (!insideScreen(posEnemyToCompare)) {
					playrunEnemy->setEntityState(Alive);
					auto it = screenEnemies.find(idEnemy);
					if (it != screenEnemies.end()) {
						it->second = NULL;
						screenEnemies.erase(it);
					}
				}

			}
			else {
				if (insideScreen(posEnemyToCompare))
				{
					if (screenEnemies.find(idEnemy) == screenEnemies.end())
					{
						screenEnemies.insert(std::pair<string, Enemy*>(idEnemy, playrunEnemy));
					}
				}
				else
				{
					auto it = screenEnemies.find(idEnemy);
					if (it != screenEnemies.end()) {
						it->second = NULL;
						screenEnemies.erase(it);
					}
				}
			}
		}
	}
	for (auto& playrunBlock : playrunBlocks)
	{
		glm::ivec2 posBlock = playrunBlock->getPosition();
		string idBlock = std::to_string(posBlock.x) + " " + std::to_string(posBlock.y);
		if (insideScreen(posBlock))
		{
			if (screenBlocks.find(idBlock) == screenBlocks.end())
			{
				screenBlocks.insert(std::pair<string, Block*>(idBlock, playrunBlock));
			}
		}
		else
		{
			auto it = screenBlocks.find(idBlock);
			if (it != screenBlocks.end()) {
				it->second = NULL;
				screenBlocks.erase(it);
			}
		}
	}

}

bool ScenePlay::insideScreen(const glm::ivec2& pos)
{

	if (cam.left < pos.x && pos.x < cam.right && cam.top < pos.y && pos.y < cam.bottom) {
		return true;
	}
	return false;
}

void ScenePlay::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}