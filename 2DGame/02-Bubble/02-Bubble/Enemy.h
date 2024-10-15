#pragma once

#include "Sprite.h"
#include "TileMap.h"
#include <glm/glm.hpp>
#include <cmath>

class Enemy {
public:
    Enemy() {}
    virtual ~Enemy() {}

    // Common functions
    //virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) = 0;
    virtual void initMov(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const ZoneEnemy& initParams) = 0;
    virtual void update(int deltaTime) = 0;
    virtual void render() = 0;

    void setTileMap(TileMap* tileMap) { map = tileMap; }
    void setPosition(const glm::vec2& pos) {
        posEnemy = pos;
        sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
    }

    glm::ivec2 getEnemyPos() const { return posEnemy; }
    glm::ivec2 getEnemySize() const { return sizeEnemy; }
    void moveHorizontal(bool left, int walk_speed);

protected:
    glm::ivec2 tileMapDispl, posEnemy, sizeEnemy, initPos;
    TileMap* map;
    Sprite* sprite;
    Texture spritesheet;
    float velocity;
    bool left;
    ZoneEnemy initParams;
};
