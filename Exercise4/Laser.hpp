#pragma once

#include "GameObject.hpp"
#include "Collidable.hpp"

class Laser : public GameObject, public Collidable  {
public:
    Laser(const sre::Sprite &sprite, glm::vec2 pos, float rotation);

    void update(float deltaTime) override;

    void onCollision(std::shared_ptr<GameObject> other) override;

    void onKey(SDL_Event &keyEvent) override;

    glm::vec2 velocity;
private:
    glm::vec2 winSize;
    float rotationSpeed;
    float speed = 400.0f;
};


