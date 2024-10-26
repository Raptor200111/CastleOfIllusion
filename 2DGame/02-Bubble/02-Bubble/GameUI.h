#pragma once

#include "Scene.h"
#include "Quad.h"
#include "Sprite.h"
#include "TexturedQuad.h"
#include "Text.h"

class GameUI : public Scene
{
public:
	GameUI();
	~GameUI();
	void init() override;
	void update(int deltaTime) override;
	void render() override;
	void setMaxTime(int level_maxTime) { maxTime = level_maxTime; time = maxTime / 1000; }
	void resetTime() { time = 0; }

private:
	void initShaders();
	Quad* quad;

	int starSize = 40;
	int time;
	int maxTime; 
	Texture starTexture;
	TexturedQuad* star;

	Texture bgTexture;
	TexturedQuad* bg;
	ShaderProgram simpleProgram, texProgram;
	int elapsedTime = 0;
	glm::mat4 projection;
	int textSize = 25;
	Text text;
};
