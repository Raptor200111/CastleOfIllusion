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
#define INIT_PLAYER_Y_TILES 8//44//+8+2 //20

LevelScene::LevelScene()
{
	map = NULL;
	player = NULL;
	zoomLevel = 2.5f;
	bgMap = NULL;
	bgQuad = NULL;
	screenEnemies = std::map<string, Enemy*>();
	screenBlocks = std::map<string, Block*>();
	playrunMovBlocks = std::map<string, Block*>();

}

LevelScene::~LevelScene()
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
	initZoneEnemyTree();
	initZoneEnemyBug();

	for (auto block : map->getBlocksPos()) {
		Block* b = new Block();
		//cout << block.pos.x << " " << block.pos.y << "\n";
		b->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, block.type);
		b->setPosition(glm::ivec2(block.pos.x * map->getTileSize(), block.pos.y * map->getTileSize()));
		b->setTileMap(map);
		allBlocks.push_back(b);
	}
	playrunBlocks = allBlocks;
	playrunEnemies = allEnemies;

	//background
	bgMap = TileMap::createTileMap("levels/bgTileMap.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	bgTexture.loadFromFile("images/portada.png", TEXTURE_PIXEL_FORMAT_RGBA);
	glm::vec2 bgSize = map->getMapSize() * map->getTileSize();
	bgQuad = Sprite::createSprite(bgSize, glm::vec2(1.f, 1.f), &bgTexture, &texProgram);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;
	gameUI.init();
	gameUI.setMaxTime(400 * 1000);


	Zone limit = { 4.0f * map->getTileSize(), 22.0f * map->getTileSize(), 0, 0 };
	glm::ivec2 finalPosBoss = glm::ivec2(10.0f * map->getTileSize(), 38.0f * map->getTileSize());
	ZoneEnemy zone1 = { limit, finalPosBoss, false };
	boss.initMov(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, zone1);
	glm::ivec2 initPos = glm::ivec2(finalPosBoss.x, finalPosBoss.y - 110);
	boss.setBossPosition(initPos);
	boss.setTileMap(map);
	//211ms

}

void LevelScene::initZoneEnemyTree()
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

	for (auto zone : zones) {
		EnemyTree* enemy = new EnemyTree();
		enemy->initMov(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, zone);
		enemy->setPosition(glm::ivec2(zone.initPos.x * map->getTileSize(), zone.initPos.y * map->getTileSize()));
		enemy->setTileMap(map);
		allEnemies.push_back(enemy);

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
		allEnemies.push_back(enemy);

	}
}

void LevelScene::update(int deltaTime)
{
	//update screenBlocks and screenEnemies
	insideScreenObj();


	CollisionManager::instance().update(screenBlocks);
	player->update(deltaTime);
	updateCamera();

	/*SHOULD playrunBlocks+allBlocks BE MAP????
	Block* a = player->PickedUpBlock();
	if (a != NULL)
	{
		//search a in playrunBlocks
		//remove it from playrunBlocks
	}
	//PULLING TO CHECK IF PLAYER THREW ANY BLOCK
	Block* b = player->ThrownBlock();
	if (b != NULL) {
		//search b in allBlocks
		//once found add it to movBlocks
	}
	*/
	if (insideBossRoom) {
		boss.update(deltaTime);
		for (auto shoot : boss.getShoots())
		{
			shoot->update(deltaTime);
			if (shoot->getEntityState() == Alive) {
				if (CollisionManager::instance().checkCollisionObject(player, shoot)) {
					shoot->setEntityState(Dying);
				}
			}
			
			if (shoot->getEntityState() == Alive){
				glm::ivec2 posShoot = shoot->getPosition();
				if(CollisionManager::instance().checkCollisionVertical(shoot) != None || CollisionManager::instance().checkCollisionHorizontal(shoot) != None)
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
	else {
		for (auto screenEnemy : screenEnemies) {
			//enemy is alive, dying or deadTree waiting to regenerate
			screenEnemy.second->update(deltaTime);
		}
		//collisions alive enemies: blocks, tiles, player
		for (auto& itEnemy = screenEnemies.begin(); itEnemy != screenEnemies.end(); ++itEnemy)
		{
			if (itEnemy->second->getEntityState() == Alive) {
				if (player->getEntityState()== Alive && CollisionManager::instance().checkCollisionObject(player, itEnemy->second)) {
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
	if (insideBossRoom) {
		boss.render();
		for (auto shoot : shoots)
		{
			if(shoot->getEntityState() != Dead)
				shoot->render();
		}
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


void LevelScene::insideScreenObj()
{
	int tileSize = map->getTileSize();
	glm::ivec2 posP = player->getPosition();
	int tilesize = map->getTileSize();
	Cam bossRoom = { 0, map->getMapSize().x * tilesize, map->getMapSize().y * tilesize, 35 * tilesize };
	if (bossRoom.left < posP.x && posP.x < bossRoom.right && bossRoom.top < posP.y && posP.y < bossRoom.bottom) {
		insideBossRoom = true;
		boss.setEntityState(Alive);
		shoots.clear();
		for (const auto& shoot : boss.getShoots())
		{
			if (shoot->getEntityState() != Dead)
				shoots.push_back(shoot);
		}
	}
	else {
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

bool LevelScene::insideScreen(const glm::ivec2& pos)
{

	if (cam.left < pos.x && pos.x < cam.right && cam.top < pos.y && pos.y < cam.bottom) {
		return true;
	}
	return false;
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