#pragma once

#include "GameObject.hpp"
#include "Collidable.hpp"
#include "AsteroidsGame.hpp"

class MeteorMedium : public GameObject, public Collidable  {
public:
    MeteorMedium(const sre::Sprite &sprite, glm::vec2 pos);

    void update(float deltaTime) override;

    void onCollision(std::shared_ptr<GameObject> other) override;

    void destroyAndSpawn();

    glm::vec2 velocity;
private:
    glm::vec2 winSize;
    float rotationSpeed;
};


