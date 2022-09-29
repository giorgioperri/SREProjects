#pragma once

#include "AsteroidsGame.hpp"
#include "GameObject.hpp"
#include "Collidable.hpp"
#include "Laser.hpp"

enum PlayerNumber {
    PlayerOne,
    PlayerTwo
};

enum ShipType {
    Original,
    Clone
};

class SpaceShip : public GameObject, public Collidable  {
public:
    explicit SpaceShip(const sre::Sprite &sprite, PlayerNumber pNum, glm::vec2 pos, float rot = 0);
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
    bool rotateCCW = false;
    bool rotateCW = false;
    bool thrust = false;
    float rotationSpeed = 150.0f;
    float thrustPower = 600.0f;
    float maxSpeed = 460.0f;
    float drag = 0.80f;
    PlayerNumber playerNumber;
    glm::vec2 winSize{};
};


