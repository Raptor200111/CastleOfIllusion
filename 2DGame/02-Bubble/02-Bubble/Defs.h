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

struct ZoneEnemy
{
	Zone limit;
	glm::ivec2 initPos;
};

struct PosSizeObject
{
	glm::ivec2 posObject;
	glm::ivec2 sizeObject;
};
