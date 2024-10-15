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

struct ZoneEnemy
{
	Zone limit;
	glm::ivec2 initPos;
	bool left;
};

struct InitEnemy
{
	int id;
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
