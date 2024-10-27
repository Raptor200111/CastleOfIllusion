#pragma once

#include "Scene.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Block.h"
#include "BlockGem.h"
#include <vector>
#include "GameUI.h"
#include "Quad.h"
#include <map>

#define SCREEN_X 32
#define SCREEN_Y 16

class ScenePlay : public Scene
{
public:
    ScenePlay();
    virtual ~ScenePlay();

    virtual void init() override;   // Initialize the scene
    virtual void update(int deltaTime) override; // Common update logic
    virtual void render() override; // Common rendering logic

    void reStart();

protected:
    void initShaders();
    vector<vector<Block*>> initBlocks();

    void updateCamera();

    int calcFloorIndex(int posY);
    void insideScreenObj(int floorIndex);
    bool insideScreen(const glm::ivec2& pos);
    void collisionsEnemies(int deltaTime);
    void collisionsMovingBlocks(int deltaTime);

    virtual void reStartLevelSpecific() = 0;
    virtual void updateCollisionsWithBoss(int deltaTime) = 0;
    virtual bool checkIfInsideBossRoom() = 0;
    virtual void collisionMovBlockInsideBossRoom(Block* movBlock) = 0;

    virtual void renderQuadBg() = 0;
    virtual void renderBoss() = 0;

    vector<vector<Enemy*>> playrunEnemies;
    vector<vector<Block*>> playrunBlocks;

    vector<vector<Enemy*>> allEnemies;
    vector<vector<Block*>> allBlocks;

    std::map<std::string, Enemy*> screenEnemies;
    std::map<std::string, Block*> screenBlocks;
    std::map<std::string, Block*> playrunMovBlocks;

    TileMap* map;
    Player* player;

    float currentTime;
    glm::mat4 projection;
    ShaderProgram texProgram, simpleProgram;
    float zoomLevel;
    glm::vec2 cameraPosition;
    Cam cam;

    // Background
    Sprite* bgQuad;
    Texture bgTexture;
    Quad* quad;
    bool winAnimScenePlay = false;
    TileMap* bgMap;

    BlockGem* blockGem;
    GameUI gameUI;
    bool insideBossRoom = false;
   
};
