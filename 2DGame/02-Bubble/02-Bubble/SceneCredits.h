#pragma once

#include "Scene.h"
#include "Sprite.h"
#include "Texture.h"
#include "Text.h"

class SceneCredits : public Scene
{
public:
    SceneCredits();
    ~SceneCredits();
    void init() override;
    void update(int deltaTime) override;
    void render() override;

private:
    void initShaders();
    Sprite* bgQuad;   // Menu quad
    Texture bgTexture;

    Sprite* credits;   // Menu quad
    Texture creditsTexture;

    int textSize = 25;
    Text text;

    ShaderProgram texProgram;
    glm::mat4 projection;

};


