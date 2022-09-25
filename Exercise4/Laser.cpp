//
// Created by Morten Nobel-Jørgensen on 12/09/2017.
//
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"
#include "Laser.hpp"
#include "SpaceShip.hpp"
#include "sre/Renderer.hpp"

Laser::Laser(const sre::Sprite &sprite, glm::vec2 pos, float rotation) : GameObject(sprite) {
    scale = glm::vec2(0.5f,0.5f);
    winSize = sre::Renderer::instance->getDrawableSize();
    radius = 5;

    position = pos;

    this->rotation = rotation;

    glm::vec2 direction = glm::rotateZ(glm::vec3(0,speed,0), glm::radians(rotation));
    velocity = direction;
}

void Laser::update(float deltaTime) {

    spawnTime += deltaTime;

    if(spawnTime > 1) {
        queueForRemoval = true;
    }

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

void Laser::onCollision(std::shared_ptr<GameObject> other) {

    if(std::dynamic_pointer_cast<SpaceShip>(other)) return;

    queueForRemoval = true;

}

Laser::~Laser() = default;
