#include "SceneFinalScreen.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cmath>

SceneFinalScreen::SceneFinalScreen()
{
	bgQuad = NULL;
	win = NULL;
	lose = NULL;
}

SceneFinalScreen::~SceneFinalScreen()
{
	if (bgQuad != NULL)
		delete bgQuad;
	if (win != NULL)
		delete win;
	if (lose != NULL)
		delete lose;
}

void SceneFinalScreen::init()
{
	initShaders();
	bgTexture.loadFromFile("images/bgInstructions.jpg", TEXTURE_PIXEL_FORMAT_RGBA);
	bgQuad = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.f, 1.f), &bgTexture, &texProgram);

	winTexture.loadFromFile("images/win.jpg", TEXTURE_PIXEL_FORMAT_RGBA);
	win = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.f, 1.f), &winTexture, &texProgram);

	loseTexture.loadFromFile("images/lose.jpg", TEXTURE_PIXEL_FORMAT_RGBA);
	lose = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.f, 1.f), &loseTexture, &texProgram);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);

	if (!text.init("fonts/retro_computer_personal_use.ttf"))
		cout << "Could not load font!!!" << endl;
}

void SceneFinalScreen::update(int deltaTime)
{
	// You can handle any specific menu logic here.
}

void SceneFinalScreen::render()
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

	if (won) {
		winTexture.use();
		win->render();
	}
	else {
		loseTexture.use();
		lose->render();
	}

	//	text.render(title, glm::vec2(0, 25), 30, glm::vec4(1, 1, 1, 1));

}

void SceneFinalScreen::initShaders()
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


