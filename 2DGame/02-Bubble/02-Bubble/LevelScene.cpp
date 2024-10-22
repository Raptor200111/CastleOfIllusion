#include "LevelScene.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cmath>
#include "Scene.h"
#include "Game.h"
#include "Enemy.h"
#include "EnemyTree.h"
#include "EnemyBug.h"
#include "CollisionManager.h"

#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 2//+35+36// 4+20
#define INIT_PLAYER_Y_TILES 8//44+2 //20

LevelScene::LevelScene()
{
	map = NULL;
	player = NULL;
	zoomLevel = 2.5f;
	bgMap = NULL;
	bgQuad = NULL;
}

LevelScene::~LevelScene() 
{
	if (map != NULL)
		delete map;
	if (bgMap != NULL)
		delete bgMap;
	if (bgQuad != NULL)
		delete bgQuad;

	//quitar
	for (auto block : blocksObj) {
		delete block;
	}
	blocksObj.clear();
	for (auto e : enemiesObj)
		delete e;
	enemiesObj.clear();
}

void LevelScene::init()
{
	initShaders();
	//sound
	SoundManager::instance().setMusicVolume(64);
	SoundManager::instance().playMusic("level", -1);

	//level
	map = TileMap::createTileMap("levels/levelMatrix.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	CollisionManager::instance().init(map);
	player = &Player::instance();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2((INIT_PLAYER_X_TILES)*map->getTileSize(), (INIT_PLAYER_Y_TILES)*map->getTileSize()));
	player->setTileMap(map);
	updateCamera();
	//initZoneEnemyTree();
	//initZoneEnemyBug();
	
	for (auto block : map->getBlocksPos()) {
		Block* b = new Block();
		//cout << block.pos.x << " " << block.pos.y << "\n";
		b->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, block.type);
		b->setPosition(glm::ivec2(block.pos.x * map->getTileSize(), block.pos.y * map->getTileSize()));
		b->setTileMap(map);
		blocksObj.push_back(b);
	}
	CollisionManager::instance().sceneInit(cam,blocksObj, enemiesObj);
	
	//background
	bgMap = TileMap::createTileMap("levels/bgTileMap.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	bgTexture.loadFromFile("images/portada.png", TEXTURE_PIXEL_FORMAT_RGBA);
	glm::vec2 bgSize =map->getMapSize() * map->getTileSize();
	bgQuad = Sprite::createSprite(bgSize, glm::vec2(1.f, 1.f), &bgTexture, &texProgram);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;
	gameUI.init();


	Zone limit = { 4.0f * map->getTileSize(), 22.0f * map->getTileSize(), 0, 0 };
	glm::ivec2 initPos = glm::ivec2(10.0f, 38.0f);
	ZoneEnemy zone1 = { limit, initPos, false };
	boss.initMov(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, zone1);
	boss.setBossPosition(glm::ivec2(zone1.initPos.x * map->getTileSize(), zone1.initPos.y * map->getTileSize()));
	boss.setTileMap(map);
	//211ms

}

void LevelScene::initZoneEnemyTree()
{
	vector<ZoneEnemy> zones;
	Zone limit = { 4.0f * map->getTileSize(), 22.0f * map->getTileSize(), 0, 0 };
	glm::ivec2 initPos = glm::ivec2(20.0f, 7.0f);
	ZoneEnemy zone1 = {limit, initPos, true };
	zones.push_back(zone1);

	limit = { 26.0f * map->getTileSize(), 38.0f * map->getTileSize(), 0, 0 };
	initPos = glm::ivec2(37.0f, 6.0f);
	ZoneEnemy zone2 = {limit, initPos, true };
	zones.push_back(zone2);

	limit = { 39.0f * map->getTileSize(), 46.0f * map->getTileSize(), 0, 0 };
	initPos = glm::ivec2(45.0f, 7.0f);
	ZoneEnemy zone3 = {limit, initPos, true };
	zones.push_back(zone3);

	limit = { 54.0f * map->getTileSize(), 66.0f * map->getTileSize(), 0, 0 };
	initPos = glm::ivec2(65.0f, 6.0f);
	ZoneEnemy zone4 = {limit, initPos, true };
	zones.push_back(zone4);

	for (auto zone : zones) {
		EnemyTree* enemy = new EnemyTree();
		enemy->initMov(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, zone);
		enemy->setPosition(glm::ivec2(zone.initPos.x * map->getTileSize(), zone.initPos.y * map->getTileSize()));
		enemy->setTileMap(map);
		enemiesObj.push_back(enemy);

	}
}
void LevelScene::initZoneEnemyBug()
{
	vector<ZoneEnemy> zones;

	Zone limit = { 34.0f * map->getTileSize(), 46.0f * map->getTileSize(), 17, 20 };
	glm::ivec2 initPos = glm::ivec2(35.0f, 17.0f);
	ZoneEnemy zoneBug1 = { limit, initPos, false };
	zones.push_back(zoneBug1);

	limit = { 76.0f * map->getTileSize(), 89.0f * map->getTileSize(), 20, 20 };
	initPos = glm::ivec2(88.0f, 20.0f);
	ZoneEnemy zoneBug2 = { limit, initPos, false };
	zones.push_back(zoneBug2);

	limit = { 69.0f * map->getTileSize(), 81.0f * map->getTileSize(), 31, 31 };
	initPos = glm::ivec2(80.0f, 31.0f);
	ZoneEnemy zoneBug3 = { limit, initPos, false };
	zones.push_back(zoneBug3);

	limit = { 26.0f * map->getTileSize(), 37.0f * map->getTileSize(), 32, 32 };
	initPos = glm::ivec2(26.0f, 32.0f);
	ZoneEnemy zoneBug4 = { limit, initPos, false };
	zones.push_back(zoneBug4);

	limit = { 14.0f * map->getTileSize(), 24.0f * map->getTileSize(), 30, 32 };
	initPos = glm::ivec2(14.0f, 30.0f);
	ZoneEnemy zoneBug5 = { limit, initPos, false };
	zones.push_back(zoneBug5);

	for (auto zone : zones) {
		EnemyBug* enemy = new EnemyBug();
		enemy->initMov(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, zone);
		enemy->setPosition(glm::ivec2(zone.initPos.x * map->getTileSize(), zone.initPos.y * map->getTileSize()));
		enemy->setTileMap(map);
		enemiesObj.push_back(enemy);

	}
}

void LevelScene::update(int deltaTime)
{

	player->update(deltaTime);

	updateCamera();
	
	
	CollisionManager::instance().update(deltaTime, cam);
	boss.update(deltaTime);
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

	//background
	
	bgTexture.use();
	bgQuad->render();
	bgMap->render();
	
	//level
	map->render();
	player->render();
	for (const auto& enemy : CollisionManager::instance().enemies) {
		enemy.second->render();
	}

	for (const auto& block : CollisionManager::instance().blocks)
	{
		block.second->render();
	}

	boss.render();
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
	float maxCameraY = map->getMapSize().y * map->getTileSize() - zoomScreenHeight+50;

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