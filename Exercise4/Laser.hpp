#pragma once

#include "GameObject.hpp"
#include "Collidable.hpp"
#include <time.h>
#include "AsteroidsGame.hpp"

enum LaserType {
    Ally,
    Enemy
};

class Laser : public GameObject, public Collidable  {
public:
    explicit Laser(const sre::Sprite &sprite, glm::vec2 pos, float rotation, LaserType laserType = Ally);
    ~Laser() override;
    void update(float deltaTime) override;
    void onCollision(std::shared_ptr<GameObject> other) override;
    glm::vec2 velocity;
    LaserType currentLaserType;
private:
    glm::vec2 winSize;
    float rotationSpeed;
    float speed = 400.0f;
    float spawnTime = 0;
};


