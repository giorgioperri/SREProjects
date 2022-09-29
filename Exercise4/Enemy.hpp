#pragma once

#include "AsteroidsGame.hpp"
#include "GameObject.hpp"
#include "Collidable.hpp"
#include "Laser.hpp"

class EnemyShip : public GameObject, public Collidable  {
public:
    explicit EnemyShip(const sre::Sprite &sprite, glm::vec2 pos = glm::vec2(50,50));
    ~EnemyShip() override;
    void update(float deltaTime) override;
    void onCollision(std::shared_ptr<GameObject> other) override;
    void fire();
    float spawnTime;
    float lastFireTime;
    glm::vec2 velocity{};
private:
    float drag = 0.80f;
    float maxSpeed = 150.0f;
    float thrustPower = 400.0f;
    glm::vec2 winSize{};
};


