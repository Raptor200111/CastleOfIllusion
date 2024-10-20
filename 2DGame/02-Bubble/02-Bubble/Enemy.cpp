#include "Enemy.h"

void Enemy::moveHorizontal(bool left, int walk_speed)
{
	if (left)
	{
		position.x -= walk_speed;
	}
	else {
		position.x += walk_speed;
	}
}
