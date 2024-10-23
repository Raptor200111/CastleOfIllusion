#pragma once

#include "Entity.h"
#include <glm/glm.hpp>
#include <cmath>
#include "Block.h"
class Enemy : public Entity
{
public:
    Enemy() {}
    virtual ~Enemy() {}

    // Common functions
    //virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) = 0;
    virtual void initMov(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const ZoneEnemy& initParams) = 0;
    virtual void update(int deltaTime) = 0;
    virtual void render() = 0;
    virtual void collideVertical() = 0;
    virtual void collideHorizontal() = 0;
    void moveHorizontal(bool left, int walk_speed);

    glm::ivec2 getInitPos() { return initParams.initPos; }

protected:
    float velocity;
    ZoneEnemy initParams;
};
