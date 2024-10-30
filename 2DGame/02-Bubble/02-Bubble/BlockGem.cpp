#include "BlockGem.h"

BlockGem::BlockGem(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) : Block(tileMapDispl, shaderProgram)
{
	blockType = Gem;
	sizeObject = glm::ivec2(16, 16);
	timeDyingAnim = 1500;
	spritesheet.loadFromFile("images/specialItems.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizeObject, glm::vec2(1 / 4.f, 0.5), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.5f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

void BlockGem::explode()
{
	entityState = DYING;
	speed = glm::vec2(0, 0);
}
