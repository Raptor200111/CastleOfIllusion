#include "BossShoot.h"

void BossShoot::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	velocity = 0.f;
	tileMapDispl = tileMapPos;
	setShootAnimations(shaderProgram);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));

}
void BossShoot::update(int deltaTime) {

}
void BossShoot::render() {
	sprite->render();
}


void BossShoot::setShootAnimations(ShaderProgram& shaderProgram)

{
	spritesheet.loadFromFile("images/dragonShoots.png", TEXTURE_PIXEL_FORMAT_RGBA);

	glm::vec2 totalSizeSpriteSheet = glm::vec2(47.f, 36.f);
	sizeObject = glm::ivec2(75.f, 110.f);
	glm::vec2 sizeSpriteSheet = glm::vec2(sizeObject.x / totalSizeSpriteSheet.x, 1);
	sprite = Sprite::createSprite(sizeObject, sizeSpriteSheet, &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);


	sprite->setAnimationSpeed(BOSS_SHOOT_START, 8);

	sizeObject = glm::vec2(8.f, 8.f);
	sizeSpriteSheet = glm::vec2(sizeObject.x / totalSizeSpriteSheet.x, sizeObject.y / totalSizeSpriteSheet.y);
	glm::vec2 frame = glm::vec2(0.f, 3.f / totalSizeSpriteSheet.y);
	sprite->addKeyframeDiffSize(BOSS_SHOOT_START, frame, sizeObject, sizeSpriteSheet);

	sizeObject = glm::vec2(12.f, 12.f);
	sizeSpriteSheet = glm::vec2(sizeObject.x / totalSizeSpriteSheet.x, sizeObject.y / totalSizeSpriteSheet.y);
	frame = glm::vec2(13.f / totalSizeSpriteSheet.x, 2.f / totalSizeSpriteSheet.y);
	sprite->addKeyframeDiffSize(BOSS_SHOOT_START, frame, sizeObject, sizeSpriteSheet);

	sizeObject = glm::vec2(16.f, 16.f);
	sizeSpriteSheet = glm::vec2(sizeObject.x / totalSizeSpriteSheet.x, sizeObject.y / totalSizeSpriteSheet.y);
	frame = glm::vec2(31.f / totalSizeSpriteSheet.x, 0.f);
	sprite->addKeyframeDiffSize(BOSS_SHOOT_START, frame, sizeObject, sizeSpriteSheet);


	sprite->setAnimationSpeed(BOSS_SHOOT_END, 8);

	sizeObject = glm::vec2(16.f, 16.f);
	sizeSpriteSheet = glm::vec2(sizeObject.x / totalSizeSpriteSheet.x, sizeObject.y / totalSizeSpriteSheet.y);
	frame = glm::vec2(3.f / totalSizeSpriteSheet.x, 20.f / totalSizeSpriteSheet.y);
	sprite->addKeyframeDiffSize(BOSS_SHOOT_END, frame, sizeObject, sizeSpriteSheet);

	sizeObject = glm::vec2(12.f, 12.f);
	sizeSpriteSheet = glm::vec2(sizeObject.x / totalSizeSpriteSheet.x, sizeObject.y / totalSizeSpriteSheet.y);
	frame = glm::vec2(23.f / totalSizeSpriteSheet.x, 23.f / totalSizeSpriteSheet.y);
	sprite->addKeyframeDiffSize(BOSS_SHOOT_END, frame, sizeObject, sizeSpriteSheet);


	glm::vec2 aux = sprite->changeAnimationDiffSize(shootState);
	if (aux != glm::vec2(0.f))
		sizeObject = aux;
}
