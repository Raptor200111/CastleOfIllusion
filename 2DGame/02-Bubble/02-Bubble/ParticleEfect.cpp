#include "ParticleEfect.h"
#include "Game.h"

void ParticleEfect::init(const glm::ivec2& tileMapPos, glm::ivec2& pos, glm::ivec2& siz, ShaderProgram& shaderProgram, string file, glm::vec2 weirdSize, int numAnims)
{
	tileMapDispl = tileMapPos;
	position = pos;
	sizeSprite = siz;

	spritesheet.loadFromFile(file, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizeSprite, weirdSize, &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(numAnims);
	//sprite->changeAnimation(0);
	//sprite->stop();
	this->numAnims = 0;
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

void ParticleEfect::addAnimation(int frameRate, glm::vec2* framesArray, int length)
{
	sprite->setAnimationSpeed(numAnims, frameRate);
	for (int i = 0; i < length; i++)
	{
		sprite->addKeyframe(numAnims, framesArray[i]);
	}

	numAnims++;
}

void ParticleEfect::play(const glm::vec2& pos, int anim)
{
	setPosition(pos);
	sprite->changeAnimation(anim);
	sprite->stop();
	sprite->playOnce();
}
