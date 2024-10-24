#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Sprite.h"
#include "TileMap.h"
#include "ParticleEfect.h"
#include "Entity.h"
#include "CollisionManager.h"


enum PlayerStates
{
	IDLE, WALK, JUMP, FALL, DODGE, BUTT_FALL, BUTT_JUMP, READY_TO_PICK, CLIMB_IDLE, CLIMB, TOTAL //Importante que TOTAL este al final
};

class Player : public Entity
{
public:
	static Player& instance() {
		static Player P;
		return P;
	}

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

private:
	bool bClimbing, bTouchBlock, bJumping;
	float yAxisSpeed;
	PlayerStates oldState, newState;
	ParticleEfect particleEfect;

	void leftMove();
	void rightMove();
	void changeToClimb();
	bool stopFallingCollision(Block*& block, CollisionType& colType);
	bool stairCollision();
};

#endif // _PLAYER_INCLUDE


