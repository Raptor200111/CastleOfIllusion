#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include <cmath>


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

enum PlayerStates
{
	STAND, WALK, JUMP, FALL, DODGE, BUTT_FALL, BUTT_JUMP, FALL_TO_STAND
};

class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	
private:
	bool bJumping, buttJumping;
	glm::ivec2 tileMapDispl, posPlayer, sizePlayer = glm::ivec2(32, 48);
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	float velocity;

	PlayerStates playerState;

};


#endif // _PLAYER_INCLUDE


