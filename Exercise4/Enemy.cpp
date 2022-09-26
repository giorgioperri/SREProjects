//
// Created by Morten Nobel-Jørgensen on 12/09/2017.
//
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "Enemy.hpp"
#include "sre/Renderer.hpp"
#include <math.h>

EnemyShip::EnemyShip(const sre::Sprite &sprite, glm::vec2 pos) : GameObject(sprite) {
    scale = glm::vec2(0.5f,0.5f);
    winSize = sre::Renderer::instance->getDrawableSize();
    radius = 23;
    position = pos;
    velocity = glm::vec2(0.0f,0.0f);
    spawnTime = 0;
    lastFireTime = 0;
}

void EnemyShip::update(float deltaTime) {

    spawnTime += deltaTime;

    if(spawnTime - lastFireTime > rand() % 200) {
        fire();
        lastFireTime = spawnTime;
    }

    float acceleration = deltaTime*thrustPower;
    glm::vec2 direction = glm::rotateZ(glm::vec3(0,acceleration,0), glm::radians(rotation));
    velocity += direction;
    float speed = glm::length(velocity);
    if (speed > maxSpeed){
        velocity = velocity * (maxSpeed / speed);
    }

    velocity = velocity * (1.0f - drag*deltaTime);

    position += velocity * deltaTime;

    // wrap around
    if (position.x < 0){
        position.x += winSize.x;
    } else if (position.x > winSize.x){
        position.x -= winSize.x;
    }
    if (position.y < 0){
        position.y += winSize.y;
    } else if (position.y > winSize.y){
        position.y -= winSize.y;
    }
}

void EnemyShip::onCollision(std::shared_ptr<GameObject> other) {
    if (std::dynamic_pointer_cast<EnemyShip>(other) || std::dynamic_pointer_cast<Laser>(other)) return;
}

void EnemyShip::fire() {
    auto laserSprite = AsteroidsGame::atlas->get("laserRed01.png");
    AsteroidsGame::gameObjects.push_back(std::make_shared<Laser>(laserSprite, position, rotation, Enemy));
}

EnemyShip::~EnemyShip() = default;