#ifndef _SPRITE_INCLUDE
#define _SPRITE_INCLUDE


#include <vector>
#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "AnimKeyframes.h"
#include "Defs.h"

// This class is derived from code seen earlier in TexturedQuad but it is also
// able to manage animations stored as a spritesheet. 


class Sprite
{

private:
	Sprite(const glm::vec2& quadSize, const glm::vec2& sizeInSpritesheet, Texture* spritesheet, ShaderProgram* program);

public:
	// Textured quads can only be created inside an OpenGL context
	static Sprite* createSprite(const glm::vec2& quadSize, const glm::vec2& sizeInSpritesheet, Texture* spritesheet, ShaderProgram* program);

	void update(int deltaTime);
	void render() const;
	void free();

	void setNumberAnimations(int nAnimations);
	void setAnimationSpeed(int animId, int keyframesPerSec);
	void addKeyframe(int animId, const glm::vec2& frame);
	void changeAnimation(int animId);
	int animation() const;

	void setPosition(const glm::vec2& pos);
	void setLeft(bool left);
	void addKeyframeDiffSize(int animId, const glm::vec2& frame, const glm::vec2& quadSize, const glm::vec2& sizeInSpritesheet);
	glm::vec2 updateDiffSize(int deltaTime);
	glm::vec2 changeAnimationDiffSize(int animId);
	void updateVertexData(const glm::vec2& quadSize, const glm::vec2& sizeInSpritesheet);
	void setPlayingOnce(bool playingOnce);
	bool getPlayingNow();
	void play();
	void playOnce();
	void stop();
	glm::ivec2 getCurrentKeyframe() { return glm::ivec2(currentKeyframe, animations[currentAnimation].keyframeDispl.size()); }

private:
	Texture* texture;
	ShaderProgram* shaderProgram;
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::vec2 position;
	glm::vec2 size;
	int currentAnimation, currentKeyframe;
	float timeAnimation;
	glm::vec2 texCoordDispl;
	vector<AnimKeyframes> animations;
	bool left, playingOnce, playingNow;
	glm::vec2 currentQuadSize;
	glm::vec2 currentSpriteSheetSize;
};

#endif // _SPRITE_INCLUDE