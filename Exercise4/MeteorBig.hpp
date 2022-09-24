#pragma once

#include "GameObject.hpp"
#include "Collidable.hpp"
#include "AsteroidsGame.hpp"

class MeteorBig : public GameObject, public Collidable  {
public:
    explicit MeteorBig(const sre::Sprite &sprite);

    void update(float deltaTime) override;

    void onCollision(std::shared_ptr<GameObject> other) override;

    void onKey(SDL_Event &keyEvent) override;

    void destroyAndSpawn();

    glm::vec2 velocity;
private:
    glm::vec2 winSize;
    float rotationSpeed;
};


