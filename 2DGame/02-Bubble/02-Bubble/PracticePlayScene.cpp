#include "PracticePlayScene.h"
#include "TileMap.h"
#include <iostream>

void PracticePlayScene::init() {
    // Call the base class init() to initialize common elements
    PlayScene::init();

    // Initialize the map and other specific elements for NormalPlayScene
    map = TileMap::createTileMap("levels/normalLevelMatrix.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
    std::cout << "NormalPlayScene: Loaded normalLevelMatrix.txt" << std::endl;

    // Initialize specific enemies, blocks, player settings for NormalPlayScene
}

bool PracticePlayScene::checkIfInsideBossRoom() {
    return false;
}

void PracticePlayScene::updateCollisionsWithBoss(int deltaTime) {
}

void PracticePlayScene::renderBoss() {
}

