#pragma once

#include "Entity.h"
#include <glm/glm.hpp>
#include <cmath>

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

    void moveHorizontal(bool left, int walk_speed);

protected:
    glm::ivec2 initPos;
    float velocity;
    bool left;
    ZoneEnemy initParams;
};
