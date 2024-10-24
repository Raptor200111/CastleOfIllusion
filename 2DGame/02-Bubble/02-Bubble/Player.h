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
#include "Block.h"


enum PlayerStates
{
	IDLE, WALK, JUMP, FALL, DODGE, BUTT_FALL, BUTT_JUMP, READY_TO_PICK, CLIMB_IDLE, CLIMB, B_PICK, B_IDLE, B_WALK, B_JUMP, B_FALL, TOTAL //Importante que TOTAL este al final
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

	//Block* ThrownBlock() { return thrownBlock; }
	//Block* PickedUpBlock() { return pickedUpBlock; }

private:
	//Block* thrownBlock;
	Block* pickedUpBlock;

	bool bClimbing, bTouchBlock, bJumping;
	float yAxisSpeed;
	PlayerStates oldState, newState;
	ParticleEfect particleEfect;

	CollisionType colType = CollisionType::None;
	Block* block = nullptr;

	void leftMove();
	void rightMove();
	void changeToClimb();
	bool stopFallingCollision(Block*& block, CollisionType& colType);
	bool stairCollision();

	//Behaviour as in all that happens in every update with the following states:
	void movementBehaviour();
	void climbBehaviour();
	void jumpBehaviour();
	void fallBehaviour();
	void buttJumpBehaviour();
};

#endif


