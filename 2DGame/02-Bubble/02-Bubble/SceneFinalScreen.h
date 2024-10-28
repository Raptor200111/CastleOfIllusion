#pragma once

#include "Scene.h"
#include "Sprite.h"
#include "Texture.h"
#include "Text.h"

class SceneFinalScreen : public Scene
{
public:
    SceneFinalScreen();
    ~SceneFinalScreen();
    void init() override;
    void update(int deltaTime) override;
    void render() override;
    void setWon(bool b) { won = b; }

private:
    void initShaders();
    Sprite* bgQuad;   // Menu quad
    Texture bgTexture;

    Sprite* win;   // Menu quad
    Texture winTexture;

    Sprite* lose;   // Menu quad
    Texture loseTexture;

    int textSize = 25;
    Text text;
    bool won = false;

    ShaderProgram texProgram;
    glm::mat4 projection;

};




