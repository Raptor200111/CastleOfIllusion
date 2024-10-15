#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 2//+35+36// 4+20
#define INIT_PLAYER_Y_TILES 8//+8+2 //20

Scene::Scene()
{
	map = NULL;
	player = NULL;
	bgQuad = NULL;
	menuQuad = NULL;
	blocksByType = std::map<int, std::vector<Block*>>();
	enemyTree = NULL;
	enemyBug = NULL;
	zoomLevel = 2.25f;
}

Scene::~Scene()
{
	if (map != NULL)
		delete map;
	/*
	if (player != NULL)
		delete player;
		*/
	if (bgQuad != NULL)
		delete bgQuad;
	if (menuQuad != NULL)
		delete menuQuad;
	for (auto blockTypes : blocksByType) {
		for (auto block : blockTypes.second) {
			delete block;
		}
	}
	if (enemyTree != NULL)
		delete enemyTree;
	if (enemyBug != NULL)
		delete enemyBug;
}


void Scene::init()
{
	initShaders();
	initMenu();
	initLevel();
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;
}

void Scene::initMenu()
{
	menuTexture.loadFromFile("images/portada.png", TEXTURE_PIXEL_FORMAT_RGBA);
	menuQuad = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.f, 1.f), &menuTexture, &texProgram);
}

void Scene::initLevel()
{
	map = TileMap::createTileMap("levels/levelMatrix.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = &Player::instance();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2((INIT_PLAYER_X_TILES)*map->getTileSize(), (INIT_PLAYER_Y_TILES)*map->getTileSize()));
	player->setTileMap(map);

	cout << player->getPosition().x << " " << player->getPosition().y << "\n";

	isInsideEnemyTreeZone = false;

	initZoneEnemyTree();
	initZoneEnemyBug();

	bgTexture.loadFromFile("images/portada.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bgQuad = Sprite::createSprite(glm::vec2(map->getMapSize()*map->getTileSize()), glm::vec2(1.f, 1.f), &bgTexture, &texProgram);

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
}

void Scene::initZoneEnemyTree()
{
	ZoneEnemy zone1, zone2, zone3, zone4;
	zone1.limit = { 4.0f * map->getTileSize(), 22.0f * map->getTileSize(), 0, 0 };
	zone1.initPos = glm::ivec2(20.0f, 7.0f);

	zone2.limit = { 26.0f * map->getTileSize(), 38.0f * map->getTileSize(), 0, 0 };
	zone2.initPos = glm::ivec2(37.0f, 6.0f);

	zone3.limit = { 39.0f * map->getTileSize(), 46.0f * map->getTileSize(), 0, 0 };
	zone3.initPos = glm::ivec2(45.0f, 7.0f);

	zone4.limit = { 54.0f * map->getTileSize(), 66.0f * map->getTileSize(), 0, 0 };
	zone4.initPos = glm::ivec2(65.0f, 6.0f);
	enemyTreeZones.push_back(zone1);
	enemyTreeZones.push_back(zone2);
	enemyTreeZones.push_back(zone3);
	enemyTreeZones.push_back(zone4);
}
void Scene::initZoneEnemyBug()
{
	ZoneEnemy zoneBug1, zoneBug2, zoneBug3, zoneBug4, zoneBug5;
	zoneBug1.limit = { 34.0f * map->getTileSize(), 46.0f * map->getTileSize(), 17, 20 };
	zoneBug1.initPos = glm::ivec2(35.0f, 17.0f);

	zoneBug2.limit = { 76.0f * map->getTileSize(), 89.0f * map->getTileSize(), 20, 20 };
	zoneBug2.initPos = glm::ivec2(88.0f, 20.0f);

	zoneBug3.limit = { 69.0f * map->getTileSize(), 81.0f * map->getTileSize(), 31, 31 };
	zoneBug3.initPos = glm::ivec2(80.0f, 31.0f);

	zoneBug4.limit = { 26.0f * map->getTileSize(), 37.0f * map->getTileSize(), 32, 32 };
	zoneBug4.initPos = glm::ivec2(26.0f, 32.0f);

	zoneBug5.limit = { 14.0f * map->getTileSize(), 24.0f * map->getTileSize(), 30, 32 };
	zoneBug5.initPos = glm::ivec2(14.0f, 30.0f);
	enemyTreeZones.push_back(zoneBug1);
	enemyTreeZones.push_back(zoneBug2);
	enemyTreeZones.push_back(zoneBug3);
	enemyTreeZones.push_back(zoneBug4);
	enemyTreeZones.push_back(zoneBug5);
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
}

void Scene::updateLevel(int deltaTime)
{
	isInsideEnemyTreeZone = insideEnemyTreeZone(glm::ivec2(player->getPosition()));
	if (isInsideEnemyTreeZone) {
		enemyTree->update(deltaTime);
	}

	//change to enemyZone
	/*
	if (enemyBug != NULL) {
		glm::ivec2 posEnemyBug = enemyBug->getEnemyBugPos();
		enemyBug->update(deltaTime, player->getPlayerPos());
	}
	*/
	player->update(deltaTime);
	//therefore i need posPlayer, sizePlayer, isAttacking, posEnemy, sizeEnemy
	// if collision (posPlayer, sizePlayer,posEnemy, sizeEnemy)
	//		if isAttacking ==> enemy die
	//		else		   ==> player "die"; --star;

	if (isInsideEnemyTreeZone) {
		PosSizeObject treePosSize = {enemyTree->getEnemyTreePos(), enemyTree->getEnemyTreeSize()};
		if (player->checkCollisionObject(treePosSize)) {
			cout << "COLLISION TREE";
		}
	}

	updateCamera();

}

void Scene::renderMenu()
{

	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	
	//menu
	menuTexture.use();
	menuQuad->render();
}

void Scene::updateCamera()
{
	glm::vec2 playerPos = player->getPlayerPos();
	float zoomScreenWidth = SCREEN_WIDTH / zoomLevel;
	float zoomScreenHeight = SCREEN_HEIGHT / zoomLevel;

	// Calculate the desired camera position based on player's position
	float halfScreenWidth = zoomScreenWidth / 2.0f;
	float halfScreenHeight = zoomScreenHeight / 2.0f;

	// Center camera on player but with some offset leading the player when moving right
	cameraPosition.x = playerPos.x - halfScreenWidth + 100; // Adjust the '100' for horizontal leading effect
	cameraPosition.y = playerPos.y - halfScreenHeight;

	// Constrain the camera within the map boundaries
	float maxCameraX = map->getMapSize().x*map->getTileSize() - zoomScreenWidth;
	float maxCameraY = map->getMapSize().y * map->getTileSize() - zoomScreenHeight;

	if (cameraPosition.x < 0) cameraPosition.x = 0;
	if (cameraPosition.y < 0) cameraPosition.y = 0;
	if (cameraPosition.x > maxCameraX) cameraPosition.x = maxCameraX;
	if (cameraPosition.y > maxCameraY) cameraPosition.y = maxCameraY;

	// Update projection matrix to account for camera movement
	projection = glm::ortho(cameraPosition.x, cameraPosition.x + zoomScreenWidth, 
							cameraPosition.y + zoomScreenHeight, cameraPosition.y);
}

void Scene::renderLevel()
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
	if (isInsideEnemyTreeZone) {
		enemyTree->render();
	}
	if(enemyBug != NULL)
		enemyBug->render();

	for (const auto& blockTypes : blocksByType)
	{
		for (auto block : blockTypes.second) {
			block->render();
		}
	}
}


void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}


