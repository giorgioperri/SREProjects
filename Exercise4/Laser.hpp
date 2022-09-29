#pragma once

#include "AsteroidsGame.hpp"
#include "GameObject.hpp"
#include "Collidable.hpp"
#include <time.h>

enum LaserType {
    Ally,
    Enemy
};

class Laser : public GameObject, public Collidable  {
public:
    explicit Laser(const sre::Sprite &sprite, glm::vec2 pos, float rot, LaserType laserType = Ally);
    ~Laser() override;
    void onCollision(std::shared_ptr<GameObject> other) override;
    void update(float deltaTime) override;
    LaserType currentLaserType;
    glm::vec2 velocity;
private:
    float speed = 400.0f;
    float spawnTime = 0;
    glm::vec2 winSize;
};


