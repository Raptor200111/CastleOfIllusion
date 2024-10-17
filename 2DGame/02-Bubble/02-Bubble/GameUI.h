#pragma once

#include "Scene.h"
#include "Quad.h"
#include "Sprite.h"
#include "TexturedQuad.h"
//#include "Text.h"

class GameUI : public Scene
{
public:
	GameUI();
	~GameUI();
	void init() override;
	void update(int deltaTime) override;
	void render() override;

private:
	void initShaders();
	Quad* quad;

	Texture starTexture;
	TexturedQuad* star;

	Texture bgTexture;
	TexturedQuad* bg;
	ShaderProgram simpleProgram, texProgram;
	float currentTime;
	glm::mat4 projection;
	//Text text;
};

