#pragma once
#include "Scene.h"
#include "Sprite.h"
#include "Texture.h"
#include "Text.h"

class SceneInstructions : public Scene
{
public:
    SceneInstructions();
    ~SceneInstructions();
    void init() override;
    void update(int deltaTime) override;
    void render() override;

private:
    void initShaders();
    Sprite* bgQuad;   // Menu quad
    Texture bgTexture;

    Sprite* instructions;   // Menu quad
    Texture instructionsTexture;

    int textSize = 25;
    Text text;

    ShaderProgram texProgram;
    glm::mat4 projection;

};

