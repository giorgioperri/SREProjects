#pragma once

#include "GameObject.hpp"
#include "Collidable.hpp"

class Meteor : public GameObject, public Collidable  {
public:
    Meteor(const sre::Sprite &sprite);

    void update(float deltaTime) override;

    void onCollision(std::shared_ptr<GameObject> other) override;

    void onKey(SDL_Event &keyEvent) override;

    glm::vec2 velocity;
private:
    glm::vec2 winSize;
    float rotationSpeed;
};


