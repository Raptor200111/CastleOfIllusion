#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4+20
#define INIT_PLAYER_Y_TILES 20


Scene::Scene()
{
	map = NULL;
	player = NULL;
	bgQuad = NULL;
	menuQuad = NULL;
	blocksByType = std::map<int, std::vector<Block*>>();
}

Scene::~Scene()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
	if (bgQuad != NULL)
		delete bgQuad;
	if (menuQuad != NULL)
		delete menuQuad;
	for (auto blockTypes : blocksByType) {
		for (auto block : blockTypes.second) {
			delete block;
		}
	}
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
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2((INIT_PLAYER_X_TILES)*map->getTileSize(), (INIT_PLAYER_Y_TILES)*map->getTileSize()));
	player->setTileMap(map);

	bgTexture.loadFromFile("images/rocks.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bgQuad = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.f, 1.f), &bgTexture, &texProgram);

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

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
}

void Scene::updateLevel(int deltaTime)
{ 
	player->update(deltaTime);
}

void Scene::scrolling()
{
	glm::ivec2 cam = player->getPlayerPos();
	float halfWidth = SCREEN_WIDTH / 2;
	float halfHeight = SCREEN_HEIGHT / 2;

	double left = cam.x - halfWidth;
	double right = halfWidth + cam.x;
	double bottom = cam.y - halfHeight;//t + 300;
	double top = halfHeight + cam.y;// +300;

	glm::ivec2 mapSize = map->getMapSize();
	int tileSize = map->getTileSize();

	if (left < 0)
	{
		left = 0;
		right = left + SCREEN_WIDTH;
	}
	if (bottom < 0)
	{
		bottom = 0;
		top = bottom + SCREEN_HEIGHT;
	}
	if (top > (mapSize.y + 2) * tileSize)//49 * 16)
	{
		top = (mapSize.y + 2) * tileSize;
		bottom = top - SCREEN_HEIGHT;
	}
	if (right > (mapSize.x + 4) * tileSize)
	{
		right = (mapSize.x + 4) * tileSize;
		left = right - SCREEN_WIDTH;
	}

	projection = glm::ortho(left, right, top, bottom);
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

void Scene::renderLevel()
{
	scrolling();
	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);


	//level
	map->render();
	player->render();
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