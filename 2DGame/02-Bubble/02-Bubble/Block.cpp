#include "Block.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Game.h"


void Block::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int numBlock)
{
	spritesheet.loadFromFile("images/blocks.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizeBlock, glm::vec2(1 / 6.f, 0.5), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 8);
	numBlock = numBlock - 1;
	if (numBlock < 6) {
		sprite->addKeyframe(0, glm::vec2(numBlock / 6.f, 0.0f));

	}
	else {
		sprite->addKeyframe(0, glm::vec2((numBlock - 6) / 6.f, 0.5f));
	}

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBlock.x), float(tileMapDispl.y + posBlock.y)));

}
void Block::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void Block::render()
{
	sprite->render();
}

void Block::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Block::setPosition(const glm::vec2& pos)
{
	posBlock = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBlock.x), float(tileMapDispl.y + posBlock.y)));
}

glm::ivec2 Block::getBlockPos() const
{
	return posBlock;
}