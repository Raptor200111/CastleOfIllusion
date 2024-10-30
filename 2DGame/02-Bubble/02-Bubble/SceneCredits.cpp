#include "SceneCredits.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cmath>

SceneCredits::SceneCredits()
{
	bgQuad = NULL;
	credits = NULL;
}

SceneCredits::~SceneCredits()
{
	if (bgQuad != NULL)
		delete bgQuad;
	if (credits != NULL)
		delete credits;
}

void SceneCredits::init()
{
	initShaders();
	bgTexture.loadFromFile("images/portada_clean.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bgQuad = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.f, 1.f), &bgTexture, &texProgram);

	//creditsTexture.loadFromFile("images/varied.png", TEXTURE_PIXEL_FORMAT_RGBA);
	//credits = Sprite::createSprite(glm::vec2(50, 50), glm::vec2(1.f, 1.f), &creditsTexture, &texProgram);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);

	texts = vector<Text>(textStrings.size());
	int yPos = SCREEN_HEIGHT + 50;
	for (int i = 0; i < textStrings.size(); i++) {
		if (!texts[i].init("fonts/retro_computer_personal_use.ttf"))
			cout << "Could not load font!!!" << endl;
		glm::ivec2 position = glm::vec2(50, yPos);
		int size = textSize;
		glm::vec4 color = glm::vec4(1, 1, 1, 1);
		string content = textStrings[i];
		TextInfo info = { position, size, color, content };
		textInfos.push_back(info);
		yPos += textSize + 20;
	}
}

void SceneCredits::resetPositions()
{
	int yPos = SCREEN_HEIGHT + 50;
	for (int i = 0; i < textInfos.size(); i++) {
		textInfos[i].position.y = yPos;
		yPos += textSize + 20;
	}
}

void SceneCredits::update(int deltaTime)
{
	int scrollSpeed = 50;  
	
	for (int i = 0; i < texts.size(); i++)
	{//if(textInfos[i].position.y >= -50.f)
		textInfos[i].position.y -= scrollSpeed * (deltaTime / 1000.f);  
	}
	if (textInfos[textInfos.size() - 1].position.y <= 185) {
		resetPositions();
	}
}

void SceneCredits::render()
{
	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	//background
	bgTexture.use();
	bgQuad->render();

	//creditsTexture.use();
	//credits->render();

	for (int i = 0; i<texts.size(); i++)
	{
		if(textInfos[i].position.y >185)
			texts[i].render(textInfos[i].content, textInfos[i].position, textInfos[i].size, textInfos[i].color);
	}
}

void SceneCredits::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}


