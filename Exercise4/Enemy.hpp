#pragma once

#include "GameObject.hpp"
#include "Collidable.hpp"
#include "AsteroidsGame.hpp"
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
    float maxSpeed = 230.0f;
    float thrustPower = 600.0f;
    glm::vec2 winSize{};
};


