#include "ScenePlayPractice.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include "TileMap.h"
#include "EnemyBug.h"
#include "EnemyTree.h"
#include <iostream>
#include "Game.h"

#define PRACTICE_INIT_PLAYER_X_TILES 2
#define PRACTICE_INIT_PLAYER_Y_TILES 6

void ScenePlayPractice::init() {
    // Call the base class init() to initialize common elements
    ScenePlay::init();
    SoundManager::instance().setMusicVolume(64);

    // Initialize the map and other specific elements for NormalPlayScene
    map = TileMap::createTileMap("levels/practiceMatrix.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
    CollisionManager::instance().init(map);
    glm::ivec2 mapSize = map->getMapSize();
    int tileSize = map->getTileSize();
    player = &Player::instance();
    player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
    player->setPosition(glm::vec2((PRACTICE_INIT_PLAYER_X_TILES)*tileSize, (PRACTICE_INIT_PLAYER_Y_TILES)*tileSize));
    player->setTileMap(map);
    updateCamera();
    //init enemies
    allEnemies = vector < vector<Enemy*>>(1);
    initZoneEnemyTree();
    initZoneEnemyBug();

    allBlocks = initBlocks();
    blockGem = new BlockGem();
    blockGem->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
    blockGem->setPosition(glm::vec2(91*tileSize, 6*tileSize));
    blockGem->setTileMap(map);
    playrunBlocks = allBlocks;
    playrunEnemies = allEnemies;

    bgMap = NULL;
    bgTexture.loadFromFile("images/portada.png", TEXTURE_PIXEL_FORMAT_RGBA);
    glm::vec2 bgSize = map->getMapSize() * tileSize;
    bgQuad = Sprite::createSprite(bgSize, glm::vec2(1.f, 1.f), &bgTexture, &texProgram);

    projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
    currentTime = 0.0f;
    gameUI.init();
    gameUI.setMaxTime(200 * 1000);
}

void ScenePlayPractice::reStartLevelSpecific()
{
    int tileSize = map->getTileSize();
    CollisionManager::instance().init(map);
    player->setPosition(glm::vec2((PRACTICE_INIT_PLAYER_X_TILES)*tileSize, (PRACTICE_INIT_PLAYER_Y_TILES)*tileSize));
    player->setTileMap(map);
    updateCamera();
    gameUI.resetTime();
}

void ScenePlayPractice::initZoneEnemyTree()
{
    vector<ZoneEnemy> zones;
    Zone limit = { 3.0f * map->getTileSize(), 22.0f * map->getTileSize(), 0, 12 };
    glm::ivec2 initPos = glm::ivec2(20.0f, 4.0f);
    ZoneEnemy zone1 = { limit, initPos, true };
    zones.push_back(zone1);

    limit = { 26.0f * map->getTileSize(), 45.0f * map->getTileSize(), 0, 12 };
    initPos = glm::ivec2(45.0f, 5.0f);
    ZoneEnemy zone2 = { limit, initPos, true };
    zones.push_back(zone2);

    limit = { 57.0f * map->getTileSize(), 75.0f * map->getTileSize(), 0, 12 };
    initPos = glm::ivec2(75.0f, 5.0f);
    ZoneEnemy zone3 = { limit, initPos, true };
    zones.push_back(zone3);

    for (const auto& zone : zones) {
        EnemyTree* enemy = new EnemyTree();
        enemy->initMov(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, zone);
        enemy->setPosition(glm::ivec2(zone.initPos.x * map->getTileSize(), zone.initPos.y * map->getTileSize()));
        enemy->setTileMap(map);
        allEnemies[0].push_back(enemy);

    }
}
void ScenePlayPractice::initZoneEnemyBug()
{
    vector<ZoneEnemy> zones;

    Zone limit = { 26.0f * map->getTileSize(), 30.0f * map->getTileSize(), 0, 12 };
    glm::ivec2 initPos = glm::ivec2(26.f, 7.f);
    ZoneEnemy zoneBug1 = { limit, initPos, false };
    zones.push_back(zoneBug1);

    limit = { 46.0f * map->getTileSize(), 54.0f * map->getTileSize(), 0, 12 };
    initPos = glm::ivec2(53.0f, 7.0f);
    ZoneEnemy zoneBug2 = { limit, initPos, false };
    zones.push_back(zoneBug2);

    for (const auto& zone : zones) {
        EnemyBug* enemy = new EnemyBug();
        enemy->initMov(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, zone);
        enemy->setPosition(glm::ivec2(zone.initPos.x * map->getTileSize(), zone.initPos.y * map->getTileSize()));
        enemy->setTileMap(map);
        allEnemies[0].push_back(enemy);

    }
}

void ScenePlayPractice::updateCollisionsWithBoss(int deltaTime) {
    if (player->getEntityState() == Alive) {
        if (CollisionManager::instance().checkCollisionObject(player, blockGem)) {
            Game::instance().onPracticeLevelWon();
        }
    }
}

bool ScenePlayPractice::checkIfInsideBossRoom() { 
    if (cam.right > blockGem->getPosition().x) {
        return true;
    }
    return false; 
}
void ScenePlayPractice::collisionMovBlockInsideBossRoom(Block* movBlock)
{

}

void ScenePlayPractice::renderBoss() {
    blockGem->render();
}

