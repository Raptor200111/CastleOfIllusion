#include "ParticleEfect.h"


void ParticleEfect::init(const glm::ivec2& tileMapPos, glm::ivec2& pos, glm::ivec2& siz, ShaderProgram& shaderProgram, string file, int frameRate, glm::vec2 weirdSize, glm::vec2* framesArray)
{
	tileMapDispl = tileMapPos;
	position = pos;
	size = siz;

	spritesheet.loadFromFile(file, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(size, glm::vec2(0.066, 0.098), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, frameRate);
	for (int i = 0; i < framesArray->length(); i++)
	{
		sprite->addKeyframe(0, framesArray[i]);
	}

	sprite->changeAnimation(0);
}

void ParticleEfect::update(int deltaTime)
{
	sprite->update(deltaTime);
	setPosition(position);
}

void ParticleEfect::render()
{
	if (sprite->getPlayingNow())
		sprite->render();
}

void ParticleEfect::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void ParticleEfect::setPosition(const glm::vec2& pos)
{
	position = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

void ParticleEfect::play(const glm::vec2& pos)
{
	setPosition(pos);
	sprite->playOnce();
}
