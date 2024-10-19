#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"

void Game::init()
{
	bPlay = true;
	currentState = MENU;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	currentScene = &menuScene;
	currentScene->init();
}

bool Game::update(int deltaTime)
{
	currentScene->update(deltaTime);
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	currentScene->render();
}

void Game::keyPressed(int key)
{
	if (key == GLFW_KEY_ESCAPE) // Escape code
	{
		bPlay = false;
	}
	else if (currentScene == &menuScene && key == GLFW_KEY_Z) {
		currentScene = &levelScene;
		currentScene->init();
	}
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



