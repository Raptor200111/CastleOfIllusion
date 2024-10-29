#include "BlockCake.h"

BlockCake::BlockCake(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) : Block(tileMapPos, shaderProgram)
{
	blockType = Chest;
	blockStatus = CHEST_STATUS;
	sizeObject = glm::ivec2(16, 16);
	spritesheet.loadFromFile("images/specialItems.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizeObject, glm::vec2(1 / 4.f, 0.5), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(CHEST_STATUS, 8);
	sprite->addKeyframe(CHEST_STATUS, glm::vec2(3.f / 4.f, 0.5f));

	sprite->setAnimationSpeed(ITEM_STATUS, 8);
	sprite->addKeyframe(ITEM_STATUS, glm::vec2(0.f, 0.0f));

	sprite->changeAnimation(blockStatus);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}
