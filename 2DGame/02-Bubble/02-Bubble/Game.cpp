#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"
#include "SoundManager.h"

void Game::init()
{
	bPlay = true;
	currentState = MENU;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	SoundManager::instance().init();
	sceneMenu.init();
	scenePlayLevel.init();
	scenePlayPractice.init();
	currentScene = &sceneMenu;
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
	else if (currentScene == &sceneMenu && key == GLFW_KEY_Z) {
		currentScene = &scenePlayPractice;
		scenePlayPractice.reStart();
	}
	else if (currentScene == &scenePlayPractice && key == GLFW_KEY_0) {
		currentScene = &scenePlayLevel;
		scenePlayLevel.reStart();
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

void Game::onPlayerFallDownHole() {
	if (godMode) return;
	stars = INIT_STARS;
	tries -= 1;
	if (tries <= 0) {
		looseGame();
	}
	else {
		ScenePlay* playScene = dynamic_cast<ScenePlay*>(currentScene);
		if (playScene) {
			playScene->reStart();
		}
	}
}

void Game::onPlayerKilled() 
{
	if (godMode) return;
	if (stars > 0)
	{
		stars -= 1;
	}
	if (stars <= 0)
	{
		stars = INIT_STARS;
		tries -= 1;
		//restart level
		ScenePlay* playScene = dynamic_cast<ScenePlay*>(currentScene);
		if (playScene) {
			playScene->reStart();
		}
	}
	if (tries <= 0)
	{
		looseGame();
	}
}

void Game::onExceededTimeLimit()
{
	looseGame();
}

void Game::onPracticeLevelWon()
{
	stars = INIT_STARS;
	tries = 8;
	currentScene = &scenePlayLevel;
	scenePlayLevel.reStart();
}

void Game::onLevelWon()
{
	//winGame();
	currentScene = &sceneMenu;
}

void Game::looseGame()
{
	currentScene = &sceneMenu;
}