bool Scene::insideEnemyTreeZone(glm::ivec2& posPlayer)
{
	for (const auto& enemyTreeZone : enemyTreeZones) {
		/*if ((posPlayer.y + SCREEN_HEIGHT / 2) > enemyTreeZone.enemyY0
			&& enemyTreeZone.enemyY0 > (posPlayer.y - SCREEN_HEIGHT / 2)
			&& */
		if (enemyTreeZone.limit.max_x > posPlayer.x && posPlayer.x >= enemyTreeZone.limit.min_x)
		{
			if (!isInsideEnemyTreeZone) {
				enemyTree = new EnemyTree();
				bool right = posPlayer.x > enemyTreeZone.initPos.x*map->getTileSize();
				enemyTree->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, right);
				enemyTree->setPosition(glm::vec2(enemyTreeZone.initPos.x * map->getTileSize(), (enemyTreeZone.initPos.y) * map->getTileSize()));
				enemyTree->setTileMap(map);
				return true;
			}
			else {
				glm::ivec2 posEnemyTree = enemyTree->getEnemyTreePos();
				if (enemyTreeZone.limit.max_x > posEnemyTree.x && posEnemyTree.x > enemyTreeZone.limit.min_x) {
					return true;
				}
			
			}
		}
	}
	this->enemyTree = NULL;
	return false;
}