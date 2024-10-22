#define _USE_MATH_DEFINES

#include "BossShoot.h"
#include <cmath>
#include <iostream>

void BossShoot::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	endDirection = glm::vec2(1.0f);

	setShootAnimations(shaderProgram);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));

}
void BossShoot::update(int deltaTime) {
	sprite->updateDiffSize(deltaTime);
	if (active) {
		float deltaSeconds = deltaTime / 1000.0f;
		glm::vec2 direction = glm::vec2(endDirection - glm::vec2(position));
		glm::vec2 normalizedDirection = glm::normalize(direction);
		glm::vec2 movement = normalizedDirection * speed * deltaSeconds;
		position += glm::ivec2(glm::round(movement));
		//cout << "ShootV" << velocity.x << " " << velocity.y << "\n";
		//cout << "ShootAdd" << add.x << " " <<z add.y << "\n";
		if(position.x < 0 || position.x > 96*16 || position.y < 0 || position.y> 736)
		{
			elapsedTime = 0;
			active = false;
		}
	}
	if (sprite->animation() != shootState) {
		glm::vec2 aux = sprite->changeAnimationDiffSize(shootState);
		if (aux != glm::vec2(0.f))
			sizeObject = aux;
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

void BossShoot::render() {
	if(active)
		sprite->render();
}

void BossShoot::setDirection(glm::vec2 direction)
{
	this->endDirection = direction;
}
void BossShoot::setShootAnimations(ShaderProgram& shaderProgram)

{
	spritesheet.loadFromFile("images/dragonShoots.png", TEXTURE_PIXEL_FORMAT_RGBA);

	glm::vec2 totalSizeSpriteSheet = glm::vec2(47.f, 36.f);
	sizeObject = totalSizeSpriteSheet;
	glm::vec2 sizeSpriteSheet = glm::vec2(sizeObject.x / totalSizeSpriteSheet.x, sizeObject.y / totalSizeSpriteSheet.y);
	sprite = Sprite::createSprite(sizeObject, sizeSpriteSheet, &spritesheet, &shaderProgram);
	//sprite->setNumberAnimations(2);
	sprite->setNumberAnimations(1);


	sprite->setAnimationSpeed(BOSS_SHOOT_START, 8);
	glm::vec2 frame;

	sizeObject = glm::vec2(8.f, 8.f);
	sizeSpriteSheet = glm::vec2(sizeObject.x / totalSizeSpriteSheet.x, sizeObject.y / totalSizeSpriteSheet.y);
	frame = glm::vec2(0.f, 3.f / totalSizeSpriteSheet.y);
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
