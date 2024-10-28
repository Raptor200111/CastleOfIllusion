#include "SceneInstructions.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cmath>

SceneInstructions::SceneInstructions()
{
	bgQuad = NULL;
	instructions = NULL;
}

SceneInstructions::~SceneInstructions()
{
	if (bgQuad != NULL)
		delete bgQuad;
	if (instructions != NULL)
		delete instructions;
}

void SceneInstructions::init()
{
	initShaders();
	bgTexture.loadFromFile("images/bgInstructions.jpg", TEXTURE_PIXEL_FORMAT_RGBA);
	bgQuad = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.f, 1.f), &bgTexture, &texProgram);

	instructionsTexture.loadFromFile("images/Instructions.png", TEXTURE_PIXEL_FORMAT_RGBA);
	instructions = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.f, 1.f), &instructionsTexture, &texProgram);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);

	if (!text.init("fonts/retro_computer_personal_use.ttf"))
		cout << "Could not load font!!!" << endl;
}

void SceneInstructions::update(int deltaTime)
{
	// You can handle any specific menu logic here.
}

void SceneInstructions::render()
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

	instructionsTexture.use();
	instructions->render();
//	text.render(title, glm::vec2(0, 25), 30, glm::vec4(1, 1, 1, 1));

}

void SceneInstructions::initShaders()
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

