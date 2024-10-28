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
	sceneInstructions.init();
	sceneCredits.init();
	sceneFinalScreen.init();
	scenePlayPractice.init();
	scenePlayLevel.init();
	currentScene = &sceneMenu;
	SoundManager::instance().setMusicVolume(40);
	//SoundManager::instance().playMusic("menu", -1);
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
	else if (key == GLFW_KEY_Z) {
		currentScene = &scenePlayPractice;
		tries = 3;
		scenePlayPractice.reStart();
		SoundManager::instance().setMusicVolume(40);
		//SoundManager::instance().playMusic("level", -1);
	}
	else if (godMode && currentScene == &scenePlayPractice && key == GLFW_KEY_0) {
		currentScene = &scenePlayLevel;
		tries = 8;
		scenePlayLevel.reStart();
	}
	else if (currentScene != &sceneFinalScreen && key == GLFW_KEY_I) {
		currentScene = &sceneInstructions;
	}
	else if (key == GLFW_KEY_M) {
		sceneFinalScreen.setWon(false);
		currentScene = &sceneMenu;
	}
	else if (currentScene != &sceneFinalScreen && key == GLFW_KEY_C) {
		currentScene = &sceneCredits;
	}
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	if (keys[GLFW_KEY_G])
	{
		godMode = !godMode;
	}
	if (keys[GLFW_KEY_H]) {
		onHeal();
	}
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
	sceneFinalScreen.setWon(true);
	currentScene = &sceneFinalScreen;
	SoundManager::instance().setMusicVolume(40);
	//SoundManager::instance().playMusic("menu", -1);
}

void Game::looseGame()
{
	sceneFinalScreen.setWon(false);
	currentScene = &sceneFinalScreen;
	SoundManager::instance().setMusicVolume(40);
	//SoundManager::instance().playMusic("menu", -1);
}
