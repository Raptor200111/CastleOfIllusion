#pragma once

#include "PlayScene.h"

class PracticePlayScene : public PlayScene
{
public:
    PracticePlayScene() = default;
    ~PracticePlayScene() = default;

    void init() override;   // Initialize NormalPlayScene-specific elements

protected:
    // Override these methods to handle boss logic and room detection for NormalPlayScene
    void updateCollisionsWithBoss(int deltaTime) override;
    bool checkIfInsideBossRoom() override;
    void renderBoss() override;
    void reStartLevelSpecific() override {}
};