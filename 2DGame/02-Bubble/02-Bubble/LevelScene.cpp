#include "LevelScene.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cmath>
#include "Scene.h"
#include "Game.h"
#include "EnemyTree.h"
#include "EnemyBug.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 2//+35+36// 4+20
#define INIT_PLAYER_Y_TILES 8//+8+2 //20

LevelScene::LevelScene()
{
	map = NULL;
	player = NULL;
	bgQuad = NULL;
	blocksByType = std::map<int, std::vector<Block*>>();
	enemies = std::map<int, Enemy*>();
	zoomLevel = 2.5f;
}

LevelScene::~LevelScene() 
{
	if (map != NULL)
		delete map;
	/*
	if (player != NULL)
		delete player;
		*/
	if (bgQuad != NULL)
		delete bgQuad;
	for (auto blockTypes : blocksByType) {
		for (auto block : blockTypes.second) {
			delete block;
		}
	}
	for (auto& pair : enemies)
	{
		delete pair.second;
	}
	enemies.clear();
}

void LevelScene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/levelMatrix.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = &Player::instance();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2((INIT_PLAYER_X_TILES)*map->getTileSize(), (INIT_PLAYER_Y_TILES)*map->getTileSize()));
	player->setTileMap(map);

	initZoneEnemyTree();
	initZoneEnemyBug();

	bgTexture.loadFromFile("images/portada.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bgQuad = Sprite::createSprite(glm::vec2(map->getMapSize() * map->getTileSize()), glm::vec2(1.f, 1.f), &bgTexture, &texProgram);
	
	std::map<int, std::vector<glm::ivec2>> blocksPosByType = map->getBlocksPos();

	for (const auto& blockType : blocksPosByType)
	{
		for (const auto& blockPos : blockType.second) {
			Block* block = new Block();
			block->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, blockType.first);
			block->setPosition(glm::vec2((blockPos.x) * map->getTileSize(), (blockPos.y) * map->getTileSize()));
			block->setTileMap(map);

			auto it = blocksByType.find(blockType.first);
			if (it == blocksByType.end()) {
				std::vector<Block*> auxItem = { block };  // Initialize vector with the item
				blocksByType.emplace(blockType.first, auxItem);
			}
			else {
				it->second.push_back(block);
			}
		}
	}
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;
	gameUI.init();
}

void LevelScene::initZoneEnemyTree()
{
	EnemyType enemyTree = EnemyType::Tree;
	Zone limit = { 4.0f * map->getTileSize(), 22.0f * map->getTileSize(), 0, 0 };
	glm::ivec2 initPos = glm::ivec2(20.0f, 7.0f);
	InitEnemy zone1 = {1, enemyTree, limit, initPos, true };

	limit = { 26.0f * map->getTileSize(), 38.0f * map->getTileSize(), 0, 0 };
	initPos = glm::ivec2(37.0f, 6.0f);
	InitEnemy zone2 = { 2, enemyTree, limit, initPos, true };

	limit = { 39.0f * map->getTileSize(), 46.0f * map->getTileSize(), 0, 0 };
	initPos = glm::ivec2(45.0f, 7.0f);
	InitEnemy zone3 = { 3, enemyTree, limit, initPos, true };

	limit = { 54.0f * map->getTileSize(), 66.0f * map->getTileSize(), 0, 0 };
	initPos = glm::ivec2(65.0f, 6.0f);
	InitEnemy zone4 = { 4, enemyTree, limit, initPos, true };
	
	enemyZones.push_back(zone1);
	enemyZones.push_back(zone2);
	enemyZones.push_back(zone3);
	enemyZones.push_back(zone4);
}
void LevelScene::initZoneEnemyBug()
{
	EnemyType enemyBug = EnemyType::Bug;

	Zone limit = { 34.0f * map->getTileSize(), 46.0f * map->getTileSize(), 17, 20 };
	glm::ivec2 initPos = glm::ivec2(35.0f, 17.0f);
	InitEnemy zoneBug1 = { 10, enemyBug, limit, initPos, false };

	limit = { 76.0f * map->getTileSize(), 89.0f * map->getTileSize(), 20, 20 };
	initPos = glm::ivec2(88.0f, 20.0f);
	InitEnemy zoneBug2 = { 20, enemyBug, limit, initPos, false };

	limit = { 69.0f * map->getTileSize(), 81.0f * map->getTileSize(), 31, 31 };
	initPos = glm::ivec2(80.0f, 31.0f);
	InitEnemy zoneBug3 = { 30, enemyBug, limit, initPos, false };

	limit = { 26.0f * map->getTileSize(), 37.0f * map->getTileSize(), 32, 32 };
	initPos = glm::ivec2(26.0f, 32.0f);
	InitEnemy zoneBug4 = { 40, enemyBug, limit, initPos, false };

	limit = { 14.0f * map->getTileSize(), 24.0f * map->getTileSize(), 30, 32 };
	initPos = glm::ivec2(14.0f, 30.0f);
	InitEnemy zoneBug5 = { 50, enemyBug, limit, initPos, false };

	enemyZones.push_back(zoneBug1);
	enemyZones.push_back(zoneBug2);
	enemyZones.push_back(zoneBug3);
	enemyZones.push_back(zoneBug4);
	enemyZones.push_back(zoneBug5);
}

