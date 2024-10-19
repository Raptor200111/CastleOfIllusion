#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "SoundManager.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Scene
{
public:
    //Scene();
    virtual ~Scene() {}

    // Virtual methods for initialization, update, and render
    virtual void init() = 0;
    virtual void update(int deltaTime) = 0;
    virtual void render() = 0;
};


#endif // _SCENE_INCLUDE

