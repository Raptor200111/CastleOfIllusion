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
	~Player();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	//----fet per liliu: check if ok
	void reStartStatePlayer();
	//return if player is attacking
	bool isAttacking() { return newState == BUTT_FALL; }
	//----

	//Block* ThrownBlock() { return thrownBlock; }
	Block* getPickedUpBlock() { return pickedUpBlock; }

private:
	//Block* thrownBlock;
	Block* pickedUpBlock;
	Block* readyToPickBlock;

	bool jumpAvailable, objInteractionAvailable;
	float yAxisSpeed;
	PlayerStates oldState, newState;
	ParticleEfect* particleEfect;

	CollisionType colType = CollisionType::None;
	Block* block = nullptr;

	//----fet per liliu: check if ok
	int elapsedTime = 0;
	int timeDyingAnim = 3000;
	bool paint = true;
	//----

	void leftMove();
	void rightMove();
	void changeToClimb();
	bool stopFallingCollision(Block*& block, CollisionType& colType);
	bool stairCollision();

	//Behaviour as in all that happens in every update with the following states:
	void movementBehaviour();
	void climbBehaviour();
	void buttJumpBehaviour();
	void blockMovementBehaviour();

	void pickUpBlock();
	void throwBlock();

	bool checkJumpButton();
	bool checkObjInteractionButton();

	void playerButtJump();
};

#endif


