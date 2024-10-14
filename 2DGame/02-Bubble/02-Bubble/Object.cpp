#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include "Object.h"
#include "Game.h"


void Object::init(const glm::ivec2& initialPos, const glm::ivec2& size, const string file, ShaderProgram& shaderProgram)
{
	posObject = initialPos;
	sizeObject = size;
	spritesheet.loadFromFile(file, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizeObject, glm::vec2(0.066, 0.098), &spritesheet, &shaderProgram);
	player = &Player::instance();
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f)); //ToDo:Hay que girar el Sprite ya que no tenemos animacion left
	sprite->addKeyframe(0, glm::vec2(0.066f, 0.f));

	sprite->setAnimationSpeed(1, 8);
	sprite->addKeyframe(1, glm::vec2(0.f, 0.f)); //ToDo:Hay que girar el Sprite ya que no tenemos animacion left
	sprite->addKeyframe(1, glm::vec2(0.066f, 0.f));
}

void Object::update(int deltaTime)
{
	
}

void Object::render()
{
	sprite->render();
}

void Object::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Object::setPosition(const glm::vec2& pos)
{
	posObject = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posObject.x), float(tileMapDispl.y + posObject.y)));
}




