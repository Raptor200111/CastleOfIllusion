#include "ScenePlayLevel.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cmath>
#include "Game.h"
#include "Enemy.h"
#include "EnemyTree.h"
#include "EnemyBug.h"

#include "CollisionManager.h"

#define INIT_PLAYER_X_TILES 79//+35+36// 4+20
#define INIT_PLAYER_Y_TILES 51//44//+8+2 //20

ScenePlayLevel::ScenePlayLevel()
{
	shoots = vector<BossShoot*>();
}

ScenePlayLevel::~ScenePlayLevel()
{
	shoots.clear();
}

void ScenePlayLevel::init()
{
	ScenePlay::init();
	//sound
	SoundManager::instance().setMusicVolume(64);
	//SoundManager::instance().playMusic("level", -1);

	//level
	map = TileMap::createTileMap("levels/levelMatrix.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	CollisionManager::instance().init(map);
	glm::ivec2 mapSize = map->getMapSize();
	int tileSize = map->getTileSize();

	player = &Player::instance();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2((INIT_PLAYER_X_TILES)*tileSize, (INIT_PLAYER_Y_TILES)*tileSize));
	player->setTileMap(map);
	updateCamera();
	initZoneEnemyTree();
	initZoneEnemyBug();

	initBlocks();
	playrunBlocks = allBlocks;
	playrunEnemies = allEnemies;

	//background
	bgMap = TileMap::createTileMap("levels/bgTileMap.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	bgTexture.loadFromFile("images/portada.png", TEXTURE_PIXEL_FORMAT_RGBA);
	glm::vec2 bgSize = map->getMapSize() * tileSize;
	bgQuad = Sprite::createSprite(bgSize, glm::vec2(1.f, 1.f), &bgTexture, &texProgram);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;
	gameUI.init();
	gameUI.setMaxTime(400 * 1000);

	Zone limit = { 79.0f * tileSize, mapSize.x * tileSize, 0, 0 };
	glm::ivec2 finalPosBoss = glm::ivec2(87.0f * tileSize, 45.0f * tileSize);
	//Zone limit = { (mapSize.x - 19) * tileSize, mapSize.x * tileSize, 0, 0 };
	//glm::ivec2 finalPosBoss = glm::ivec2((mapSize.x - 9) * tileSize, (mapSize.y-9) * tileSize);
	ZoneEnemy zone1 = { limit, finalPosBoss, false };
	boss.initMov(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, zone1);
	initPosBoss = glm::ivec2(finalPosBoss.x, finalPosBoss.y - 110);
	boss.setBossPosition(initPosBoss);
	boss.setTileMap(map);
	//bossRoom = { float((mapSize.x-19) * tileSize), float(mapSize.x * tileSize), float(mapSize.y * tileSize), float(40 * tileSize) };
	bossRoom = { float(79.0f * tileSize), float(mapSize.x * tileSize), float(mapSize.y * tileSize), float(45 * tileSize) };

	//211ms

}

void ScenePlayLevel::reStartLevelSpecific()
{
	int tileSize = map->getTileSize();
	CollisionManager::instance().init(map);
	player->setPosition(glm::vec2((INIT_PLAYER_X_TILES)*tileSize, (INIT_PLAYER_Y_TILES)*tileSize));
	player->setTileMap(map);
	updateCamera();
	gameUI.resetTime();
	boss.setBossPosition(initPosBoss);
}
void ScenePlayLevel::initZoneEnemyTree()
{
	vector<ZoneEnemy> zones;
	Zone limit = { 3.0f * map->getTileSize(), 22.0f * map->getTileSize(), 0, 12 };
	glm::ivec2 initPos = glm::ivec2(20.0f, 7.0f);
	ZoneEnemy zone1 = { limit, initPos, true };
	zones.push_back(zone1);

	limit = { 28.0f * map->getTileSize(), 46.0f * map->getTileSize(), 0, 12 };
	initPos = glm::ivec2(45.0f, 7.0f);
	ZoneEnemy zone3 = { limit, initPos, true };
	zones.push_back(zone3);

	limit = { 28.0f * map->getTileSize(), 53.0f * map->getTileSize(), 0, 12 };
	initPos = glm::ivec2(45+7.0f, 6-4.0f);
	ZoneEnemy zone2 = { limit, initPos, true };
	zones.push_back(zone2);

	limit = { 54.0f * map->getTileSize(), 66.0f * map->getTileSize(), 0, 12 };
	initPos = glm::ivec2(65.0f, 6.0f);
	ZoneEnemy zone4 = { limit, initPos, true };
	zones.push_back(zone4);

	for (const auto& zone : zones) {
		EnemyTree* enemy = new EnemyTree();
		enemy->initMov(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, zone);
		enemy->setPosition(glm::ivec2(zone.initPos.x * map->getTileSize(), zone.initPos.y * map->getTileSize()));
		enemy->setTileMap(map);
		allEnemies.push_back(enemy);

	}
}
void ScenePlayLevel::initZoneEnemyBug()
{
	vector<ZoneEnemy> zones;
	Zone limit = { 34.0f * map->getTileSize(), 46.0f * map->getTileSize(), 19, 22 };
	glm::ivec2 initPos = glm::ivec2(35.0f, 19.0f);
	ZoneEnemy zoneBug1 = { limit, initPos, false };
	zones.push_back(zoneBug1);

	limit = { 76.0f * map->getTileSize(), 89.0f * map->getTileSize(), 22, 22 };
	initPos = glm::ivec2(88.0f, 22.0f);
	ZoneEnemy zoneBug2 = { limit, initPos, false };
	zones.push_back(zoneBug2);

	limit = { 69.0f * map->getTileSize(), 81.0f * map->getTileSize(), 35, 35 };
	initPos = glm::ivec2(80.0f, 35.0f);
	ZoneEnemy zoneBug3 = { limit, initPos, false };
	zones.push_back(zoneBug3);

	limit = { 26.0f * map->getTileSize(), 37.0f * map->getTileSize(), 36, 36 };
	initPos = glm::ivec2(26.0f, 36.0f);
	ZoneEnemy zoneBug4 = { limit, initPos, false };
	zones.push_back(zoneBug4);

	limit = { 14.0f * map->getTileSize(), 24.0f * map->getTileSize(), 30, 32 };
	initPos = glm::ivec2(14.0f, 30.0f);
	ZoneEnemy zoneBug5 = { limit, initPos, false };
	zones.push_back(zoneBug5);

	for (const auto& zone : zones) {
		EnemyBug* enemy = new EnemyBug();
		enemy->initMov(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, zone);
		enemy->setPosition(glm::ivec2(zone.initPos.x * map->getTileSize(), zone.initPos.y * map->getTileSize()));
		enemy->setTileMap(map);
		allEnemies.push_back(enemy);

	}
}

bool ScenePlayLevel::checkIfInsideBossRoom() {
	bool inside = false;
	glm::ivec2 posP = player->getPosition();
	if (bossRoom.left <= posP.x && posP.x < bossRoom.right && bossRoom.top < posP.y && posP.y < bossRoom.bottom) {
		inside = true;
		boss.setEntityState(Alive);
		shoots.clear();
		for (const auto& shoot : boss.getShoots())
		{
			if (shoot->getEntityState() != Dead)
				shoots.push_back(shoot);
		}
	}
	if (inside)
		return true;
	else 
		return false;
}

void ScenePlayLevel::updateCollisionsWithBoss(int deltaTime) {
	boss.update(deltaTime);
	for (auto shoot : boss.getShoots())
	{
		shoot->update(deltaTime);
		//add if !godMode
		if (shoot->getEntityState() == Alive) {
			if (CollisionManager::instance().checkCollisionObject(player, shoot)) {
				shoot->setEntityState(Dying);
			}
		}

		if (shoot->getEntityState() == Alive) {
			glm::ivec2 posShoot = shoot->getPosition();
			if (CollisionManager::instance().checkCollisionVertical(shoot) != None || CollisionManager::instance().checkCollisionHorizontal(shoot) != None)
				shoot->setEntityState(Dying);
		}

		if (shoot->getEntityState() == Alive) {
			for (auto& itBlock = screenBlocks.begin(); itBlock != screenBlocks.end(); ++itBlock)
			{
				VColType vBlockCollision = CollisionManager::instance().checkCollisionBlockVertical(shoot, itBlock->second);
				if (CollisionManager::instance().checkCollisionBlockVertical(shoot, itBlock->second) != NoVcol || CollisionManager::instance().checkCollisionBlockHorizontal(shoot, itBlock->second) != NoHcol)
				{
					shoot->setEntityState(Dying);
					break;
				}
			}
		}
	}
}

void ScenePlayLevel::renderBoss() {
	boss.render();
	//cout << "player: " << player->getPosition().x << " " << player->getPosition().y << "\n";
	for (auto shoot : shoots)
	{
		if (shoot->getEntityState() != Dead)
			shoot->render();
	}
}