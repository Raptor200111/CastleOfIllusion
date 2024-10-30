#ifndef _MENU_SCENE_INCLUDE
#define _MENU_SCENE_INCLUDE

#include "Scene.h"
#include "Sprite.h"
#include "Texture.h"
#include "Text.h"
#include <iostream>
#include <vector>

class SceneMenu : public Scene
{
public:
    SceneMenu();
    ~SceneMenu();
    void init() override;
    void update(int deltaTime) override;
    void render() override;

private:
    void initShaders();
    Sprite* menuQuad;   // Menu quad
    Texture menuTexture;
    ShaderProgram texProgram;
    glm::mat4 projection;
    int selectedOption = 0;
    bool buttonPressed[3] = {false, false, false};
    vector<Text> optionsText;// = { Text(), Text(), Text(), Text() };
    vector<string> texts = { "Play", "Instructions", "Credits", "Exit" };
};

#endif // _MENU_SCENE_INCLUDE
