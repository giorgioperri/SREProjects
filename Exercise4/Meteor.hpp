#pragma once

#include "GameObject.hpp"
#include "Collidable.hpp"
#include "AsteroidsGame.hpp"

enum MeteorSize {
    Small,
    Medium,
    Big
};

class Meteor : public GameObject, public Collidable  {
public:
    explicit Meteor(const sre::Sprite &sprite, MeteorSize meteorSize, glm::vec2 pos = glm::vec2(0,0));
    ~Meteor() override;
    void onCollision(std::shared_ptr<GameObject> other) override;
    void update(float deltaTime) override;
    glm::vec2 velocity;
    MeteorSize size;
private:
    float rotationSpeed;
    glm::vec2 winSize;
};


