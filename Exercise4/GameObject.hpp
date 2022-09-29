#pragma once

#include "sre/SpriteBatch.hpp"
#include "sre/Sprite.hpp"
#include <SDL_events.h>
#include <string>

class GameObject {
public:
    bool queueForRemoval;
    explicit GameObject(sre::Sprite sprite);
    virtual ~GameObject();
    virtual void render(sre::SpriteBatch::SpriteBatchBuilder& spriteBatchBuilder);
    virtual void update(float deltaTime);
    virtual void onKey(SDL_Event& keyEvent);
    glm::vec2 position;
protected:
    sre::Sprite sprite;
    glm::vec2 scale;
    float rotation;
    friend class AsteroidsGame;
};
