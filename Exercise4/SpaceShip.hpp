#pragma once

#include "GameObject.hpp"
#include "Collidable.hpp"
#include "AsteroidsGame.hpp"
#include "Laser.hpp"

class SpaceShip : public GameObject, public Collidable  {
public:
    explicit SpaceShip(const sre::Sprite &sprite);
    ~SpaceShip() override;
    void update(float deltaTime) override;
    void onCollision(std::shared_ptr<GameObject> other) override;
    void fire();
    void restart();
    void destroySelf();
    void onKey(SDL_Event &keyEvent) override;
    bool isDead = false;
    glm::vec2 velocity{};
private:
    bool rotateCW = false;
    bool rotateCCW = false;
    bool thrust = false;
    float drag = 0.80f;
    float maxSpeed = 460.0f;
    float thrustPower = 600.0f;
    float rotationSpeed = 100.0f;
    glm::vec2 winSize{};
};


