#pragma once

#include "Scene.h"
#include "Sprite.h"
#include "Texture.h"
#include "Text.h"


struct TextInfo {
    glm::ivec2 position;
    int size;                  
    glm::vec4 color;  
    std::string content;     
};

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
    void resetPositions();
    Sprite* bgQuad;   
    Texture bgTexture;

    Sprite* credits;   
    Texture creditsTexture;

    int textSize = 25;
    bool reset = false;
    vector<TextInfo> textInfos;
    vector<Text> texts;
    vector<string> textStrings = {"DEVELOPED BY: ", "    LILIU MARTINEZ", "    ALVAR DALDA","",
                                  "SOUNDS: ", "    LILIU MARTINEZ","", "ART: ", "    ALVAR DALDA", "" };


    ShaderProgram texProgram;
    glm::mat4 projection;

};


