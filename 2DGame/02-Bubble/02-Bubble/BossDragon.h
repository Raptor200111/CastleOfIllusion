#pragma once

#include "Enemy.h"

enum BossDragonStates
{
	BOSS_IDLE //, BOSS_MOVE_RIGHT, BOSS_DIE, BOSS_ATTACK
};

enum BossBodyStates
{
	BOSS_BODY_IDLE
};
class BossDragon : public Enemy
{
public:
	BossDragon() { bodySprite = NULL; }
	~BossDragon() override { if (bodySprite != NULL) delete bodySprite; }
	//void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override {}
	void update(int deltaTime) override {}
	void initMov(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const ZoneEnemy& initParams) override;
	void update(int deltaTime, const glm::ivec2& posPlayer);
	void render() override;
	void setDragonPosition(const glm::vec2& pos) {
		position = pos;
		//top neck in body at 43 pixel. head is 32 pixels tall. 43-32 = 11. 
		if(left)
			sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x - offset.x), float(tileMapDispl.y + position.y+12 - offset.y)));
		else
		{
			int headX = position.x + sizeObjectBody.x - sizeObject.x;
			sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x+10 - offset.x), float(tileMapDispl.y + position.y + 12 - offset.y)));
		}
		bodySprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x - offset.x), float(tileMapDispl.y + position.y - offset.y)));
	}

private:
	void setBodyAnimations(ShaderProgram& shaderProgram);
	void setHeadAnimations(ShaderProgram& shaderProgram);
	int attackDistance = 3 * 16;//5*mapTileSize
	int attackSpeed;
	vector<glm::vec2> diffBodyHead;
		//body 1 + head 1 and 2 
		//if left
			//head1 3,11
			//head2  3,11
		//else 
			// head1 28, 11
			//	head2 31,11

		//body 2 + head 1 and 2
		//if left
			//head1 3,11
			//head2  3,11
		//else 
			// head1 28, 11
			//	head2 27,11

		//body 3 + head 1 and 2
		//if left
		//else 


		//body 1 + head 3 
		//if left 0, 12
		//else 37,12

		//body 2 + head 3
		//if left
		//else 

		//body 3 + head 3
		//if left
		//else 



		//body 1 + head 4 
		//if left 0, 12
		//else 37,12

		//body 2 + head 4
		//if left
		//else 

		//body 3 + head 4
		//if left
		//else 
	Texture bodySpritesheet;
	Sprite* bodySprite;
	glm::vec2 sizeObjectBody;
	BossDragonStates bossDragonState = BossDragonStates::BOSS_IDLE;
	BossBodyStates bossBodyState = BossBodyStates::BOSS_BODY_IDLE;

};

