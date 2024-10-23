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
	void addStar() { stars += 1; }
	void substractStar() { stars -= 1; }
	void addPoints(int addPoints) { points += addPoints; }
	void setTries(int newTries) { tries = newTries; }
	int getTime() { return time; }

private:
	void initShaders();
	Quad* quad;

	int stars = 5; int starSize = 40; int time = 400;
	int tries = 3;
	int points = 153;
	Texture starTexture;
	TexturedQuad* star;

	Texture bgTexture;
	TexturedQuad* bg;
	ShaderProgram simpleProgram, texProgram;
	int maxTime = 400 * 1000;
	int elapsedTime = 0;
	glm::mat4 projection;
	int textSize = 25;
	Text text;
};
