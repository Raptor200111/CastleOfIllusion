#include "Block.h"

void Block::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void Block::render()
{
	sprite->render();
}