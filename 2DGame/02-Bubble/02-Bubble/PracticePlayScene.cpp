#include "ScenePlayPractice.h"
#include "TileMap.h"
#include <iostream>

void ScenePlayPractice::init() {
    // Call the base class init() to initialize common elements
    ScenePlay::init();

    // Initialize the map and other specific elements for NormalPlayScene
    map = TileMap::createTileMap("levels/normalLevelMatrix.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
    std::cout << "NormalPlayScene: Loaded normalLevelMatrix.txt" << std::endl;

    // Initialize specific enemies, blocks, player settings for NormalPlayScene
}

bool ScenePlayPractice::checkIfInsideBossRoom() {
    return false;
}

void ScenePlayPractice::updateCollisionsWithBoss(int deltaTime) {
}

void ScenePlayPractice::renderBoss() {
}

