#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include <cmath>


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

enum PlayerStates
{
	STAND, WALK, JUMP, FALL, DODGE, BUTT_FALL, BUTT_JUMP, FALL_TO_STAND, CLIMB, TOUCH_BLOCK
};

class Player
{

public:
	static Player& instance()
	{
		static Player P;

		return P;
	}
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::vec4 getPositionAndSize();
	glm::ivec2 getPlayerPos() const;
  
	bool checkCollisionObject(const PosSizeObject& object);

private:
	bool bJumping, buttJumping, bClimbing, bTouchBlock;
	glm::ivec2 tileMapDispl, posPlayer, sizePlayer = glm::ivec2(24, 32);
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	float velocity;

	PlayerStates playerState;

};


#endif // _PLAYER_INCLUDE


