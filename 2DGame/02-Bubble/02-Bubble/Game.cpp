#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	currentState = MENU;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	scene.init();
}

bool Game::update(int deltaTime)
{
	if (currentState ==  PLAY)
	{
		scene.updateLevel(deltaTime);
	}
	scene.update(deltaTime);

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (currentState == MENU) {
		scene.renderMenu();
	}
	else{
		scene.renderLevel();
	}
}

void Game::keyPressed(int key)
{
	if(key == GLFW_KEY_ESCAPE) // Escape code
		bPlay = false;
	else if (currentState == MENU && key == GLFW_KEY_Z) // Escape code
		currentState = PLAY;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}



