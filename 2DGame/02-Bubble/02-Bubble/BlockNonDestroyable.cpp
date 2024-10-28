#include "BlockNonDestroyable.h"

BlockNonDestroyable::BlockNonDestroyable(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) : Block(tileMapPos, shaderProgram)
{
	blockType = NonDestroyable;
	blockStatus = ITEM_STATUS;
	sizeObject = glm::ivec2(16, 16);
	spritesheet.loadFromFile("images/blocks.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizeObject, glm::vec2(1 / 6.f, 0.5), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(ITEM_STATUS, 8);

	sprite->addKeyframe(ITEM_STATUS, glm::vec2(0.f, 0.5f));

	sprite->changeAnimation(blockStatus);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}


