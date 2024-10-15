#include "Enemy.h"

void Enemy::moveHorizontal(bool left, int walk_speed)
{
	if (left)
	{
		posEnemy.x -= walk_speed;
	}
	else {
		posEnemy.x += walk_speed;
	}
}
