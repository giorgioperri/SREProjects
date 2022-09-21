//
// Created by Morten Nobel-Jørgensen on 12/09/2017.
//
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"
#include "Meteor.hpp"
#include "sre/Renderer.hpp"

Meteor::Meteor(const sre::Sprite &sprite) : GameObject(sprite) {
    scale = glm::vec2(0.5f,0.5f);
    winSize = sre::Renderer::instance->getDrawableSize();
    radius = 28;

    float randX1 = rand() % (int)(winSize.x-(winSize.x * 0.6f) + 1) + winSize.x * 0.8f;
    float randX2 = rand() % (int) winSize.x * 0.4f;
    float randY1 = rand() % (int)(winSize.y-(winSize.y * 0.6f) + 1) + winSize.y * 0.8f;
    float randY2 = rand() % (int) winSize.y * 0.4f;

    position.x = rand() % 50 > 25 ? randX1 : randX2;
    position.y = rand() % 50 > 25 ? randY1 : randY2;

    velocity = glm::vec2(rand() % 80,rand() % 80);
    rotationSpeed = rand() % 80;
}

void Meteor::update(float deltaTime) {
    position += velocity * deltaTime;
    rotation += deltaTime * rotationSpeed;

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

void Meteor::onCollision(std::shared_ptr<GameObject> other) {

}

void Meteor::onKey(SDL_Event &keyEvent) {

}
