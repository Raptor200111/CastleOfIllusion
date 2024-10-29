#include "BlockNonDestroyable.h"

BlockNonDestroyable::BlockNonDestroyable(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) : Block(tileMapPos, shaderProgram)
{
	blockType = NonDestroyable;
	sizeObject = glm::ivec2(16, 16);
	spritesheet.loadFromFile("images/blocks.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizeObject, glm::vec2(1 / 6.f, 0.5), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.5f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

void BlockNonDestroyable::explode()
{
	//entityState = DYING;
	speed = glm::vec2(0, 0);
	//explosionEfect->play(position + (getSize() / 2) - glm::ivec2(16, 18), 0);
}


