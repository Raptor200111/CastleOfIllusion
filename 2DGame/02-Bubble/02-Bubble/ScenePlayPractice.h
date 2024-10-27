#pragma once

#include "ScenePlay.h"

class ScenePlayPractice : public ScenePlay
{
public:
    ScenePlayPractice() = default;
    ~ScenePlayPractice() = default;

    void init() override;   // Initialize NormalPlayScene-specific elements

protected:
    // Override these methods to handle boss logic and room detection for NormalPlayScene
    void reStartLevelSpecific() override;

    void updateCollisionsWithBoss(int deltaTime) override;
    bool checkIfInsideBossRoom() override;
    void collisionMovBlockInsideBossRoom(Block* movBlock) override;

    void renderQuadBg() override;
    void renderBoss() override;

    void initZoneEnemyTree();
    void initZoneEnemyBug();

};