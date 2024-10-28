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
	bgTexture.loadFromFile("images/bgInstructions.jpg", TEXTURE_PIXEL_FORMAT_RGBA);
	bgQuad = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.f, 1.f), &bgTexture, &texProgram);

	creditsTexture.loadFromFile("images/varied.png", TEXTURE_PIXEL_FORMAT_RGBA);
	credits = Sprite::createSprite(glm::vec2(50, 50), glm::vec2(1.f, 1.f), &creditsTexture, &texProgram);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);

	if (!text.init("fonts/retro_computer_personal_use.ttf"))
		cout << "Could not load font!!!" << endl;
}

void SceneCredits::update(int deltaTime)
{
	// You can handle any specific menu logic here.
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

	creditsTexture.use();
	credits->render();
	//	text.render(title, glm::vec2(0, 25), 30, glm::vec4(1, 1, 1, 1));

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


