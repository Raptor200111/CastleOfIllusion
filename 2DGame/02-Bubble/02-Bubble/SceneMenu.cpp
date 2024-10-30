#include "SceneMenu.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include <iostream>
#include <cmath>

SceneMenu::SceneMenu()
{
	menuQuad = NULL;
}

SceneMenu::~SceneMenu()
{
	if (menuQuad != NULL)
		delete menuQuad;
}

void SceneMenu::init()
{ 
	initShaders();
    menuTexture.loadFromFile("images/portada_clean.png", TEXTURE_PIXEL_FORMAT_RGBA);
    menuQuad = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.f, 1.f), &menuTexture, &texProgram);
    projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	
	//optionsText = { Text(), Text(), Text(), Text() };
	optionsText = vector<Text>(4);
	
	for (int i = 0; i < optionsText.size(); i++)
	{
		//optionsText.push_back(Text());
		optionsText[i].init("fonts/OpenSans-Regular.ttf");
	}
}

void SceneMenu::update(int deltaTime)
{
	if (Game::instance().getKey(GLFW_KEY_W))
	{
		if (!buttonPressed[0])
		{
			selectedOption--;
			if (selectedOption < 0)
				selectedOption = 3;
			buttonPressed[0] = true;
		}
	}
	else
		buttonPressed[0] = false;

	if (Game::instance().getKey(GLFW_KEY_S))
	{
		if (!buttonPressed[1])
		{
			selectedOption++;
			if (selectedOption > 3)
				selectedOption = 0;
			buttonPressed[1] = true;
		}
	}
	else
		buttonPressed[1] = false;

	if (Game::instance().getKey(GLFW_KEY_K))
	{
		if (!buttonPressed[2])
		{
			switch (selectedOption)
			{
			case 0:
				Game::instance().setScene(GameState::PLAY_PRACTICE);
				break;
			case 1:
				Game::instance().setScene(GameState::INSTRUCTIONS);
				break;
			case 2:
				Game::instance().setScene(GameState::CREDITS);
				break;
			case 3:
				Game::instance().exitGame();
				break;
			}
			buttonPressed[2] = true;
		}
	}
	else
		buttonPressed[2] = false;
}

void SceneMenu::render()
{
	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	//menu
	menuTexture.use();
	menuQuad->render();

	for (int i = 0; i < optionsText.size(); i++)
	{
		int size = 30;
		int x = 320 - (texts[i].size() / 2 * size / 2);
		int y = 240 - size * 2 + (i * size);
		
		if (i == selectedOption)
			optionsText[i].render("> " + texts[i] + " <", glm::vec2(x - size, y), size, glm::vec4(1, 1, 1, 1));
		else
			optionsText[i].render(texts[i], glm::vec2(x,y), size, glm::vec4(1, 1, 1, 1));
	}
}

void SceneMenu::initShaders()
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
