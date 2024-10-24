#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include <GLFW/glfw3.h>
#include "Scene.h"
#include "MenuScene.h"
#include "LevelScene.h"


#define SCREEN_WIDTH 640 
#define SCREEN_HEIGHT 480


// Game is a singleton (a class with a single instance) that represents our whole application

enum GameState {
	MENU,
	INSTRUCTIONS,
	PLAY
};
class Game
{

private:
	Game() {}

public:
	static Game& instance()
	{
		static Game G;

		return G;
	}

	void init();
	bool update(int deltaTime);
	void render();

	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);

	bool getKey(int key) const;


	int getStars() { return stars; }
	int getScore() { return score; }
	int getTries() { return tries; }
	void addStar() { stars += 1; }
	void substractStar() { stars -= 1; }
	void onPlayerKilledEnemy() { score += ENEMY_POINTS; }
	void onPlayerFallDownHole();
	void onPlayerKilled();
	void onPracticeLevelWon();
	void onLevelWon();
	void onGetCake() { if (stars < MAX_STARS) stars += 1; }
	void onGetCoin() { score += COIN_POINTS; }
	void onExceededTimeLimit();
	void onHeal() { stars = INIT_STARS; }

private:
	void looseGame();

	bool bPlay; // Continue to play game?
	bool keys[GLFW_KEY_LAST+1]; // Store key states so that 
							    // we can have access at any time
	Scene* currentScene;
	GameState currentState;
	MenuScene menuScene;
	LevelScene levelScene;

	int score = 0;
	int tries = 3;
	int stars = 3;
	bool godMode = false;
};


#endif // _GAME_INCLUDE


