#include "GameUI.h"
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include <iomanip>
#include <sstream>

GameUI::GameUI()
{
	quad = NULL;
	star = NULL;
	bg = NULL;
}

GameUI::~GameUI()
{
	if (quad != NULL)
		delete quad;
	if (star != NULL)
		delete star;
	if (bg != NULL)
		delete bg;
}


void GameUI::init()
{
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	initShaders();
	quad = Quad::createQuad(0.f, SCREEN_HEIGHT - 77.f, SCREEN_WIDTH, SCREEN_HEIGHT, simpleProgram);
	
	glm::vec2 geomBg[2] = { glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, 77) };
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f);
	bg = TexturedQuad::createTexturedQuad(geomBg, texCoords, texProgram);

	glm::vec2 geomStar[2] = { glm::vec2(0.f, 0.f), glm::vec2(starSize, starSize) };
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.0f);
	star = TexturedQuad::createTexturedQuad(geomStar, texCoords, texProgram);

	starTexture.loadFromFile("images/powerStar.png", TEXTURE_PIXEL_FORMAT_RGBA);
	starTexture.setMagFilter(GL_NEAREST);
	bgTexture.loadFromFile("images/GameUI.png", TEXTURE_PIXEL_FORMAT_RGB);
	bgTexture.setMagFilter(GL_NEAREST);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);

	// Select which font you want to use
		//if (!text.init("fonts/OpenSans-Regular.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
	if (!text.init("fonts/retro_computer_personal_use.ttf"))
		cout << "Could not load font!!!" << endl;

		
}

void GameUI::update(int deltaTime)
{
	elapsedTime += deltaTime;
	if (elapsedTime >= maxTime) {
		//die maximum time;
		Game::instance().onExceededTimeLimit();
		elapsedTime = 0;
	}
	time = (maxTime-elapsedTime)/1000;
}

void GameUI::render()
{
	glm::mat4 modelview;

	simpleProgram.use();
	simpleProgram.setUniformMatrix4f("projection", projection);
	simpleProgram.setUniform4f("color", 0.f, 0.f, 0.f, 1.0f);

	modelview = glm::mat4(1.0f);
	simpleProgram.setUniformMatrix4f("modelview", modelview);
	//quad->render();

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	modelview = glm::mat4(1.0f);
	/*
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(384.f, 304.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	modelview = glm::rotate(modelview, -currentTime / 1000.f, glm::vec3(0.0f, 0.0f, 1.0f));
	*/
	modelview = glm::translate(modelview, glm::vec3(0.f, SCREEN_HEIGHT - 77, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	bg->render(bgTexture);

	int stars = Game::instance().getStars();
	for (int i = 0; i < stars; i++)
	{
		modelview = glm::mat4(1.0f);
		//modelview = glm::translate(glm::mat4(1.0f), glm::vec3(384.f, 48.f, 0.f));
		//modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
		//modelview = glm::rotate(modelview, currentTime / 1000.f, glm::vec3(0.0f, 0.0f, 1.0f));
		modelview = glm::translate(modelview, glm::vec3(35+i*starSize, SCREEN_HEIGHT - starSize - 15, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		star->render(starTexture);
	}

	text.render("0", glm::vec2(295, SCREEN_HEIGHT - 20), textSize, glm::vec4(1, 1, 1, 1));
	text.render(std::to_string(Game::instance().getTries()), glm::vec2(295+ textSize, SCREEN_HEIGHT - 20), textSize, glm::vec4(1, 1, 1, 1));

	std::ostringstream oss;
	oss << std::setw(6) << std::setfill('0') << Game::instance().getScore();
	std::string numStr = oss.str();
	text.render(numStr, glm::vec2(380, SCREEN_HEIGHT - 20), textSize, glm::vec4(1, 1, 1, 1));

	std::ostringstream oss2;
	oss2 << std::setw(3) << std::setfill('0') << time;
	std::string timeStr = oss2.str();
	text.render(timeStr, glm::vec2(530, SCREEN_HEIGHT - 20), textSize, glm::vec4(1, 1, 1, 1));
}

void GameUI::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/simple.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/simple.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleProgram.init();
	simpleProgram.addShader(vShader);
	simpleProgram.addShader(fShader);
	simpleProgram.link();
	if (!simpleProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleProgram.log() << endl << endl;
	}
	simpleProgram.bindFragmentOutput("outColor");

	vShader.free();
	fShader.free();
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
}