#include "BlockCoin.h"

void BlockCoin::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	blockType = Coin;
	sizeObject = glm::ivec2(16, 16);
	spritesheet.loadFromFile("images/specialItems.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizeObject, glm::vec2(1 / 4.f, 0.5), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(3.f/4.f, 0.0f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));

}
