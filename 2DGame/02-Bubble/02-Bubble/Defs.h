#pragma once
#include <vector>


using namespace std;

struct Zone
{
	float min_x;
	float max_x;
	float min_y;
	float max_y;
};
enum EnemyType
{
	Tree,
	Bug,
};

enum EntityState
{
	Alive,
	Dead,
	Dying
};

enum CollisionType
{
	None,
	Tile,
	Stairs,
	TileStairs
};

enum VColType
{
	NoVcol,
	Down,
	Up,
};

enum HColType
{
	NoHcol,
	Left,
	Right,
};
struct BlockObj
{
	int type;
	glm::ivec2 pos;

};

struct ZoneEnemy
{
	Zone limit;
	glm::ivec2 initPos;
	bool left;
};

struct Cam {
	float left, right, bottom, top;
};

struct InitEnemy
{
	EnemyType enemyType;
	Zone limit;
	glm::ivec2 initPos;
	bool left;
};

struct PosSizeObject
{
	glm::ivec2 posObject;
	glm::ivec2 sizeObject;
};
