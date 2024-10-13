#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Block.h"
#include "EnemyTree.h"
// Scene contains all the entities of our game.
// It is responsible for updating and render them.

struct EnemyZone {
	float x0, x1, enemyX0, enemyY0;
};
struct Cam {
	float left, right, bottom, top;
};

class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void initMenu();
	void initLevel();
	void update(int deltaTime);
	void renderMenu();
	void renderLevel();
	void updateLevel(int deltaTime);
private:
	void initShaders();
	void scrolling();
	bool insideEnemyTreeZone(glm::ivec2& posPlayer);

	Sprite* menuQuad;   // Single textured quad
	Texture menuTexture;

	Sprite* bgQuad;
	Texture bgTexture;
	TileMap* map;
	Player* player;
	std::map<int, std::vector<Block*>> blocksByType;

	Cam camDimensions;
	bool isInsideEnemyTreeZone;
	EnemyTree* enemyTree;
	vector<EnemyZone> enemyTreeZones;

	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;


};


#endif // _SCENE_INCLUDE