void LevelScene::update(int deltaTime)
{
	insideEnemyTreeZone(player->getPosition());
	
	for (const auto& enemy : enemies) {
		EnemyBug* enemyBug = dynamic_cast<EnemyBug*>(enemy.second);
		if (enemyBug)
			enemyBug->update(deltaTime, player->getPosition());
		else
			enemy.second->update(deltaTime);
	}
	

	player->update(deltaTime);
	/*
	therefore i need posPlayer, sizePlayer, isAttacking, posEnemy, sizeEnemy
	// if collision (posPlayer, sizePlayer,posEnemy, sizeEnemy)
	//		if isAttacking ==> enemy die
	//		else		   ==> player "die"; --star;
	*/

	updateCamera();
	gameUI.update(deltaTime);
}

void LevelScene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection); // Projection is now affected by camera
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	//level
	bgTexture.use();
	bgQuad->render();
	map->render();
	player->render();
	for (const auto& enemy : enemies) {
		enemy.second->render();
	}

	for (const auto& blockTypes : blocksByType)
	{
		for (auto block : blockTypes.second) {
			block->render();
		}
	}
	gameUI.render();
}

void LevelScene::updateCamera()
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
	float maxCameraY = map->getMapSize().y * map->getTileSize() - zoomScreenHeight;

	if (cameraPosition.x < 0) cameraPosition.x = 0;
	if (cameraPosition.y < 0) cameraPosition.y = 0;
	if (cameraPosition.x > maxCameraX) cameraPosition.x = maxCameraX;
	if (cameraPosition.y > maxCameraY) cameraPosition.y = maxCameraY;

	// Update projection matrix to account for camera movement
	projection = glm::ortho(cameraPosition.x, cameraPosition.x + zoomScreenWidth,
		cameraPosition.y + zoomScreenHeight, cameraPosition.y);
}


void LevelScene::insideEnemyTreeZone(glm::ivec2& posPlayer)
{
	for (const auto& enemyZone : enemyZones) {
		if (enemyZone.limit.max_x > posPlayer.x && posPlayer.x >= enemyZone.limit.min_x)
		{
			if (enemies.find(enemyZone.id) == enemies.end()) {
				Enemy* enemy;
				bool right = posPlayer.x > enemyZone.initPos.x * map->getTileSize();
				if (enemyZone.enemyType == EnemyType::Tree)
				{
					enemy = new EnemyTree();
				}
				else
				{
					enemy = new EnemyBug();
				}
				ZoneEnemy initParams = { enemyZone.limit, enemyZone.initPos, !right };
				enemy->initMov(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, initParams);
				enemy->setPosition(glm::vec2(enemyZone.initPos.x * map->getTileSize(), (enemyZone.initPos.y) * map->getTileSize()));
				enemy->setTileMap(map);
				enemies.insert(std::pair<int, Enemy*>(enemyZone.id, enemy));
				cout << "INSERTING \n";
				cout << "Enemy x " << enemy->getEnemyPos().x << " y " << enemy->getEnemyPos().y << "\n";
				cout << "ZoneLimit x " << enemyZone.limit.min_x << "  max: " << enemyZone.limit.max_x << "\n";
				cout << "player x " << posPlayer.x << " y " << posPlayer.y << "\n";
				debug = enemyZone;
			}
		}
		else {
			auto it = enemies.find(enemyZone.id);
			if (it != enemies.end())
			{
				cout << "DELETING \n";
				cout << "Enemy x " << it->second->getEnemyPos().x << " y " << it->second->getEnemyPos().y << "\n";
				cout << "ZoneLimit x " << enemyZone.limit.min_x << "  max: " << enemyZone.limit.max_x << "\n";
				cout << "player x " << posPlayer.x << " y " << posPlayer.y << "\n";
				enemies.erase(enemyZone.id);
			}
		}
	}
}


void LevelScene::initShaders()
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
