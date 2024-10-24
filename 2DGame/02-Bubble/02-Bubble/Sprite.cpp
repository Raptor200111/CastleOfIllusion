#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Sprite.h"
#include <iostream>

Sprite *Sprite::createSprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program)
{
	Sprite *quad = new Sprite(quadSize, sizeInSpritesheet, spritesheet, program);

	return quad;
}


Sprite::Sprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program)
{
	float vertices[24] = {0.f, 0.f, 0.f, 0.f, 
												quadSize.x, 0.f, sizeInSpritesheet.x, 0.f, 
												quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y, 
												0.f, 0.f, 0.f, 0.f, 
												quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y, 
												0.f, quadSize.y, 0.f, sizeInSpritesheet.y};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLocation = program->bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program->bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
	texture = spritesheet;
	shaderProgram = program;
	currentAnimation = -1;
	position = glm::vec2(0.f);
	size = quadSize;
	left = false;
	playingOnce = false;
	playingNow = false;
}

void Sprite::update(int deltaTime)
{
	if(playingNow)
	{
		timeAnimation += deltaTime;
		while(timeAnimation > animations[currentAnimation].millisecsPerKeyframe)
		{
			timeAnimation -= animations[currentAnimation].millisecsPerKeyframe;
			currentKeyframe = (currentKeyframe + 1) % animations[currentAnimation].keyframeDispl.size();
		}
		texCoordDispl = animations[currentAnimation].keyframeDispl[currentKeyframe];
		if (playingOnce && (currentKeyframe == (animations[currentAnimation].keyframeDispl.size() - 1)))
		{
			playingOnce = false;
			playingNow = false;
		}
	}
}

void Sprite::render() const
{
	glm::mat4 modelview = glm::mat4(1.0f);
	modelview = glm::translate(modelview, glm::vec3(position.x, position.y, 0.f));
	if (left) {
		modelview = glm::translate(modelview, glm::vec3(size.x / 2, 0.f, 0.f));
		modelview = glm::rotate(modelview, 3.141592f, glm::vec3(0.f, 1.f, 0.f));
		modelview = glm::translate(modelview, glm::vec3(-size.x / 2, 0.f, 0.f));
	}
	shaderProgram->setUniformMatrix4f("modelview", modelview);
	shaderProgram->setUniform2f("texCoordDispl", texCoordDispl.x, texCoordDispl.y);
	glEnable(GL_TEXTURE_2D);
	texture->use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}

void Sprite::free()
{
	glDeleteBuffers(1, &vbo);
}

void Sprite::setNumberAnimations(int nAnimations)
{
	animations.clear();
	animations.resize(nAnimations);
}

void Sprite::setAnimationSpeed(int animId, int keyframesPerSec)
{
	if(animId < int(animations.size()))
		animations[animId].millisecsPerKeyframe = 1000.f / keyframesPerSec;
}

void Sprite::addKeyframe(int animId, const glm::vec2 &displacement)
{
	if(animId < int(animations.size()))
		animations[animId].keyframeDispl.push_back(displacement);
}

void Sprite::changeAnimation(int animId)
{
	if (animId < int(animations.size()))
	{
		playingNow = true;
		currentAnimation = animId;
		currentKeyframe = 0;
		timeAnimation = 0.f;
		texCoordDispl = animations[animId].keyframeDispl[0];
	}
	else
		playingNow = false;
}

//Added
glm::vec2 Sprite::updateDiffSize(int deltaTime)
{
	glm::vec2 quadsize = glm::vec2(0.f);
	if (currentAnimation >= 0)
	{
		timeAnimation += deltaTime;
		while (timeAnimation > animations[currentAnimation].millisecsPerKeyframe)
		{
			timeAnimation -= animations[currentAnimation].millisecsPerKeyframe;
			currentKeyframe = (currentKeyframe + 1) % animations[currentAnimation].keyframeDispl.size();

			//changed
			updateVertexData(animations[currentAnimation].quadSizes[currentKeyframe], animations[currentAnimation].spriteSheetSizes[currentKeyframe]);

		}
		texCoordDispl = animations[currentAnimation].keyframeDispl[currentKeyframe];
		quadsize = animations[currentAnimation].quadSizes[currentKeyframe];
	}
	return quadsize;
}

void Sprite::addKeyframeDiffSize(int animId, const glm::vec2& displacement, const glm::vec2& quadSize, const glm::vec2& sizeInSpritesheet)
{
	if (animId < int(animations.size()))
	{
		animations[animId].keyframeDispl.push_back(displacement);
		
		//changed
		animations[animId].quadSizes.push_back(quadSize);  
		animations[animId].spriteSheetSizes.push_back(sizeInSpritesheet);  

	}
}

glm::vec2 Sprite::changeAnimationDiffSize(int animId)
{
	glm::vec2 quadsize = glm::vec2(0.f);
	if (animId < int(animations.size()))
	{
		currentAnimation = animId;
		currentKeyframe = 0;
		timeAnimation = 0.f;
		texCoordDispl = animations[animId].keyframeDispl[0];
		
		//changed
		currentQuadSize = animations[animId].quadSizes[0];  
		currentSpriteSheetSize = animations[animId].spriteSheetSizes[0];
		updateVertexData(currentQuadSize, currentSpriteSheetSize);
		quadsize = currentQuadSize;
	}
	return quadsize;
}

void Sprite::updateVertexData(const glm::vec2& quadSize, const glm::vec2& sizeInSpritesheet)
{
	float vertices[24] = {
		0.f, 0.f, 0.f, 0.f,
		quadSize.x, 0.f, sizeInSpritesheet.x, 0.f,
		quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y,
		0.f, 0.f, 0.f, 0.f,
		quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y,
		0.f, quadSize.y, 0.f, sizeInSpritesheet.y };

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);

}

void Sprite::setPlayingOnce(bool playingOnce)
{
	this->playingOnce = playingOnce;
}

bool Sprite::getPlayingNow()
{
	return playingNow;
}

void Sprite::play()
{
	if (currentAnimation >= 0)
		playingNow = true;
}

void Sprite::stop()
{
	playingNow = false;
}

void Sprite::playOnce()
{
	playingOnce = true;
	play();
}

int Sprite::animation() const
{
	return currentAnimation;
}

void Sprite::setPosition(const glm::vec2 &pos)
{
	position = pos;
}

void Sprite::setLeft(bool left) {
	this->left = left;
}

