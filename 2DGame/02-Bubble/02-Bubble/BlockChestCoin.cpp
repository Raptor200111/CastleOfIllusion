#include "BlockChestCoin.h"

BlockChestCoin::BlockChestCoin(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) : Block(tileMapPos, shaderProgram)
{
	blockType = Chest;
	sizeObject = glm::ivec2(16, 16);
	spritesheet.loadFromFile("images/specialItems.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizeObject, glm::vec2(1 / 4.f, 0.5), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(3.f / 4.f, 0.5f));

	sprite->setAnimationSpeed(1, 8);
	sprite->addKeyframe(1, glm::vec2(3.f / 4.f, 0.0f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

void BlockChestCoin::explode()
{
	if (blockType == BlockType::Chest)
	{
		speed = glm::vec2(0, 0);
		blockType = BlockType::Coin;
		explosionEfect->play(position + (getSize() / 2) - glm::ivec2(16, 18), 0);
		sprite->changeAnimation(1);
		entityState = STILL;
	}
	else if (blockType == BlockType::Coin)
	{
		entityState = DYING;
		speed = glm::vec2(0, 0);
	}
}